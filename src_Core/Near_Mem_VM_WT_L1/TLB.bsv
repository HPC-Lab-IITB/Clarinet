// Copyright (c) 2018-2020 Bluespec, Inc. All Rights Reserved

package TLB;

// ================================================================
// This package implements a TLB (Translation Lookaside Buffer)
// for use by RISC-V system MMUs
// for VM schemes Sv32 (for RV32) and Sv39 (for RV64)

// ================================================================
// Bluespec libraries

import RegFile :: *;
import Vector  :: *;
import FIFOF   :: *;

// ----------------
// BSV additional libs

import Cur_Cycle  :: *;

// ================================================================
// Project imports

import ISA_Decls :: *;

// ================================================================

export TLB_Lookup_Result (..), TLB_IFC (..), mkTLB, fav_vm_xlate;
export VM_Xlate_Outcome (..), VM_Xlate_Result (..);

// ================================================================
// Abbreviations (from ISA spec)
//    ASID:   Address space identifier
//    VPN:    Virtual Page Number
//    PPN:    Physical Page Number
//    VA:     Virtual Address
//    PA:     Physical Address    (= { PPN, 12'b_Offset }
//    PTE:    Page Table Entry

// Abbreviations (other)
//    TLB:    Translation Lookaside Buffer (a cache mapping {ASID,VPN}->PPN)

// ================================================================
// TLB interface

typedef struct {
   Bool      hit;
   PTE       pte;            // The leaf PTE for this translation (contains PPN)

   Bit #(2)  pte_level;      // Level of leaf PTE for this translation
   PA        pte_pa;         // PA to which this PTE is written back if A,D bits are updated
   } TLB_Lookup_Result
deriving (Bits, FShow);

interface TLB_IFC;
   method Action flush;    // Invalidates all entries

   method TLB_Lookup_Result  lookup (ASID asid, VPN vpn);

   method Action insert (ASID asid, VPN vpn, PTE pte, Bit #(2) level, PA pte_pa);
endinterface

// ================================================================
// Implementation notes:

// RV32.sv32 page tables are 2-level trees (levels are called 1,0).
// A leaf can occur at any level ('megapages', 'pages').

// RV64.sv39 page tables are 3-level trees (levels are called 2,1,0).
// A leaf can occur at any level ('gigapages', 'megapages', 'pages').

// For maximum hashing (least collisions), the TLB cache index should
// be taken from the least-significant bits of the VPN.
// The remaining VPN bits form the cache tag.
//                   Tag bits            Index bits
// For gigapages:    in VPN [2]          in VPN [2]
// For megapages:    in VPN [2,1]        in VPN [1]
// For pages:        in VPN [2,1,0]      in VPN [0]

// When we do a TLB lookup for a VA, we don't know whether it'll be in
// a page, megapage or gigapage, or is unmapped.

// Thus our "TLB" is actually multiple sub-TLBs, one each for ordinary,
// mega and giga pages.

// These are probed concurrently (at most one should HIT).
// Note: we can choose different params for each sub-TLB
//    (size, associativity)

// ================================================================
// TLB parameters and help-functions
// These TLBs are direct-mapped caches.
// The index is a few bits from VPN [level]
// The tag is (asid, remaining bits from VPN)

// ----------------
// Level 2 tags and indexes (for RV64 only)

typedef  4                     TLB2_Size;    // # of entries in TLB2
typedef  TLog #(TLB2_Size)     TLB2_Index_sz;
typedef  Bit #(TLB2_Index_sz)  TLB2_Index;
Integer  tlb2_index_sz = valueOf (TLB2_Index_sz);

typedef  TSub #(VPN_J_sz, TLB2_Index_sz)  TLB2_Tag_sz;
typedef  Bit #(TLB2_Tag_sz)  TLB2_Tag;
Integer  tlb2_tag_sz = valueOf (TLB2_Tag_sz);

// ----------------
// Level 1 tags and indexes

typedef  8                     TLB1_Size;    // # of entries in TLB1
typedef  TLog #(TLB1_Size)     TLB1_Index_sz;
typedef  Bit #(TLB1_Index_sz)  TLB1_Index;
Integer  tlb1_index_sz = valueOf (TLB1_Index_sz);

typedef  TSub #(TMul #(2, VPN_J_sz), TLB1_Index_sz)  TLB1_Tag_sz;
typedef  Bit #(TLB1_Tag_sz)  TLB1_Tag;
Integer  tlb1_tag_sz = valueOf (TLB1_Tag_sz);

// ----------------
// Level 0 tags and indexes

typedef  16                    TLB0_Size;    // # of entries in TLB0
typedef  TLog #(TLB0_Size)     TLB0_Index_sz;
typedef  Bit #(TLB0_Index_sz)  TLB0_Index;
Integer  tlb0_index_sz = valueOf (TLB0_Index_sz);

typedef  TSub #(TMul #(3, VPN_J_sz), TLB0_Index_sz)  TLB0_Tag_sz;
typedef  Bit #(TLB0_Tag_sz)  TLB0_Tag;
Integer  tlb0_tag_sz = valueOf (TLB0_Tag_sz);

// ----------------
// Each of the 3 sub-TLBs contains TLBEs (TLB Entries)
// Each TLBE is a PTE + additional info (tag, pte_pa)
// We keep the whole PTE even though PTEs have 'reserved' fields
// which we don't use, to avoid disturbing those fields on
// PTE write-back, both because software may use those fields
// and to preserve tandem-verification.

typedef struct {
   ASID           asid_tag;   // Address-space tag
   Bit #(tag_sz)  vpn_tag;    // VPN tag (Tag_sz MSBs of VPN)
   PTE            pte;        // Contains PPN + control bits
   PA             pte_pa;     // For future writes-back of this PTE
   } TLBE #(numeric type tag_sz)
deriving (Bits, FShow);

// ================================================================
// TLB module

(* synthesize *)
module mkTLB #(parameter Bool dmem_not_imem) (TLB_IFC);

   String i_or_d = (dmem_not_imem ? "D" : "I");

   Integer verbosity = 0;    // For debugging only

   Reg #(Bool) rg_flushing <- mkReg (True);

   // The TLBs below use Vector-of-Reg for 'valids', allowing 1-cycle flushing
   // They use RegFiles for 'entries', which should map to LUTRAMs.
   // (Should we change them to BRAMs?  Would take a 1-cycle hit)

   // ----------------
   // Level 2 TLB (for gigapages)

`ifdef RV64
   Vector  #(TLB2_Size,  Reg #(Bool))         tlb2_valids  <- replicateM (mkRegU);
   RegFile #(TLB2_Index, TLBE #(TLB2_Tag_sz)) tlb2_entries <- mkRegFileFull;
`endif

   // ----------------
   // Level 1 TLB (for megapages)

   Vector  #(TLB1_Size,   Reg #(Bool))        tlb1_valids  <- replicateM (mkRegU);
   RegFile #(TLB1_Index, TLBE #(TLB1_Tag_sz)) tlb1_entries <- mkRegFileFull;

   // ----------------
   // Level 0 TLB (for pages)

   Vector  #(TLB0_Size,   Reg #(Bool))        tlb0_valids  <- replicateM (mkRegU);
   RegFile #(TLB0_Index, TLBE #(TLB0_Tag_sz)) tlb0_entries <- mkRegFileFull;

   // ----------------------------------------------------------------
   // Lookup functions for each sub-page
   // In each case 2-tuple results is (HIT/MISS, index into TLB array)

   // Note: these are not straightforward to combine into a single
   // polymorphic function because of the different types for tags,
   // indexes and tlb_entries.

`ifdef RV64
   function Tuple2 #(Bool, TLB2_Index) fn_lookup2 (ASID asid, VPN vpn);
      Integer index_lo = 2 * vpn_j_sz;
      Integer index_hi = (2 * vpn_j_sz) + tlb2_index_sz - 1;
      Integer tag_lo   = (2 * vpn_j_sz) + tlb2_index_sz;
      Integer tag_hi   = vpn_sz - 1;

      TLB2_Index idx   = vpn [index_hi : index_lo];
      TLB2_Tag   tag   = vpn [  tag_hi :   tag_lo];

      let  tlbe           = tlb2_entries.sub (idx);
      Bool global_mapping = (tlbe.pte [pte_G_offset] == 1'b1);

      Bool match2 = (   (tlb2_valids [idx])
		     && ((tlbe.asid_tag == asid) || global_mapping)
		     && (tlbe.vpn_tag  == tag));
      return tuple2 (match2, idx);
   endfunction
`endif

   function Tuple2 #(Bool, TLB1_Index) fn_lookup1 (ASID asid, VPN vpn);
      Integer index_lo = vpn_j_sz;
      Integer index_hi = vpn_j_sz + tlb1_index_sz - 1;
      Integer tag_lo   = vpn_j_sz + tlb1_index_sz;
      Integer tag_hi   = vpn_sz - 1;

      TLB1_Index idx   = vpn [index_hi : index_lo];
      TLB1_Tag   tag   = vpn [  tag_hi : tag_lo];

      let  tlbe           = tlb1_entries.sub (idx);
      Bool global_mapping = (tlbe.pte [pte_G_offset] == 1'b1);

      Bool match1 = (   (tlb1_valids [idx])
		     && ((tlbe.asid_tag == asid) || global_mapping)
		     && (tlbe.vpn_tag  == tag));
      return tuple2 (match1, idx);
   endfunction

   function Tuple2 #(Bool, TLB0_Index) fn_lookup0 (ASID asid, VPN vpn);
      Integer index_lo = 0;
      Integer index_hi = tlb0_index_sz - 1;
      Integer tag_lo   = tlb0_index_sz;
      Integer tag_hi   = vpn_sz - 1;

      TLB0_Index idx   = vpn [index_hi : index_lo];
      TLB0_Tag   tag   = vpn [  tag_hi :   tag_lo];

      let  tlbe           = tlb0_entries.sub (idx);
      Bool global_mapping = (tlbe.pte [pte_G_offset] == 1'b1);

      Bool match0 = (   (tlb0_valids [idx])
		     && ((tlbe.asid_tag == asid) || global_mapping)
		     && (tlbe.vpn_tag  == tag));
      return tuple2 (match0, idx);
   endfunction

   // ----------------------------------------------------------------
   // BEHAVIOR

   rule rl_initialize (rg_flushing);
      // Invalidate all tlb entries
`ifdef RV64
      writeVReg (tlb2_valids, replicate (False));
`endif
      writeVReg (tlb1_valids, replicate (False));
      writeVReg (tlb0_valids, replicate (False));

      rg_flushing <= False;

      if (verbosity > 1)
	 $display ("%0d: %sTLB: rl_initialize: TLBs flushed", cur_cycle, i_or_d);
   endrule

   // ----------------------------------------------------------------
   // INTERFACE

   method Action flush;
      rg_flushing <= True;
   endmethod

   // ----------------
   // Look for a matching entry for a given va in the TLBs

   method TLB_Lookup_Result  lookup (ASID asid, VPN vpn)  if (! rg_flushing);
      match { .match0, .idx0 } = fn_lookup0 (asid, vpn);
      match { .match1, .idx1 } = fn_lookup1 (asid, vpn);
`ifdef RV64
      match { .match2, .idx2 } = fn_lookup2 (asid, vpn);
`else
      let match2 = False;
`endif

      TLB_Lookup_Result  result0 = unpack (0);
      TLB_Lookup_Result  result1 = unpack (0);
      TLB_Lookup_Result  result2 = unpack (0);

      if (match0) begin
	 let tlbe0 = tlb0_entries.sub (idx0);
	 result0 = TLB_Lookup_Result {hit: True, pte: tlbe0.pte, pte_level: 0, pte_pa: tlbe0.pte_pa};
      end

      if (match1) begin
	 let tlbe1 = tlb1_entries.sub (idx1);
	 result1 = TLB_Lookup_Result {hit: True, pte: tlbe1.pte, pte_level: 1, pte_pa: tlbe1.pte_pa};
      end

`ifdef RV64
      if (match2) begin
	 let tlbe2 = tlb2_entries.sub (idx2);
	 result2 = TLB_Lookup_Result {hit: True, pte: tlbe2.pte, pte_level: 2, pte_pa: tlbe2.pte_pa};
      end
`endif
      let result = (pack (result0) | pack (result1) | pack (result2));
      return unpack (result);
   endmethod

   // ----------------
   // Insert a PTE into the TLB

   method Action insert (ASID asid, VPN vpn, PTE pte, Bit #(2) level, PA pte_pa)  if (! rg_flushing);
      if (verbosity > 1)
	 $display ("%0d: %sTLB: insert: asid 0x%0h  vpn 0x%0h  pa 0x%0h  level %0d  pte 0x%0h",
		   cur_cycle, i_or_d, asid, vpn, pte, level, pte_pa);

      if (level == 0) begin
	 TLB0_Tag            tag  = vpn [(vpn_sz - 1) : tlb0_index_sz + (0 * vpn_j_sz)];
	 TLB0_Index          idx  = vpn [(tlb0_index_sz + (0 * vpn_j_sz) - 1) : (0 * vpn_j_sz)];
	 TLBE #(TLB0_Tag_sz) tlbe = TLBE {asid_tag: asid, vpn_tag: tag, pte: pte, pte_pa: pte_pa};

	 tlb0_valids [idx] <= True;
	 tlb0_entries.upd (idx, tlbe);
      end
      else if (level == 1) begin
	 TLB1_Tag            tag  = vpn [(vpn_sz - 1) : tlb1_index_sz + (1 * vpn_j_sz)];
	 TLB1_Index          idx  = vpn [(tlb1_index_sz + (1 * vpn_j_sz) - 1) : (1 * vpn_j_sz)];
	 TLBE #(TLB1_Tag_sz) tlbe = TLBE {asid_tag: asid, vpn_tag: tag, pte: pte, pte_pa: pte_pa};

	 tlb1_valids [idx] <= True;
	 tlb1_entries.upd (idx, tlbe);
      end
`ifdef RV64
      else begin // (level == 2)
	 TLB2_Tag            tag  = vpn [(vpn_sz - 1) : tlb2_index_sz + (2 * vpn_j_sz)];
	 TLB2_Index          idx  = vpn [(tlb2_index_sz + (2 * vpn_j_sz) - 1) : (2 * vpn_j_sz)];
	 TLBE #(TLB2_Tag_sz) tlbe = TLBE {asid_tag: asid, vpn_tag: tag, pte: pte, pte_pa: pte_pa};

	 tlb2_valids [idx] <= True;
	 tlb2_entries.upd (idx, tlbe);
      end
`endif
   endmethod

endmodule

// ================================================================
// This function performs a VM translation and checks PTE permissions if necessary

typedef enum { VM_XLATE_OK, VM_XLATE_TLB_MISS, VM_XLATE_EXCEPTION } VM_Xlate_Outcome
deriving (Bits, Eq, FShow);

typedef struct {
   VM_Xlate_Outcome   outcome;
   PA                 pa;            // phys addr, if VM_XLATE_OK
   Exc_Code           exc_code;      // if VM_XLATE_EXC
   Bool               pte_modified;  // if VM_XLATE_OK and pte's A or D bits were modified
   PTE                pte;           // updated PTE, if pte_modified
   } VM_Xlate_Result
deriving (Bits, FShow);

function ActionValue #(VM_Xlate_Result)  fav_vm_xlate (WordXL             addr,
						       WordXL             satp,
						       TLB_Lookup_Result  tlb_result,
						       Bool               dmem_not_imem,
						       Bool               read_not_write,
						       Priv_Mode          priv,
						       Bit #(1)           sstatus_SUM,
						       Bit #(1)           mstatus_MXR);
   actionvalue
      // Translate if in VM mode (sv32, sv39), and priv <= s_Priv_Mode
      // Default PA (no translation) = addr

`ifdef RV32
      Bool xlate = ((priv <= s_Priv_Mode) && (fn_satp_to_VM_Mode (satp) == satp_mode_RV32_sv32));
      PA   pa    = zeroExtend (addr);
`elsif SV39
      Bool xlate = ((priv <= s_Priv_Mode) && (fn_satp_to_VM_Mode (satp) == satp_mode_RV64_sv39));
      PA   pa    = truncate (addr);
`endif

      VM_Xlate_Outcome   outcome      = VM_XLATE_OK;
      Exc_Code           exc_code     = ?;
      Bool               pte_modified = False;
      PTE                pte          = tlb_result.pte;

      if (xlate) begin
	 if (tlb_result.hit) begin
	    Bool deny = is_pte_denial (dmem_not_imem, read_not_write, priv, sstatus_SUM, mstatus_MXR, pte);
	    if (deny) begin
	       outcome = VM_XLATE_EXCEPTION;
	       exc_code = fn_page_fault_exc_code (dmem_not_imem, read_not_write);
	       // $display ("fav_vm_xlate: page fault due to pte_denial");
	    end

	    else if (is_pte_A_D_fault (read_not_write, pte)) begin
	       outcome = VM_XLATE_EXCEPTION;
	       exc_code = fn_page_fault_exc_code (dmem_not_imem, read_not_write);
	       // $display ("fav_vm_xlate: page fault due to A/D fault");
	    end

	    else begin
	       if (tlb_result.pte_level == 0)
		  pa = zeroExtend ({fn_PTE_to_PPN (pte),
				    fn_Addr_to_Offset (addr) });

	       else if (tlb_result.pte_level == 1)
		  pa = zeroExtend ({fn_PTE_to_PPN_mega (pte),
				    fn_Addr_to_VPN_0 (addr),
				    fn_Addr_to_Offset (addr) });
`ifdef SV39
	       else if (tlb_result.pte_level == 2)
		  pa = zeroExtend ({fn_PTE_to_PPN_giga (pte),
				    fn_Addr_to_VPN_1 (addr),
				    fn_Addr_to_VPN_0 (addr),
				    fn_Addr_to_Offset (addr) });
`endif

	       // $display ("    fav_vm_xlate: PTE.A = %0d", fn_PTE_to_A (pte));
	       if (fn_PTE_to_A (pte) == 1'b0) begin
		  pte_modified = True;
		  WordXL tmp = 1;
		  pte = (pte | (tmp << pte_A_offset));
	       end

	       // $display ("    fav_vm_xlate: PTE.D = %0d  read = %0d", fn_PTE_to_D (pte), pack (read_not_write));
	       if ((fn_PTE_to_D (pte) == 1'b0) && (! read_not_write)) begin
		  pte_modified = True;
		  WordXL tmp = 1;
		  pte = (pte | (tmp << pte_D_offset));
	       end
	    end
	 end
	 else
	    outcome = VM_XLATE_TLB_MISS;
      end
      return VM_Xlate_Result {outcome:      outcome,
			      pa:           pa,
			      exc_code:     exc_code,
			      pte_modified: pte_modified,
			      pte:          pte};
   endactionvalue
endfunction: fav_vm_xlate

// ================================================================

endpackage

// Copyright (c) 2016-2019 Bluespec, Inc. All Rights Reserved

package PPR_RegFile;

// ================================================================
// PPR (Floating Point Register) Register File

// ================================================================
// Exports

export PPR_RegFile_IFC (..), mkPPR_RegFile;

// ================================================================
// BSV library imports

import ConfigReg    :: *;
import RegFile      :: *;
import FIFOF        :: *;
import GetPut       :: *;
import ClientServer :: *;

// BSV additional libs

import GetPut_Aux :: *;

// ================================================================
// Project imports

import ISA_Decls :: *;

// ================================================================

interface PPR_RegFile_IFC;
   // Reset
   interface Server #(Token, Token) server_reset;

   // PPR read
   (* always_ready *)
   method WordPL read_rs1 (RegName rs1);
   (* always_ready *)
   method WordPL read_rs2 (RegName rs2);

   // PPR write
   (* always_ready *)
   method Action write_rd (RegName rd, WordPL rd_val);

endinterface

// ================================================================
// Major states of mkPPR_RegFile module

typedef enum { RF_RESET_START, RF_RESETTING, RF_RUNNING } RF_State
deriving (Eq, Bits, FShow);

// ================================================================

(* synthesize *)
module mkPPR_RegFile (PPR_RegFile_IFC);

   Reg #(RF_State) rg_state    <- mkReg (RF_RESET_START);

   // Reset
   FIFOF #(Token) f_reset_rsps <- mkFIFOF;

   // Posit Point Registers
   // Unlike GPRs, all registers in the PPR are regular registers (no r0)
   RegFile #(RegName, WordPL) regfile <- mkRegFileFull;

   // ----------------------------------------------------------------
   // Reset.
   // This loop initializes all PPRs to 0.
   // The spec does not require this, but it's useful for debugging
   // and tandem verification

   Reg #(RegName) rg_j <- mkRegU;    // reset loop index

   rule rl_reset_start (rg_state == RF_RESET_START);
      rg_state <= RF_RESETTING;
      rg_j <= 1;
   endrule

   rule rl_reset_loop (rg_state == RF_RESETTING);
      regfile.upd (rg_j, 0);
      rg_j <= rg_j + 1;
      if (rg_j == 31)
         rg_state <= RF_RUNNING;
   endrule

   // ----------------------------------------------------------------
   // INTERFACE

   // Reset
   interface Server server_reset;
      interface Put request;
         method Action put (Token token);
            rg_state <= RF_RESET_START;

            // This response is placed here, and not in rl_reset_loop, because
            // reset_loop can happen on power-up, where no response is expected.
            f_reset_rsps.enq (?);
         endmethod
      endinterface
      interface Get response;
         method ActionValue #(Token) get if (rg_state == RF_RUNNING);
            let token <- pop (f_reset_rsps);
            return token;
         endmethod
      endinterface
   endinterface

   // PPR read
   method WordPL read_rs1 (RegName rs1);
      return (regfile.sub (rs1));
   endmethod

   method WordPL read_rs2 (RegName rs2);
      return (regfile.sub (rs2));
   endmethod

   // PPR write
   method Action write_rd (RegName rd, WordPL rd_val);
      regfile.upd (rd, rd_val);
   endmethod

endmodule

// ================================================================

endpackage

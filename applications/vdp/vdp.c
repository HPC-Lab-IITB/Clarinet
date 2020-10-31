#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "clarinet.h"

#define VSZ 16

/* Define the REP macro to indicate number of repetitions to simulate
 * longer vectors:
 * REP =  4  (  64)
 * REP =  8  ( 128)
 * REP = 16  ( 256)
 * REP = 32  ( 512)
 * REP = 64  (1024)
 * REP =128  (2048)
 * REP =256  (4096)
 */


static float v_a [VSZ]        = {3.4050474167, 3.1847724915, 7.3107628822, -5.6170530319, -4.7297372818, 4.5974335670, 6.0904574394, -3.0683014393, 4.1741681099, -3.3324038982, -7.1214756966, 2.7567198277, -3.4179165363, -9.2278394699, 8.9482812881, -0.3960950971};
static float v_b [VSZ]        = {3.4050474167, 3.1847724915, 7.3107628822, -5.6170530319, -4.7297372818, 4.5974335670, 6.0904574394, -3.0683014393, 4.1741681099, -3.3324038982, -7.1214756966, 2.7567198277, -3.4179165363, -9.2278394699, 8.9482812881, -0.3960950971};

#ifdef PWIDTH_8
static unsigned int p_a [VSZ] = { 0x00000087, 0x0000008f, 0x0000008c, 0x00000069, 0x00000073, 0x00000078, 0x0000008e, 0x000000b1, 0x000000b2, 0x00000062, 0x00000087, 0x00000012, 0x0000009e, 0x00000076, 0x0000009e, 0x000000ef };
static unsigned int p_b [VSZ] = { 0x00000087, 0x0000008f, 0x0000008c, 0x00000069, 0x00000073, 0x00000078, 0x0000008e, 0x000000b1, 0x000000b2, 0x00000062, 0x00000087, 0x00000012, 0x0000009e, 0x00000076, 0x0000009e, 0x000000ef };
#endif
#ifdef PWIDTH_16
static unsigned int p_a [VSZ] = { 0x000096ac, 0x00009f09, 0x00009c02, 0x00005937, 0x000062b1, 0x00006880, 0x00009e74, 0x0000b8ae, 0x0000b916, 0x000051f4, 0x000096c3, 0x00002262, 0x0000adce, 0x00006676, 0x0000adcd, 0x0000dec6 };
static unsigned int p_b [VSZ] = { 0x000096ac, 0x00009f09, 0x00009c02, 0x00005937, 0x000062b1, 0x00006880, 0x00009e74, 0x0000b8ae, 0x0000b916, 0x000051f4, 0x000096c3, 0x00002262, 0x0000adce, 0x00006676, 0x0000adcd, 0x0000dec6 };
#endif
#ifdef PWIDTH_24
static unsigned int p_a [VSZ] = { 0xa6ac1500, 0xaf08ad00, 0xac018400, 0x4c9b9500, 0x52b13500, 0x58803d00, 0xae745400, 0xbc56fb00, 0xbc8b1b00, 0x48fa0500, 0xa6c30000, 0x31314000, 0xb6e6d000, 0x5675aa00, 0xb6e64d00, 0xcf62f600 };
static unsigned int p_b [VSZ] = { 0xa6ac1500, 0xaf08ad00, 0xac018400, 0x4c9b9500, 0x52b13500, 0x58803d00, 0xae745400, 0xbc56fb00, 0xbc8b1b00, 0x48fa0500, 0xa6c30000, 0x31314000, 0xb6e6d000, 0x5675aa00, 0xb6e64d00, 0xcf62f600 };
#endif
#ifdef PWIDTH_32
static unsigned int p_a [VSZ] = { 0xa6ac1570, 0xaf08ad60, 0xac0183f0, 0x4c9b9540, 0x52b13500, 0x58803d70, 0xae745430, 0xbc56fb50, 0xbc8b1aa0, 0x48fa0540, 0xa6c2ffa0, 0x31313ff0, 0xb6e6cfe0, 0x5675aa10, 0xb6e64d30, 0xcf62f5a0 };
static unsigned int p_b [VSZ] = { 0xa6ac1570, 0xaf08ad60, 0xac0183f0, 0x4c9b9540, 0x52b13500, 0x58803d70, 0xae745430, 0xbc56fb50, 0xbc8b1aa0, 0x48fa0540, 0xa6c2ffa0, 0x31313ff0, 0xb6e6cfe0, 0x5675aa10, 0xb6e64d30, 0xcf62f5a0 };
#endif

float fn_posit_p_vdp (int r) {
   unsigned int acc = 0;
   int idx = 0;
   int rep = 0;
   fn_init_p_quire (acc);

   for (rep=0; rep < r; rep ++)
      for (idx=0; idx<24; idx++) fn_posit_p_fma (p_a[idx], p_b[idx]);

   acc = fn_read_p_quire ();
   return (acc);
}

float fn_posit_vdp (int r) {
   float acc = 0.0;
   int idx = 0;
   int rep = 0;
   fn_init_quire (acc);
   for (rep=0; rep < r; rep ++)
      for (idx=0; idx<VSZ; idx++) fn_posit_fma (v_a[idx], v_b[idx]);
   acc = fn_read_quire ();
   return (acc);
}

float fn_float_vdp (int r) {
   float acc = 0.0;
   int idx = 0;
   int rep = 0;

   for (rep=0; rep < r; rep ++)
      for (idx=0; idx<VSZ; idx++) acc += fn_float_fma (v_a[idx], v_b[idx], acc);

   return (acc);
}

float fn_float_optimized_vdp (int r) {
   float acc = 0.0;
   int idx = 0;
   int rep = 0;

   for (rep=0; rep < r; rep ++)
      for (idx=0; idx<VSZ; idx++) acc += (v_a[idx] * v_b[idx]);

   return (acc);
}


int main (void) {
   float acc1, acc2;
   unsigned int acc3;
   uint64_t start = 0;
   uint64_t end = 0;

   printf ("Starting VDP benchmarks...\n");

   // Basic VDPs are working
   // Now lets do some cycle analysis
#ifdef OPT_FLOAT
   start=read_cycle();
   acc1 = fn_float_optimized_vdp (REP);
   end=read_cycle();
   printf("Compile Optimized Float VDP: %d cycles have elapsed.\n",(end-start));
#endif

#ifdef FLOAT
   start=read_cycle();
   acc1 = fn_float_vdp (REP);
   end=read_cycle();
   printf("Unoptimized Float VDP: %d cycles have elapsed.\n",(end-start));
#endif

#ifdef FLOAT_POSIT
   start=read_cycle();
   acc2 = fn_posit_vdp (REP);
   end=read_cycle();
   printf("Float VDP With Posits: %d cycles have elapsed.\n",(end-start));
#endif

#ifdef POSIT
   start=read_cycle();
   acc3 = fn_posit_p_vdp (REP);
   end=read_cycle();
   printf("Native Posit VDP: %d cycles have elapsed.\n",(end-start));
#endif

   return (0);
}

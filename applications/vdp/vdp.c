#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "clarinet.h"

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


#ifndef POSIT
static float v_a [VSZ]        = {3.4050474167, 3.1847724915, 7.3107628822, -5.6170530319, -4.7297372818, 4.5974335670, 6.0904574394, -3.0683014393, 4.1741681099, -3.3324038982, -7.1214756966, 2.7567198277, -3.4179165363, -9.2278394699, 8.9482812881, -0.3960950971};
static float v_b [VSZ]        = {3.4050474167, 3.1847724915, 7.3107628822, -5.6170530319, -4.7297372818, 4.5974335670, 6.0904574394, -3.0683014393, 4.1741681099, -3.3324038982, -7.1214756966, 2.7567198277, -3.4179165363, -9.2278394699, 8.9482812881, -0.3960950971};
#endif

#ifdef POSIT
#ifdef PWIDTH_8
static unsigned char p_a [VSZ] = { 0x87, 0x8f, 0x8c, 0x69, 0x73, 0x78, 0x8e, 0xb1, 0xb2, 0x62, 0x87, 0x12, 0x9e, 0x76, 0x9e, 0xef };
static unsigned char p_b [VSZ] = { 0x87, 0x8f, 0x8c, 0x69, 0x73, 0x78, 0x8e, 0xb1, 0xb2, 0x62, 0x87, 0x12, 0x9e, 0x76, 0x9e, 0xef };
#endif
#ifdef PWIDTH_16
static unsigned short p_a [VSZ] = { 0x96ac, 0x9f09, 0x9c02, 0x5937, 0x62b1, 0x6880, 0x9e74, 0xb8ae, 0xb916, 0x51f4, 0x96c3, 0x2262, 0xadce, 0x6676, 0xadcd, 0xdec6 };
static unsigned short p_b [VSZ] = { 0x96ac, 0x9f09, 0x9c02, 0x5937, 0x62b1, 0x6880, 0x9e74, 0xb8ae, 0xb916, 0x51f4, 0x96c3, 0x2262, 0xadce, 0x6676, 0xadcd, 0xdec6 };
#endif
#ifdef PWIDTH_24
static unsigned int p_a [VSZ] = { 0xa6ac1500, 0xaf08ad00, 0xac018400, 0x4c9b9500, 0x52b13500, 0x58803d00, 0xae745400, 0xbc56fb00, 0xbc8b1b00, 0x48fa0500, 0xa6c30000, 0x31314000, 0xb6e6d000, 0x5675aa00, 0xb6e64d00, 0xcf62f600 };
static unsigned int p_b [VSZ] = { 0xa6ac1500, 0xaf08ad00, 0xac018400, 0x4c9b9500, 0x52b13500, 0x58803d00, 0xae745400, 0xbc56fb00, 0xbc8b1b00, 0x48fa0500, 0xa6c30000, 0x31314000, 0xb6e6d000, 0x5675aa00, 0xb6e64d00, 0xcf62f600 };
#endif
#ifdef PWIDTH_32
static unsigned int p_a [VSZ] = { 0xa6ac1570, 0xaf08ad60, 0xac0183f0, 0x4c9b9540, 0x52b13500, 0x58803d70, 0xae745430, 0xbc56fb50, 0xbc8b1aa0, 0x48fa0540, 0xa6c2ffa0, 0x31313ff0, 0xb6e6cfe0, 0x5675aa10, 0xb6e64d30, 0xcf62f5a0 };
static unsigned int p_b [VSZ] = { 0xa6ac1570, 0xaf08ad60, 0xac0183f0, 0x4c9b9540, 0x52b13500, 0x58803d70, 0xae745430, 0xbc56fb50, 0xbc8b1aa0, 0x48fa0540, 0xa6c2ffa0, 0x31313ff0, 0xb6e6cfe0, 0x5675aa10, 0xb6e64d30, 0xcf62f5a0 };
#endif
#endif

int main (void) {
   float acc1, acc2;

#ifdef POSIT
#ifdef PWIDTH_8
   unsigned char acc3;
#endif
#ifdef PWIDTH_16
   unsigned short acc3;
#endif
#ifdef PWIDTH_24
   unsigned int acc3;
#endif
#ifdef PWIDTH_32
   unsigned int acc3;
#endif
#endif

   uint64_t start = 0;
   uint64_t end = 0;

   printf ("Starting VDP benchmarks...\n");

   // Basic VDPs are working
   // Now lets do some cycle analysis
#ifdef OPT_FLOAT
   start=read_cycle();
   acc1 = fn_float_optimized_vdp (REP, v_a, v_b);
   end=read_cycle();
   printf("Compile Optimized Float VDP: %d cycles have elapsed.\n",(end-start));
#endif

#ifdef FLOAT
   start=read_cycle();
   acc1 = fn_float_vdp (REP, v_a, v_b);
   end=read_cycle();
   printf("Unoptimized Float VDP: %d cycles have elapsed.\n",(end-start));
#endif

#ifdef FLOAT_POSIT
   start=read_cycle();
   acc2 = fn_posit_vdp (REP, v_a, v_b);
   end=read_cycle();
   printf("Float VDP With Posits: %d cycles have elapsed.\n",(end-start));
#endif

#ifdef POSIT
   start=read_cycle();
   acc3 = fn_posit_p_vdp (REP, p_a, p_b);
   end=read_cycle();
   printf("Native Posit VDP: %d cycles have elapsed.\n",(end-start));
#endif

   return (0);
}

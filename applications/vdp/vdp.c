#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "clarinet.h"

#define VSZ 24

static float v_a [VSZ]        = {-7.8550171852, -8.9708328247, -2.3183071613, -8.6166706085, 3.0767068863, -7.8940663338, 6.5208415985, 2.5259063244, 3.4050474167, 3.1847724915, 7.3107628822, -5.6170530319, -4.7297372818, 4.5974335670, 6.0904574394, -3.0683014393, 4.1741681099, -3.3324038982, -7.1214756966, 2.7567198277, -3.4179165363, -9.2278394699, 8.9482812881, -0.3960950971};
static float v_b [VSZ]        = {-7.8550171852, -8.9708328247, -2.3183071613, -8.6166706085, 3.0767068863, -7.8940663338, 6.5208415985, 2.5259063244, 3.4050474167, 3.1847724915, 7.3107628822, -5.6170530319, -4.7297372818, 4.5974335670, 6.0904574394, -3.0683014393, 4.1741681099, -3.3324038982, -7.1214756966, 2.7567198277, -3.4179165363, -9.2278394699, 8.9482812881, -0.3960950971};
static unsigned int p_a [VSZ] = {0x4400bf40, 0x5600c140, 0x4f7df750, 0x522a4120, 0xb9ffa7e0, 0xafb115e0, 0x5889f220, 0xb20a6420, 0x48be1910, 0xaed8eee0, 0xc75db8a0, 0xaa549e70, 0x5579b790, 0x5946d260, 0x57ff6560, 0xb7c7ce80, 0xa699d5f0, 0x56e2b790, 0x54b0c870, 0xaf905880, 0xa6c3cce0, 0x590f6a50, 0xa8e4ab00, 0xb5d2b4c0};
static unsigned int p_b [VSZ] = {0x4400bf40, 0x5600c140, 0x4f7df750, 0x522a4120, 0xb9ffa7e0, 0xafb115e0, 0x5889f220, 0xb20a6420, 0x48be1910, 0xaed8eee0, 0xc75db8a0, 0xaa549e70, 0x5579b790, 0x5946d260, 0x57ff6560, 0xb7c7ce80, 0xa699d5f0, 0x56e2b790, 0x54b0c870, 0xaf905880, 0xa6c3cce0, 0x590f6a50, 0xa8e4ab00, 0xb5d2b4c0};


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
   start=read_cycle();
   acc1 = fn_float_optimized_vdp (100);
   end=read_cycle();
   printf("Compile Optimized Float VDP: %d cycles have elapsed.\n",(end-start));

   start=read_cycle();
   acc1 = fn_float_vdp (100);
   end=read_cycle();
   printf("Unoptimized Float VDP: %d cycles have elapsed.\n",(end-start));

   start=read_cycle();
   acc2 = fn_posit_vdp (100);
   end=read_cycle();
   printf("Float VDP With Posits: %d cycles have elapsed.\n",(end-start));

   start=read_cycle();
   acc3 = fn_posit_p_vdp (100);
   end=read_cycle();
   printf("Native Posit VDP: %d cycles have elapsed.\n",(end-start));

   return (0);
}

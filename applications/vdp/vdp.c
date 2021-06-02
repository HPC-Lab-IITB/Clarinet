#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "clarinet.h"
#include "data_vdp.h"

int main (void) {

#ifdef POSIT
#ifdef PWIDTH_8
   unsigned char acc;
#endif
#ifdef PWIDTH_16
   unsigned short acc;
#endif
#ifdef PWIDTH_24
   unsigned int acc;
#endif
#ifdef PWIDTH_32
   unsigned int acc;
#endif
#endif

   uint32_t start = 0;
   uint32_t end = 0;
   uint32_t elapsed = 0;

   // Basic VDPs are working
   // Now lets do some cycle analysis
#ifdef FLOAT
   float acc;
#ifdef WARM_CACHES
   acc = fn_float_vdp (VSZ, v_a, v_b);
#endif

   start=read_cycle();
   acc = fn_float_vdp (VSZ, v_a, v_b);
   end=read_cycle();
#endif

#ifdef FLOAT_POSIT
   float acc;
#ifdef WARM_CACHES   // run once to warm up caches
   acc = fn_posit_vdp (VSZ, v_a, v_b);
#endif
   start=read_cycle();
   acc = fn_posit_vdp (VSZ, v_a, v_b);
   end=read_cycle();
#endif

#ifdef DOUBLE
   double acc;
#ifdef WARM_CACHES   // run once to warm up caches
   acc = fn_double_vdp (VSZ, v_a, v_b);
#endif

   start=read_cycle();
   acc = fn_double_vdp (VSZ, v_a, v_b);
   end=read_cycle();
#endif

#ifdef POSIT
#ifdef WARM_CACHES
   // run once to warm up caches
#ifdef PWIDTH_24
   acc = fn_posit_p_vdp (VSZ, v_aH, v_a, v_bH, v_b);
#else
   acc = fn_posit_p_vdp (VSZ, v_a, v_b);
#endif
#endif
   start=read_cycle();
#ifdef PWIDTH_24
   acc = fn_posit_p_vdp (VSZ, v_aH, v_a, v_bH, v_b);
#else
   acc = fn_posit_p_vdp (VSZ, v_a, v_b);
#endif
   end=read_cycle();
#endif

   elapsed = end - start;
#ifdef WARM_CACHES
   printf ("VDP Cycle Report (warm caches). Size: %d. Cycles: %d\n", VSZ, elapsed);
#else
   printf ("VDP Cycle Report. Size: %d. Cycles: %d\n", VSZ, elapsed);
#endif

   return (0);
}

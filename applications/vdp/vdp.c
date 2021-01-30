#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "clarinet.h"
#include "data.h"

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
#ifdef OPT_FLOAT
   float acc;
   start=read_cycle();
   acc = fn_float_optimized_vdp (VSZ, v_a, v_b);
   end=read_cycle();
#endif

#ifdef FLOAT
   float acc;
   start=read_cycle();
   acc = fn_float_vdp (VSZ, v_a, v_b);
   end=read_cycle();
#endif

#ifdef FLOAT_POSIT
   float acc;
   start=read_cycle();
   acc = fn_posit_vdp (VSZ, v_a, v_b);
   end=read_cycle();
#endif

#ifdef DOUBLE
   double acc;
   start=read_cycle();
   acc = fn_double_vdp (VSZ, v_a, v_b);
   end=read_cycle();
#endif

#ifdef POSIT
   start=read_cycle();
   acc = fn_posit_p_vdp (VSZ, v_a, v_b);
   end=read_cycle();
#endif

   elapsed = end - start;
   printf ("VDP Cycle Report. Size: %d. Cycles: %d\n", VSZ, elapsed);

   return (0);
}

#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "data.h"
#include "clarinet.h"

int main (void) {

#ifdef POSIT
#ifdef PWIDTH_8
   unsigned char acc [VSZ][VSZ];
#endif
#ifdef PWIDTH_16
   unsigned short acc [VSZ][VSZ];
#endif
#ifdef PWIDTH_24
   unsigned int acc [VSZ][VSZ];
#endif
#ifdef PWIDTH_32
   unsigned int acc [VSZ][VSZ];
#endif
#else
#ifdef DOUBLE
   double acc [VSZ][VSZ];
#else
   float acc [VSZ][VSZ];
#endif
#endif

   uint32_t start = 0;
   uint32_t end = 0;
   uint32_t elapsed = 0;

   printf ("Starting GEMM benchmarks...\n");

   // Now lets do some cycle analysis
#ifdef OPT_FLOAT
   start=read_cycle();
   fn_float_optimized_gemm (acc, VSZ, m_a, m_b);
   end=read_cycle();
#endif

#ifdef FLOAT
   start=read_cycle();
   fn_float_gemm (acc, VSZ, m_a, m_b);
   end=read_cycle();
#endif

#ifdef DOUBLE
   start=read_cycle();
   fn_double_gemm (acc, VSZ, m_a, m_b);
   end=read_cycle();
#endif

#ifdef FLOAT_POSIT
   start=read_cycle();
   fn_posit_gemm (acc, VSZ, m_a, m_b);
   end=read_cycle();
#endif

#ifdef POSIT
   start=read_cycle();
   fn_posit_p_gemm (acc, VSZ, m_a, m_b);
   end=read_cycle();
#endif

   elapsed = end - start;
   printf ("VDP Cycle Report. Size: %d. Cycles: %d\n", VSZ, elapsed);

   return (0);
}

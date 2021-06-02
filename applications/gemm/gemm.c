#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "data_gemm.h"
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
#endif

#ifdef DOUBLE
#else
#endif

   uint32_t start = 0;
   uint32_t end = 0;
   uint32_t elapsed = 0;

   // Now lets do some cycle analysis
#ifdef FLOAT
   float acc [VSZ][VSZ];
#ifdef WARM_CACHES
   fn_float_gemm (acc, VSZ, m_a, m_b);
#endif
   start=read_cycle();
   fn_float_gemm (acc, VSZ, m_a, m_b);
#endif

#ifdef DOUBLE
   double acc [VSZ][VSZ];
#ifdef WARM_CACHES
   fn_double_gemm (acc, VSZ, m_a, m_b);
#endif
   start=read_cycle();
   fn_double_gemm (acc, VSZ, m_a, m_b);
#endif

#ifdef FLOAT_POSIT
   float acc [VSZ][VSZ];
#ifdef WARM_CACHES
   fn_posit_gemm (acc, VSZ, m_a, m_b);
#endif
   start=read_cycle();
   fn_posit_gemm (acc, VSZ, m_a, m_b);
#endif

#ifdef POSIT
#ifdef WARM_CACHES
#ifdef PWIDTH_24
   fn_posit_p_gemm (acc, VSZ, m_aH, m_a, m_bH, m_b);
#else
   fn_posit_p_gemm (acc, VSZ, m_a, m_b);
#endif
#endif
   start=read_cycle();
#ifdef PWIDTH_24
   fn_posit_p_gemm (acc, VSZ, m_aH, m_a, m_bH, m_b);
#else
   fn_posit_p_gemm (acc, VSZ, m_a, m_b);
#endif
#endif

   end=read_cycle();
   elapsed = end - start;
   printf ("VDP Cycle Report. Size: %d. Cycles: %d\n", VSZ, elapsed);

   return (0);
}

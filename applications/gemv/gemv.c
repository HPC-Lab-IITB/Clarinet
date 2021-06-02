#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "data_gemv.h"
#include "clarinet.h"

int main (void) {

#ifdef POSIT
#ifdef PWIDTH_8
   unsigned char acc [VSZ];
#endif
#ifdef PWIDTH_16
   unsigned short acc [VSZ];
#endif
#ifdef PWIDTH_24
   unsigned int acc [VSZ];
#endif
#ifdef PWIDTH_32
   unsigned int acc [VSZ];
#endif
#endif

   uint32_t start = 0;
   uint32_t end = 0;
   uint32_t elapsed = 0;

   // Now lets do some cycle analysis
#ifdef FLOAT
   float acc [VSZ];
#ifdef WARM_CACHES
   fn_float_gemv (acc, VSZ, m_a, v_b);
#endif
   start=read_cycle();
   fn_float_gemv (acc, VSZ, m_a, v_b);
#endif

#ifdef DOUBLE
   double acc [VSZ];
#ifdef WARM_CACHES
   fn_double_gemv (acc, VSZ, m_a, v_b);
#endif
   start=read_cycle();
   fn_double_gemv (acc, VSZ, m_a, v_b);
#endif

#ifdef FLOAT_POSIT
   float acc [VSZ];
#ifdef WARM_CACHES
   fn_posit_gemv (acc, VSZ, m_a, v_b);
#endif
   start=read_cycle();
   fn_posit_gemv (acc, VSZ, m_a, v_b);
#endif

#ifdef POSIT
#ifdef WARM_CACHES
#ifdef PWIDTH_24
   fn_posit_p_gemv (acc, VSZ, m_aH, m_a, v_bH, v_b);
#else
   fn_posit_p_gemv (acc, VSZ, m_a, v_b);
#endif
#endif
   start=read_cycle();
#ifdef PWIDTH_24
   fn_posit_p_gemv (acc, VSZ, m_aH, m_a, v_bH, v_b);
#else
   fn_posit_p_gemv (acc, VSZ, m_a, v_b);
#endif
#endif

   end=read_cycle();
   elapsed = end - start;
#ifdef WARM_CACHES
   printf ("GEMV Cycle Report (warm caches). Size: %d. Cycles: %d\n", VSZ, elapsed);
#else
   printf ("GEMV Cycle Report. Size: %d. Cycles: %d\n", VSZ, elapsed);
#endif

   return (0);
}

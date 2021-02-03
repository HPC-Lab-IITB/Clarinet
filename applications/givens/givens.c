#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "clarinet.h"
#include "data.h"

int main (void) {

   uint32_t start = 0;
   uint32_t end = 0;
   uint32_t elapsed = 0;

#ifdef FLOAT
   float acc;
   start=read_cycle();
   fn_float_givens (VSZ, m_a, VSZ, VSZ);
   end=read_cycle();
#endif

#ifdef FLOAT_POSIT
   float acc;
   start=read_cycle();
   fn_posit_givens (VSZ, m_a, VSZ, VSZ);
   end=read_cycle();
#endif

#ifdef DOUBLE
   double acc;
   start=read_cycle();
   fn_double_givens (VSZ, m_a, VSZ, VSZ);
   end=read_cycle();
#endif

   elapsed = end - start;
   printf ("Givens Cycle Report. Size: %d. Cycles: %d\n", VSZ, elapsed);

   return (0);
}

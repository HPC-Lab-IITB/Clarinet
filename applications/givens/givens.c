#include <stdio.h>
//for read_cycle
#include "riscv_counters.h"
// for PRIu64
#include <inttypes.h>
#include "clarinet.h"
#include "data_givens.h"

// --------
// Givens routines
//

#ifdef DOUBLE
void fn_double_givens (int dim, double mat[][VSZ], int r, int c) {
   double tmp, sine_t, cosine_t, row1[dim], row2[dim];
#else
void fn_float_givens (int dim, float mat[][VSZ], int r, int c) {
   float tmp, sine_t, cosine_t, row1[dim], row2[dim];
#endif
   for (int j=0; j<r-1; j++) {
      for (int i=r-1; i>j; i--) {
         tmp = ((mat[i][j]*mat[i][j]) + (mat[i-1][j]*mat[i-1][j]));
#ifdef DOUBLE
         tmp = fn_double_sqrt (tmp);
#else
         tmp = fn_float_sqrt (tmp);
#endif
	 sine_t = (mat[i][j])/tmp;
	 cosine_t = (mat[i-1][j])/tmp;
	 for(int k=j; k<r; k++){
            row1[k] = cosine_t*mat[i-1][k] + sine_t*mat[i][k];
            row2[k] = -sine_t*mat[i-1][k] + cosine_t*mat[i][k];
         }
	 for(int k=j; k<r; k++){
            mat[i-1][k] = row1[k];
            mat[i][k] = row2[k];
         }
      }
   }
}

#ifdef FLOAT_POSIT
// Float input, posit compute, float output
void fn_posit_givens (int dim, float mat[][VSZ], int r, int c) {
   float tmp, sine_t, cosine_t;
   float row1[dim], row2[dim];
   for (int j=0; j<r-1; j++) {
      for (int i=r-1; i>j; i--) {
         // compute tmp
         fn_init_p_quire (0);
         fn_posit_fma (mat[i][j], mat[i][j]);
         fn_posit_fma (mat[i-1][j], mat[i-1][j]);
         tmp = fn_read_quire(); tmp = fn_float_sqrt (tmp);

         // compute sine_t
         fn_init_p_quire (0);
         fn_posit_fda (mat[i][j], tmp);
	 sine_t = fn_read_quire();

         // compute cosine_t
         fn_init_p_quire (0);
         fn_posit_fda (mat[i-1][j], tmp);
	 cosine_t = fn_read_quire ();

	 for(int k=j; k<r; k++){
            // compute row1[k]
            fn_init_p_quire (0);
            fn_posit_fma (cosine_t, mat[i-1][k]);
            fn_posit_fma (sine_t, mat[i][k]);
            row1[k] = fn_read_quire ();

            // compute row2[k]
            fn_init_p_quire (0);
            fn_posit_fma (cosine_t, mat[i][k]);
            fn_posit_fms (sine_t, mat[i-1][k]);
            row2[k] = fn_read_quire ();
         }

	 for(int k=j; k<r; k++){
            mat[i-1][k] = row1[k];
            mat[i][k] = row2[k];
         }
      }
   }
}
#endif
// Posit input, posit compute, posit output
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_givens (int dim, unsigned char mat[][VSZ], int r, int c) {
   unsigned char sine_t, cosine_t, p_matsq;
   unsigned char row1[dim], row2[dim];
#endif
#ifdef PWIDTH_16
void fn_posit_p_givens (int dim, unsigned short mat[][VSZ], int r, int c) {
   unsigned short sine_t, cosine_t, p_matsq;
   unsigned short row1[dim], row2[dim];
#endif
#ifdef PWIDTH_32
void fn_posit_p_givens (int dim, unsigned int mat[][VSZ], int r, int c) {
   unsigned int sine_t, cosine_t, p_matsq;
   unsigned int row1[dim], row2[dim];
#endif
   float f_matsq;
   for (int j=0; j<r-1; j++) {
      for (int i=r-1; i>j; i--) {
         // compute f_matsq
         fn_init_p_quire (0);
         fn_posit_p_fma (mat[i][j], mat[i][j]);
         fn_posit_p_fma (mat[i-1][j], mat[i-1][j]);
         f_matsq = fn_read_quire(); f_matsq = fn_float_sqrt (f_matsq);
         p_matsq = fn_float_to_posit (f_matsq);

         // compute sine_t
         fn_init_p_quire (0);
         fn_posit_p_fda (mat[i][j], p_matsq);
	 sine_t = fn_read_p_quire();

         // compute cosine_t
         fn_init_p_quire (0);
         fn_posit_fda (mat[i-1][j], p_matsq);
	 cosine_t = fn_read_p_quire ();

	 for(int k=j; k<r; k++){
            // compute row1[k]
            fn_init_p_quire (0);
            fn_posit_p_fma (cosine_t, mat[i-1][k]);
            fn_posit_p_fma (sine_t, mat[i][k]);
            row1[k] = fn_read_p_quire ();

            // compute row2[k]
            fn_init_p_quire (0);
            fn_posit_fma (cosine_t, mat[i][k]);
            fn_posit_fms (sine_t, mat[i-1][k]);
            row2[k] = fn_read_p_quire ();
         }

	 for(int k=j; k<r; k++){
            mat[i-1][k] = row1[k];
            mat[i][k] = row2[k];
         }
      }
   }
}
#endif

int main (void) {

   uint32_t start = 0;
   uint32_t end = 0;
   uint32_t elapsed = 0;

#ifdef FLOAT
#ifdef WARM_CACHES
   fn_float_givens (VSZ, m_a, VSZ, VSZ);
#endif
   start=read_cycle();
   fn_float_givens (VSZ, m_a, VSZ, VSZ);
   end=read_cycle();
#endif

#ifdef FLOAT_POSIT
#ifdef WARM_CACHES
   fn_posit_givens (VSZ, m_a, VSZ, VSZ);
#endif
   start=read_cycle();
   fn_posit_givens (VSZ, m_a, VSZ, VSZ);
   end=read_cycle();
#endif

#ifdef DOUBLE
#ifdef WARM_CACHES
   fn_double_givens (VSZ, m_a, VSZ, VSZ);
#endif
   start=read_cycle();
   fn_double_givens (VSZ, m_a, VSZ, VSZ);
   end=read_cycle();
#endif

#ifdef POSIT
#ifdef WARM_CACHES
   fn_posit_p_givens (VSZ, m_a, VSZ, VSZ);
#endif
   start=read_cycle();
   fn_posit_p_givens (VSZ, m_a, VSZ, VSZ);
   end=read_cycle();
#endif

   elapsed = end - start;
#ifdef WARM_CACHES
   printf ("Givens Cycle Report (warm caches). Size: %d. Cycles: %d\n", VSZ, elapsed);
#else
   printf ("Givens Cycle Report. Size: %d. Cycles: %d\n", VSZ, elapsed);
#endif

   return (0);
}

#include <stdio.h>
#include <math.h>
#include "clarinet.h"

// For a detailed explainer of inline assembler, the following resource is excellent:
// http://www.ethernut.de/en/documents/arm-inline-asm.html

// --------
// FMA macros
// the _p_ indicates that the input values are posit type
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_fma (unsigned char a, unsigned char b) {
#endif
#ifdef PWIDTH_16
void fn_posit_p_fma (unsigned short a, unsigned short b) {
#endif
#ifdef PWIDTH_24
void fn_posit_p_fma (unsigned int a, unsigned int b) {
#endif
#ifdef PWIDTH_32
void fn_posit_p_fma (unsigned int a, unsigned int b) {
#endif
   asm volatile (
          "pmv.w.x fa4, %[inA]\n"
          "pmv.w.x fa5, %[inB]\n"
          "fma.p   ft0, fa4, fa5"
          : 
          : [inA] "r" (a), [inB] "r" (b)  /* GPR registers */
          : "ft0", "fa4", "fa5" /* clobber list */
       );
   return;
}
#endif

void fn_posit_fma (float a, float b) {
   asm volatile (
         "fcvt.p.s ft0, %[inA]\n" 
         "fcvt.p.s ft1, %[inB]\n" 
         "fma.p    ft0, ft0, ft1"
         :
         : [inA] "f" (a), [inB] "f" (b)
         : "ft0", "ft1"
       );
   return;
}

#ifdef DOUBLE
double fn_double_fma (double a, double b, double acc) {
   double res;
   asm volatile ( "fmadd.d   %[o_acc], %[i_a], %[i_b], %[i_acc]"
#else
float fn_float_fma (float a, float b, float acc) {
   float res;
   asm volatile ( "fmadd.s   %[o_acc], %[i_a], %[i_b], %[i_acc]"
#endif
         : [o_acc] "=f" (res)
         : [i_a] "f" (a), [i_b] "f" (b), [i_acc] "f" (acc)
         :
       );
   return (res);
}

// --------
// FMS macros
// the _p_ indicates that the input values are posit type
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_fms (unsigned char a, unsigned char b) {
#endif
#ifdef PWIDTH_16
void fn_posit_p_fms (unsigned short a, unsigned short b) {
#endif
#ifdef PWIDTH_24
void fn_posit_p_fms (unsigned int a, unsigned int b) {
#endif
#ifdef PWIDTH_32
void fn_posit_p_fms (unsigned int a, unsigned int b) {
#endif
   asm volatile (
          "pmv.w.x fa4, %[inA]\n"
          "pmv.w.x fa5, %[inB]\n"
          "fms.p   ft0, fa4, fa5"
          : 
          : [inA] "r" (a), [inB] "r" (b)  /* GPR registers */
          : "ft0", "fa4", "fa5" /* clobber list */
       );
   return;
}
#endif

void fn_posit_fms (float a, float b) {
   asm volatile (
         "fcvt.p.s ft0, %[inA]\n" 
         "fcvt.p.s ft1, %[inB]\n" 
         "fms.p    ft0, ft0, ft1"
         :
         : [inA] "f" (a), [inB] "f" (b)
         : "ft0", "ft1"
       );
   return;
}

// --------
// FDA macros
// the _p_ indicates that the input values are posit type
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_fda (unsigned char a, unsigned char b) {
#endif
#ifdef PWIDTH_16
void fn_posit_p_fda (unsigned short a, unsigned short b) {
#endif
#ifdef PWIDTH_24
void fn_posit_p_fda (unsigned int a, unsigned int b) {
#endif
#ifdef PWIDTH_32
void fn_posit_p_fda (unsigned int a, unsigned int b) {
#endif
   asm volatile (
          "pmv.w.x fa4, %[inA]\n"
          "pmv.w.x fa5, %[inB]\n"
          "fda.p   ft0, fa4, fa5"
          : 
          : [inA] "r" (a), [inB] "r" (b)  /* GPR registers */
          : "ft0", "fa4", "fa5" /* clobber list */
       );
   return;
}
#endif

void fn_posit_fda (float a, float b) {
   asm volatile (
         "fcvt.p.s ft0, %[inA]\n" 
         "fcvt.p.s ft1, %[inB]\n" 
         "fda.p    ft0, ft0, ft1"
         :
         : [inA] "f" (a), [inB] "f" (b)
         : "ft0", "ft1"
       );
   return;
}

// --------
// FDS macros
// the _p_ indicates that the input values are posit type
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_fds (unsigned char a, unsigned char b) {
#endif
#ifdef PWIDTH_16
void fn_posit_p_fds (unsigned short a, unsigned short b) {
#endif
#ifdef PWIDTH_24
void fn_posit_p_fds (unsigned int a, unsigned int b) {
#endif
#ifdef PWIDTH_32
void fn_posit_p_fds (unsigned int a, unsigned int b) {
#endif
   asm volatile (
          "pmv.w.x fa4, %[inA]\n"
          "pmv.w.x fa5, %[inB]\n"
          "fds.p   ft0, fa4, fa5"
          : 
          : [inA] "r" (a), [inB] "r" (b)  /* GPR registers */
          : "ft0", "fa4", "fa5" /* clobber list */
       );
   return;
}
#endif

void fn_posit_fds (float a, float b) {
   asm volatile (
         "fcvt.p.s ft0, %[inA]\n" 
         "fcvt.p.s ft1, %[inB]\n" 
         "fds.p    ft0, ft0, ft1"
         :
         : [inA] "f" (a), [inB] "f" (b)
         : "ft0", "ft1"
       );
   return;
}

// Using the math sqrt does not seem to infer fsqrt.s instruction.
// This function explicitly uses the fsqrt instruction
#ifdef DOUBLE
double fn_double_sqrt (double a) {
   double res;
   asm volatile ( "fsqrt.d %[o_res], %[i_a]"
#else
float fn_float_sqrt (float a) {
   float res;
   asm volatile ( "fsqrt.s %[o_res], %[i_a]"
#endif
         : [o_res] "=f" (res)
         : [i_a]   "f"  (a)
         :
       );
   return (res);
}

// --------
// Quire initialization macros
// the _p_ indicates that the input values are posit type
#ifdef PWIDTH_8
void fn_init_p_quire (unsigned char initVal) {
#endif
#ifdef PWIDTH_16
void fn_init_p_quire (unsigned short initVal) {
#endif
#ifdef PWIDTH_24
void fn_init_p_quire (unsigned int initVal) {
#endif
#ifdef PWIDTH_32
void fn_init_p_quire (unsigned int initVal) {
#endif
   asm volatile (
         "pmv.w.x fa4, %[inA]\n"
         "fcvt.r.p ft0, fa4"
         : 
         : [inA] "r" (initVal)
         : "fa4", "ft0"
       );
   return;
}

void fn_init_quire (float initVal) {
   asm volatile (
         "fcvt.p.s ft1, %[inA]\n"
         "fcvt.r.p ft0, ft1"
         :
         : [inA] "f" (initVal)
         : "ft1", "ft0"
       );
   return;
}

// --------
// Quire read macros
// Read the quire and return the appropriately sized posit
#ifdef POSIT
#ifdef PWIDTH_8
unsigned char fn_read_p_quire (void) {
   unsigned char res;
#endif
#ifdef PWIDTH_16
unsigned short fn_read_p_quire (void) {
   unsigned short res;
#endif
#ifdef PWIDTH_24
unsigned int fn_read_p_quire (void) {
   unsigned int res;
#endif
#ifdef PWIDTH_32
unsigned int fn_read_p_quire (void) {
   unsigned int res;
#endif
   asm volatile ( "fcvt.p.r ft0, ft0 \n"
         "pmv.x.w  %[o_res], ft0" 
         : [o_res] "=r" (res)
         : 
         : "ft0"
       );
   return (res);
}
#endif

// Read the quire and return a float
float fn_read_quire (void) {
   float res;
   asm volatile ( "fcvt.p.r ft0, ft0 \n"
         "fcvt.s.p %[o_res], ft0" 
         : [o_res] "=f" (res)
         : 
         : "ft0"
       );
   return (res);
}


// --------
// Float to Posit conversion
#ifdef POSIT
#ifdef PWIDTH_8
unsigned char  fn_float_to_posit (float a) {
   unsigned char res;
#endif
#ifdef PWIDTH_16
unsigned short fn_float_to_posit (float a) {
   unsigned short res;
#endif
#ifdef PWIDTH_24
unsigned int   fn_float_to_posit (float a) {
   unsigned int res;
#endif
#ifdef PWIDTH_32
unsigned int   fn_float_to_posit (float a) {
   unsigned int res;
#endif
   asm volatile ( "fcvt.p.s ft0, %[i_a]\n"
         "pmv.x.w  %[o_res], ft0" 
         : [o_res] "=r" (res)
         : [i_a] "f" (a)
         : "ft0"
       );
   return (res);
}
#endif

// --------
// VDP Routines
// Posit input, posit compute, posit output
#ifdef POSIT
#ifdef PWIDTH_8
unsigned char fn_posit_p_vdp (int r, unsigned char p_a[], unsigned char p_b[]) {
   unsigned char acc;
#endif
#ifdef PWIDTH_16
unsigned short fn_posit_p_vdp (int r, unsigned short p_a[], unsigned short p_b[]) {
   unsigned short acc;
#endif
#ifdef PWIDTH_24
unsigned int fn_posit_p_vdp (int r, unsigned char p_aH[], unsigned short p_aL[], unsigned char p_bH[], unsigned short p_bL[]) {
   unsigned int acc;
#endif
#ifdef PWIDTH_32
unsigned int fn_posit_p_vdp (int r, unsigned int p_a[], unsigned int p_b[]) {
   unsigned int acc;
#endif

   int idx = 0; int rep = 0;
   fn_init_p_quire (0);

   for (idx=0; idx < r; idx ++) {
#ifdef PWIDTH_24
      fn_posit_p_fma (
              (unsigned int) (p_aL[idx] | (p_aH[idx] << 16))
            , (unsigned int) (p_bL[idx] | (p_bH[idx] << 16)));
#else
      fn_posit_p_fma (p_a[idx], p_b[idx]);
#endif
   }

   acc = fn_read_p_quire ();
   return (acc);
}
#endif

#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_gemv (unsigned char  v_acc[], int r, unsigned char  m_a[][VSZ], unsigned char  p_b[]) {
#endif
#ifdef PWIDTH_16
void fn_posit_p_gemv (unsigned short v_acc[], int r, unsigned short m_a[][VSZ], unsigned short p_b[]) {
#endif
#ifdef PWIDTH_24
void fn_posit_p_gemv (unsigned int   v_acc[], int r, unsigned char  m_aH[][VSZ], unsigned short m_a[][VSZ], unsigned char p_bH[], unsigned short p_b[]) {
#endif
#ifdef PWIDTH_32
void fn_posit_p_gemv (unsigned int   v_acc[], int r, unsigned int   m_a[][VSZ], unsigned int   p_b[]) {
#endif
   int idx = 0;
   for (idx=0; idx < r; idx ++)
#ifdef PWIDTH_24
      v_acc[idx] = fn_posit_p_vdp (r, m_aH[idx], m_a[idx], p_bH, p_b);
#else
      v_acc[idx] = fn_posit_p_vdp (r, m_a[idx], p_b);
#endif

   return;
}
#endif

#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_gemm (unsigned char  m_acc[][VSZ], int dimension, unsigned char  m_a[][VSZ], unsigned char  m_b[][VSZ]) {
   unsigned char v_acc[VSZ];
#endif
#ifdef PWIDTH_16
void fn_posit_p_gemm (unsigned short m_acc[][VSZ], int dimension, unsigned short m_a[][VSZ], unsigned short m_b[][VSZ]) {
   unsigned short v_acc[VSZ];
#endif
#ifdef PWIDTH_24
void fn_posit_p_gemm (unsigned int   m_acc[][VSZ], int dimension, unsigned char m_aH[][VSZ], unsigned short m_a[][VSZ], unsigned char  m_bH[][VSZ], unsigned short m_b[][VSZ]) {
   unsigned int v_acc[VSZ];
#endif
#ifdef PWIDTH_32
void fn_posit_p_gemm (unsigned int   m_acc[][VSZ], int dimension, unsigned int   m_a[][VSZ], unsigned int   m_b[][VSZ]) {
   unsigned int v_acc[VSZ];
#endif
   int ridx = 0; int cidx = 0;

   for (ridx=0; ridx < dimension; ridx++) {
#ifdef PWIDTH_24
      fn_posit_p_gemv (v_acc, dimension, m_aH, m_a, m_bH[ridx], m_b[ridx]);
#else
      fn_posit_p_gemv (v_acc, dimension, m_a, m_b[ridx]);
#endif
      // implement a transpose to get the result matrix
      for (cidx=0; cidx < dimension; cidx++) {
         m_acc[cidx][ridx] = v_acc[cidx];
      }
   }

   return;
}
#endif

// Float input, posit compute, float output
float fn_posit_vdp (int r, float v_a[], float v_b[]) {
   float acc;
   int idx = 0;
   fn_init_p_quire (0);
   for (idx=0; idx < r; idx ++) fn_posit_fma (v_a[idx], v_b[idx]);
   acc = fn_read_quire ();
   return (acc);
}

// Float input, posit compute, float output
void fn_posit_gemv (float v_acc[], int r, float m_a[][VSZ], float v_b[]) {
   int idx = 0;
   for (idx=0; idx < r; idx ++) v_acc[idx] = fn_posit_vdp (r, m_a[idx], v_b);
   return;
}

// Float input, float compute, float output. Code written to allow compiler optimization wrt
// function calls.
void fn_posit_gemm (float m_acc[][VSZ], int dimension, float m_a[][VSZ], float m_b[][VSZ]) {
   int ridx = 0; int cidx = 0;
   float v_acc[VSZ];

   for (ridx=0; ridx < dimension; ridx++) {
      fn_posit_gemv (v_acc, dimension, m_a, m_b[ridx]);
      // implement a transpose to get the result matrix
      for (cidx=0; cidx < dimension; cidx++) {
         m_acc[cidx][ridx] = v_acc[cidx];
      }
   }

   return;
}

#ifdef DOUBLE
double fn_double_vdp (int r, double v_a[], double v_b[]) {
   double acc = 0.0;
#else
float fn_float_vdp (int r, float v_a[], float v_b[]) {
   float acc = 0.0;
#endif

   int idx = 0;
   for (idx=0; idx < r; idx ++)

#ifdef FMADD_ENABLED
#ifdef DOUBLE
      acc = fn_double_fma (v_a[idx], v_b[idx], acc);
#else
      acc = fn_float_fma (v_a[idx], v_b[idx], acc);
#endif
#else
      acc += (v_a[idx] * v_b[idx]);
#endif
   return (acc);
}


#ifdef DOUBLE
// Double input, double compute, double output
void fn_double_gemv (double v_acc[], int r, double m_a[][VSZ], double v_b[]) {
   int idx = 0;
   for (idx=0; idx < r; idx ++)
      v_acc[idx] = fn_double_vdp (r, m_a[idx], v_b);

   return;
}
#else

// Float input, float compute, float output
void fn_float_gemv (float v_acc[], int r, float m_a[][VSZ], float v_b[]) {
   int idx = 0;
   for (idx=0; idx < r; idx ++)
      v_acc[idx] = fn_float_vdp (r, m_a[idx], v_b);

   return;
}
#endif

#ifdef DOUBLE
// double input, double compute, double output. The second matrix is the transposed version
void fn_double_gemm (double m_acc[][VSZ], int dimension, double m_a[][VSZ], double m_b[][VSZ]) {
   int ridx = 0; int cidx = 0;
   double v_acc[VSZ];
   for (ridx=0; ridx < dimension; ridx++) {
      fn_double_gemv (v_acc, dimension, m_a, m_b[ridx]);
      // implement a transpose to get the result matrix
      for (cidx=0; cidx < dimension; cidx++) {
         m_acc[cidx][ridx] = v_acc[cidx];
      }
   }

   return;
}
#else

// Float input, float compute, float output. The second matrix is the transposed version
void fn_float_gemm (float m_acc[][VSZ], int dimension, float m_a[][VSZ], float m_b[][VSZ]) {
   int ridx = 0; int cidx = 0;
   float v_acc[VSZ];
   for (ridx=0; ridx < dimension; ridx++) {
      fn_float_gemv (v_acc, dimension, m_a, m_b[ridx]);
      // implement a transpose to get the result matrix
      for (cidx=0; cidx < dimension; cidx++) {
         m_acc[cidx][ridx] = v_acc[cidx];
      }
   }

   return;
}
#endif


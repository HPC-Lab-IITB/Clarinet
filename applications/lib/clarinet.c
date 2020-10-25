#include <stdio.h>
#include "clarinet.h"

// the _p_ indicates that the input values are posit type
void fn_posit_p_fma (unsigned int a, unsigned int b) {
   // input posit values in GPR
   register unsigned int gA asm ("a0") = a;
   register unsigned int gB asm ("a1") = b;

   // intermediate posit values in PPR
   register float pA asm ("ft0");
   register float pB asm ("ft1");

   asm ("pmv.w.x ft0, a0" : "=f" (pA) : "r" (gA));
   asm ("pmv.w.x ft1, a1" : "=f" (pB) : "r" (gB));

   // for instrns which do not have a rd, using zeros for the rd
   // field is necessary as that is the instruction encoding.
   asm ("fma.p    ft0, ft0, ft1" : "=f" (pA) : "f" (pA), "f" (pB));
   return;
}

void fn_posit_fma (float a, float b) {
   // input float values
   register float fA asm ("fa0") = a;
   register float fB asm ("fa1") = b;

   // intermediate posit values
   register float pA asm ("ft0");
   register float pB asm ("ft1");

   asm ("fcvt.p.s ft0, fa0" : "=f" (pA) : "f" (fA));
   asm ("fcvt.p.s ft1, fa1" : "=f" (pB) : "f" (fB));
   // for instrns which do not have a rd, using zeros for the rd
   // field is necessary as that is the instruction encoding.
   asm ("fma.p    ft0, ft0, ft1" : "=f" (pA) : "f" (pA), "f" (pB));
   return;
}

float fn_float_fma (float a, float b, float acc) {
   // input float values
   register float f_a asm ("fa0") = a;
   register float f_b asm ("fa1") = b;
   register float f_acc asm ("fa2") = acc;
   register float f_prod asm ("fa3");
   register float f_res asm ("fa4");

   asm ("fmul.s    fa3, fa0, fa1" : "=f" (f_prod) : "f" (f_a), "f" (f_b));
   asm ("fadd.s    fa4, fa3, fa2" : "=f" (f_res)  : "f" (f_prod), "f" (f_acc));
   return (f_res);
}

// the _p_ indicates that the input values are posit type
void fn_init_p_quire (unsigned int initVal) {
   register unsigned int gI asm ("a0") = initVal;
   register float pI asm ("ft1");
   register float q  asm ("ft0");
   asm ("pmv.w.x ft1, a0" : "=f" (pI) : "r" (gI));
   // for instrns which do not have a rd, using zeros for the rd
   // field is necessary as that is the instruction encoding.
   asm ("fcvt.r.p ft0, ft1" : "=f" (q) : "f" (pI));
   return;
}

void fn_init_quire (float initVal) {
   register float fI asm ("fa0") = initVal;
   register float pQ asm ("ft1");
   register float qQ asm ("ft0");
   asm ("fcvt.p.s ft1, fa0" : "=f" (pQ) : "f" (fI));
   // for instrns which do not have a rd, using zeros for the rd
   // field is necessary as that is the instruction encoding.
   asm ("fcvt.r.p ft0, ft1" : "=f" (qQ) : "f" (pQ));
   return;
}

unsigned int fn_read_p_quire (void) {
   register float pQ asm ("f0");
   register float gO asm ("a0");
   asm ("fcvt.p.r f0, f0" : "=f" (pQ) : "f" (pQ));
   asm ("pmv.x.w  a0, f0" : "=r" (gO) : "f" (pQ));
   return (gO);
}

float fn_read_quire (void) {
   register float pQ asm ("f0");
   register float fO asm ("fa0");
   asm ("fcvt.p.r f0, f0" : "=f" (pQ) : "f" (pQ));
   asm ("fcvt.s.p fa0, f0" : "=f" (fO) : "f" (pQ));
   return (fO);
}


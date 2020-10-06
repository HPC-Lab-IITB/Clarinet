#include <stdlib.h>

static float v_a [5] = {1.0, 2.0, 3.0, 4.0, 5.0};
static float v_b [5] = {1.25, 2.25, 3.25, 4.25, 5.25};

static inline void fn_posit_fma (float a, float b) {
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

float fn_read_quire (void) {
   register float pQ asm ("f0");
   register float fO asm ("fa0");
   asm ("fcvt.p.r f0, f0" : "=f" (pQ) : "f" (pQ));
   asm ("fcvt.s.p fa0, f0" : "=f" (fO) : "f" (pQ));
   return (fO);
}

float fn_posit_vdp (void) {
   float acc = 0.0;
   int idx = 0;
   fn_init_quire (acc);
   for (idx=0; idx<5; idx++) fn_posit_fma (v_a[idx], v_b[idx]);
   acc = fn_read_quire ();
   return (acc);
}

float fn_float_vdp (void) {
   float acc = 0.0;
   int idx = 0;

   for (idx=0; idx<5; idx++) acc += v_a[idx]*v_b[idx];
   return (acc);
}

int main (void) {
   float acc1 = fn_float_vdp ();
   float acc2 = fn_posit_vdp ();

   if (acc1 != acc2) return (1);
   else return (0);
}

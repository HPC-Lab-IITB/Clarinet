// the size of the array - repeat this n times to get longer arrays. Ignores cache effects
// though which may be interesting when we reduce posit width.
#define VSZ 16

#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_fma (unsigned char a, unsigned char b);
#endif
#ifdef PWIDTH_16
void fn_posit_p_fma (unsigned short a, unsigned short b);
#endif
#ifdef PWIDTH_24
void fn_posit_p_fma (unsigned int a, unsigned int b);
#endif
#ifdef PWIDTH_32
void fn_posit_p_fma (unsigned int a, unsigned int b);
#endif
#endif
void fn_posit_fma (float a, float b);
float fn_float_fma (float a, float b, float acc);

void fn_init_p_quire (unsigned int initVal);
void fn_init_quire (float initVal);

#ifdef POSIT
#ifdef PWIDTH_8
unsigned char fn_read_p_quire (void);
#endif
#ifdef PWIDTH_16
unsigned short fn_read_p_quire (void);
#endif
#ifdef PWIDTH_24
unsigned int fn_read_p_quire (void);
#endif
#ifdef PWIDTH_32
unsigned int fn_read_p_quire (void);
#endif
#endif

float fn_read_quire (void);
#ifdef POSIT
#ifdef PWIDTH_8
unsigned char fn_posit_p_vdp (int r, unsigned char a[], unsigned char b[]);
#endif
#ifdef PWIDTH_16
unsigned short fn_posit_p_vdp (int r, unsigned short a[], unsigned short b[]);
#endif
#ifdef PWIDTH_24
unsigned int fn_posit_p_vdp (int r, unsigned int a[], unsigned int b[]);
#endif
#ifdef PWIDTH_32
unsigned int fn_posit_p_vdp (int r, unsigned int a[], unsigned int b[]);
#endif
#endif
float fn_posit_vdp (int r, float a[], float b[]);
float fn_float_vdp (int r, float a[], float b[]);
float fn_float_optimized_vdp (int r, float a[], float b[]);

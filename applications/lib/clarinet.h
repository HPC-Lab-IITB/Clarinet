// --------
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
#ifdef DOUBLE
double fn_double_fma (double a, double b, double acc);
#endif

// --------
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_fms (unsigned char a, unsigned char b);
#endif
#ifdef PWIDTH_16
void fn_posit_p_fms (unsigned short a, unsigned short b);
#endif
#ifdef PWIDTH_24
void fn_posit_p_fms (unsigned int a, unsigned int b);
#endif
#ifdef PWIDTH_32
void fn_posit_p_fms (unsigned int a, unsigned int b);
#endif
#endif
void fn_posit_fms (float a, float b);

// --------
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_fda (unsigned char a, unsigned char b);
#endif
#ifdef PWIDTH_16
void fn_posit_p_fda (unsigned short a, unsigned short b);
#endif
#ifdef PWIDTH_24
void fn_posit_p_fda (unsigned int a, unsigned int b);
#endif
#ifdef PWIDTH_32
void fn_posit_p_fda (unsigned int a, unsigned int b);
#endif
#endif
void fn_posit_fda (float a, float b);

// --------
float fn_float_sqrt (float a);
#ifdef DOUBLE
double fn_double_sqrt (double a);
#endif

// --------
void fn_init_p_quire (unsigned int initVal);
void fn_init_quire (float initVal);


// --------
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

// --------
#ifdef POSIT
#ifdef PWIDTH_8
unsigned char  fn_float_to_posit (float);
#endif
#ifdef PWIDTH_16
unsigned short fn_float_to_posit (float);
#endif
#ifdef PWIDTH_24
unsigned int   fn_float_to_posit (float);
#endif
#ifdef PWIDTH_32
unsigned int   fn_float_to_posit (float);
#endif
#endif


// --------
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
#ifdef DOUBLE
double fn_double_vdp (int r, double a[], double b[]);
#endif


// --------
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_gemv          (unsigned char  v_acc[], int r, unsigned char  a[][VSZ], unsigned char b[]);
#endif
#ifdef PWIDTH_16
void  fn_posit_p_gemv         (unsigned short v_acc[], int r, unsigned short a[][VSZ], unsigned short b[]);
#endif
#ifdef PWIDTH_24
void n_posit_p_gemv           (unsigned int   v_acc[], int r, unsigned int   a[][VSZ], unsigned int b[]);
#endif
#ifdef PWIDTH_32
void n_posit_p_gemv           (unsigned int   v_acc[], int r, unsigned int   a[][VSZ], unsigned int b[]);
#endif
#endif
void fn_posit_gemv            (float v_acc[], int r, float a[][VSZ], float b[]);
void fn_float_gemv            (float v_acc[], int r, float a[][VSZ], float b[]);
void fn_float_optimized_gemv  (float v_acc[], int r, float a[][VSZ], float b[]);
#ifdef DOUBLE
void fn_double_gemv           (double v_acc[], int r, double a[][VSZ], double b[]);
#endif


// --------
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_gemm          (unsigned char  m_acc[][VSZ], int r, unsigned char  a[][VSZ], unsigned char  b[][VSZ]);
#endif
#ifdef PWIDTH_16
void  fn_posit_p_gemm         (unsigned short m_acc[][VSZ], int r, unsigned short a[][VSZ], unsigned short b[][VSZ]);
#endif
#ifdef PWIDTH_24
void n_posit_p_gemm           (unsigned int   m_acc[][VSZ], int r, unsigned int   a[][VSZ], unsigned int   b[][VSZ]);
#endif
#ifdef PWIDTH_32
void n_posit_p_gemm           (unsigned int   m_acc[][VSZ], int r, unsigned int   a[][VSZ], unsigned int   b[][VSZ]);
#endif
#endif
void fn_posit_gemm            (float   m_acc[][VSZ], int r, float    a[][VSZ], float   b[][VSZ]);
void fn_float_gemm            (float   m_acc[][VSZ], int r, float    a[][VSZ], float   b[][VSZ]);
void fn_float_optimized_gemm  (float   m_acc[][VSZ], int r, float    a[][VSZ], float   b[][VSZ]);
#ifdef DOUBLE
void fn_double_gemm           (double  m_acc[][VSZ], int r, double   a[][VSZ], double  b[][VSZ]);
#endif


// --------
#ifdef POSIT
#ifdef PWIDTH_8
void fn_posit_p_givens        (int dim, unsigned char  mat[][VSZ], int r, int c);
#endif
#ifdef PWIDTH_16
void fn_posit_p_givens        (int dim, unsigned short mat[][VSZ], int r, int c);
#endif
#ifdef PWIDTH_24
void fn_posit_p_givens        (int dim, unsigned int   mat[][VSZ], int r, int c);
#endif
#ifdef PWIDTH_32
void fn_posit_p_givens        (int dim, unsigned int   mat[][VSZ], int r, int c);
#endif
#endif
void fn_float_givens          (int dim, float  mat[][VSZ], int r, int c);
void fn_posit_givens          (int dim, float  mat[][VSZ], int r, int c);
#ifdef DOUBLE
void fn_double_givens         (int dim, double mat[][VSZ], int r, int c);
#endif

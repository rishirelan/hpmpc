// kernel
void kernel_dgemm_pp_nt_8x4_avx_lib4(int kmax, double *A0, double *A1, double *B, double *C0, double *C1, int ldc, int alg);
void kernel_dgemm_pp_nt_8x2_avx_lib4(int kmax, double *A0, double *A1, double *B, double *C0, double *C1, int ldc, int alg);
void kernel_dgemm_pp_nt_8x1_avx_lib4(int kmax, double *A0, double *A1, double *B, double *C0, double *C1, int ldc, int alg);
void kernel_dgemm_pp_nt_4x4_avx_lib4(int kmax, double *A, double *B, double *C, int ldc, int alg);
void kernel_dgemm_pp_nt_4x2_avx_lib4(int kmax, double *A, double *B, double *C, int ldc, int alg);
void kernel_dgemm_pp_nt_4x1_avx_lib4(int kmax, double *A, double *B, double *C, int ldc, int alg);
void kernel_dpotrf_dtrsv_dcopy_4x4_sse_lib4(int kmax, int kna_dummy, double *A, int sda, int shf, double *L, int sdl);
void kernel_dpotrf_dtrsv_4x4_sse_lib4(int kmax, int kna_dummy, double *A, int sda);
void kernel_dgemv_t_8_avx_lib4(int kmax, int kna, double *A, int sda, double *x, double *y, int alg);
void kernel_dgemv_t_4_avx_lib4(int kmax, int kna, double *A, int sda, double *x, double *y, int alg);
void kernel_dgemv_t_2_avx_lib4(int kmax, int kna, double *A, int sda, double *x, double *y, int alg);
void kernel_dgemv_t_1_avx_lib4(int kmax, int kna, double *A, int sda, double *x, double *y, int alg);
void kernel_dgemv_n_8_avx_lib4(int kmax, double *A0, double *A1, double *x, double *y, int alg);
void kernel_dgemv_n_4_avx_lib4(int kmax, double *A, double *x, double *y, int alg);
void kernel_dgemv_n_2_avx_lib4(int kmax, double *A, double *x, double *y, int alg);
void kernel_dgemv_n_1_avx_lib4(int kmax, double *A, double *x, double *y, int alg);
// corner
void corner_dtrmm_pp_nt_8x3_avx_lib4(double *A0, double *A1, double *B, double *C0, double *C1, int ldc);
void corner_dtrmm_pp_nt_8x2_avx_lib4(double *A0, double *A1, double *B, double *C0, double *C1, int ldc);
void corner_dtrmm_pp_nt_8x1_avx_lib4(double *A0, double *A1, double *B, double *C0, double *C1, int ldc);
void corner_dtrmm_pp_nt_4x3_avx_lib4(double *A, double *B, double *C, int ldc);
void corner_dtrmm_pp_nt_4x2_avx_lib4(double *A, double *B, double *C, int ldc);
void corner_dtrmm_pp_nt_4x1_avx_lib4(double *A, double *B, double *C, int ldc);
void corner_dpotrf_dtrsv_dcopy_2x2_sse_lib4(double *A, int sda, int shf, double *L, int sdl);


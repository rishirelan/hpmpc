/**************************************************************************************************
*                                                                                                 *
* This file is part of HPMPC.                                                                     *
*                                                                                                 *
* HPMPC -- Library for High-Performance implementation of solvers for MPC.                        *
* Copyright (C) 2014 by Technical University of Denmark. All rights reserved.                     *
*                                                                                                 *
* HPMPC is free software; you can redistribute it and/or                                          *
* modify it under the terms of the GNU Lesser General Public                                      *
* License as published by the Free Software Foundation; either                                    *
* version 2.1 of the License, or (at your option) any later version.                              *
*                                                                                                 *
* HPMPC is distributed in the hope that it will be useful,                                        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                                            *
* See the GNU Lesser General Public License for more details.                                     *
*                                                                                                 *
* You should have received a copy of the GNU Lesser General Public                                *
* License along with HPMPC; if not, write to the Free Software                                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA                  *
*                                                                                                 *
* Author: Gianluca Frison, giaf (at) dtu.dk                                                       *
*                                                                                                 *
**************************************************************************************************/



// normal-transposed, 4x4 with data packed in 4
// prefetch optimized for Cortex-A9 (cache line is 32 bytes, while A15 is 64 bytes)
/*void kernel_dgemm_pp_nt_4x4_lib4(int kmax, double *A, double *B, double *C, double *D, int ldc_dummy, int alg)*/
void kernel_dtrsm_pp_nt_4x4_lib4(int kadd, int ksub, double *A, double *B, double *C, double *D, int ldc_dummy, double *fact)
	{
	
	__builtin_prefetch( A );
	__builtin_prefetch( B );
#if defined(CORTEX_A9)
	__builtin_prefetch( A+4 );
	__builtin_prefetch( B+4 );
#endif

	int ki_add = kadd/2;
	int kl_add = kadd%2;
	int ki_sub = ksub/2;

	const int bs = 4;
	const int d_ncl = 2;

	int dA = bs*((d_ncl-kadd%d_ncl)%d_ncl)*sizeof(double);
/*	int dA = bs*((d_ncl-kadd%d_ncl)%d_ncl);*/

	__builtin_prefetch( A+8 );
	__builtin_prefetch( B+8 );
#if defined(CORTEX_A9)
	__builtin_prefetch( A+12 );
	__builtin_prefetch( B+12 );
#endif

//	printf("\n%d %d %d\n", kmax, k_iter, k_left);

	__asm__ volatile
	(
		"                                \n\t"
		"mov    r3, %0                   \n\t" // k_iter
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
//		"mov    r2, %5                   \n\t" // load address of C
		"                                \n\t"
		"                                \n\t"
		"fldd   d16, [%3, #0]            \n\t" // prefetch A_even
		"fldd   d17, [%3, #8]            \n\t"
		"fldd   d18, [%3, #16]           \n\t"
		"fldd   d19, [%3, #24]           \n\t"
		"                                \n\t"
		"fldd   d20, [%4, #0]            \n\t" // prefetch B_even
		"fldd   d21, [%4, #8]            \n\t"
		"fldd   d22, [%4, #16]           \n\t"
		"fldd   d23, [%4, #24]           \n\t"
		"                                \n\t"
		"cmp    r3, #0                   \n\t"
		"                                \n\t"
		"fldd   d24, [%3, #32]           \n\t" // prefetch A_odd
		"fldd   d25, [%3, #40]           \n\t"
		"fldd   d26, [%3, #48]           \n\t"
		"fldd   d27, [%3, #56]           \n\t"
		"                                \n\t"
		"fldd   d28, [%4, #32]           \n\t" // prefetch B_odd
		"fldd   d29, [%4, #40]           \n\t"
		"fldd   d30, [%4, #48]           \n\t"
		"fldd   d31, [%4, #56]           \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"fldd   d0, .DOUBLEZERO          \n\t" // load zero double
		"fcpyd  d1, d0                   \n\t"
		"fcpyd  d2, d0                   \n\t"
		"fcpyd  d3, d0                   \n\t"
		"fcpyd  d4, d0                   \n\t"
		"fcpyd  d5, d0                   \n\t"
		"fcpyd  d6, d0                   \n\t"
		"fcpyd  d7, d0                   \n\t"
		"fcpyd  d8, d0                   \n\t"
		"fcpyd  d9, d0                   \n\t"
		"fcpyd  d10, d0                  \n\t"
		"fcpyd  d11, d0                  \n\t"
		"fcpyd  d12, d0                  \n\t"
		"fcpyd  d13, d0                  \n\t"
		"fcpyd  d14, d0                  \n\t"
		"fcpyd  d15, d0                  \n\t"
		"                                \n\t"
		"                                \n\t"
		".align 3                        \n\t"
		".DOUBLEZERO:                    \n\t" // zero double word
		".word  0                        \n\t"
		".word  0                        \n\t"
		"                                \n\t"
		"                                \n\t"
		"ble    .DCONSIDERADD            \n\t"
		"                                \n\t"
		"                                \n\t"
		".DLOOPADD:                      \n\t" // main loop
		"                                \n\t"
		"                                \n\t"
		"pld    [%3, #128]               \n\t"
		"pld    [%4, #128]               \n\t"
		"                                \n\t"
		"                                \n\t"
		"fmacd  d0, d16, d20             \n\t"
		"fmacd  d1, d17, d20             \n\t"
		"fmacd  d2, d18, d20             \n\t"
		"fmacd  d3, d19, d20             \n\t"
		"fldd   d20, [%4, #64]           \n\t" // prefetch B_even
		"                                \n\t"
		"fmacd  d4, d16, d21             \n\t"
		"fmacd  d5, d17, d21             \n\t"
		"fmacd  d6, d18, d21             \n\t"
		"fmacd  d7, d19, d21             \n\t"
		"fldd   d21, [%4, #72]           \n\t"
		"                                \n\t"
		"fmacd  d8, d16, d22             \n\t"
		"fmacd  d9, d17, d22             \n\t"
		"fmacd  d10, d18, d22            \n\t"
		"fmacd  d11, d19, d22            \n\t"
		"fldd   d22, [%4, #80]           \n\t"
		"                                \n\t"
		"fmacd  d12, d16, d23            \n\t"
		"fldd   d16, [%3, #64]           \n\t" // prefetch A_even
		"fmacd  d13, d17, d23            \n\t"
		"fldd   d17, [%3, #72]           \n\t"
		"fmacd  d14, d18, d23            \n\t"
		"fldd   d18, [%3, #80]           \n\t"
		"fmacd  d15, d19, d23            \n\t"
		"fldd   d23, [%4, #88]           \n\t"
		"                                \n\t"
		"                                \n\t"
#if defined(CORTEX_A9)
		"pld    [%3, #160]               \n\t"
		"pld    [%4, #160]               \n\t"
#endif
		"                                \n\t"
		"                                \n\t"
		"fmacd  d0, d24, d28             \n\t"
		"fldd   d19, [%3, #88]           \n\t"
		"fmacd  d1, d25, d28             \n\t"
		"sub    r3, r3, #1               \n\t" // iter++
		"fmacd  d2, d26, d28             \n\t"
		"fmacd  d3, d27, d28             \n\t"
		"fldd   d28, [%4, #96]           \n\t" // prefetch B_odd
		"                                \n\t"
		"fmacd  d4, d24, d29             \n\t"
		"fmacd  d5, d25, d29             \n\t"
		"fmacd  d6, d26, d29             \n\t"
		"fmacd  d7, d27, d29             \n\t"
		"fldd   d29, [%4, #104]          \n\t"
		"                                \n\t"
		"fmacd  d8, d24, d30             \n\t"
		"fmacd  d9, d25, d30             \n\t"
		"cmp    r3, #0                   \n\t" // next iter?
		"fmacd  d10, d26, d30            \n\t"
		"fmacd  d11, d27, d30            \n\t"
		"fldd   d30, [%4, #112]          \n\t"
		"                                \n\t"
		"fmacd  d12, d24, d31            \n\t"
		"fldd   d24, [%3, #96]           \n\t" // prefetch A_odd
		"fmacd  d13, d25, d31            \n\t"
		"fldd   d25, [%3, #104]          \n\t"
		"fmacd  d14, d26, d31            \n\t"
		"fldd   d26, [%3, #112]          \n\t"
		"fmacd  d15, d27, d31            \n\t"
		"fldd   d31, [%4, #120]          \n\t"
		"fldd   d27, [%3, #120]          \n\t"
		"add    %3, %3, #64              \n\t" // increase A
		"add    %4, %4, #64              \n\t" // increase B
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"bgt    .DLOOPADD                \n\t"
		"                                \n\t"
		"                                \n\t"
		".DCONSIDERADD:                  \n\t" // consider left
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"mov    r3, %1                   \n\t" // k_left
		"cmp    r3, #0                   \n\t"
		"ble    .DCONSIDERSUB            \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		".DLOOPLEFT:                     \n\t" // clean up loop
		"                                \n\t"
		"sub    r3, r3, #1               \n\t"
		"                                \n\t"
		"fmacd  d0, d16, d20             \n\t"
		"fmacd  d1, d17, d20             \n\t"
		"fmacd  d2, d18, d20             \n\t"
		"fmacd  d3, d19, d20             \n\t"
		"fldd   d20, [%4, #32]           \n\t" // prefetch B_even
		"                                \n\t"
		"fmacd  d4, d16, d21             \n\t"
		"fmacd  d5, d17, d21             \n\t"
		"fmacd  d6, d18, d21             \n\t"
		"fmacd  d7, d19, d21             \n\t"
		"fldd   d21, [%4, #40]           \n\t"
		"                                \n\t"
		"fmacd  d8, d16, d22             \n\t"
		"fmacd  d9, d17, d22             \n\t"
		"fmacd  d10, d18, d22            \n\t"
		"fmacd  d11, d19, d22            \n\t"
		"fldd   d22, [%4, #48]           \n\t"
		"                                \n\t"
		"cmp    r3, #0                   \n\t"
		"                                \n\t"
		"fmacd  d12, d16, d23            \n\t"
		"fldd   d16, [%3, #32]           \n\t" // prefetch A_even
		"fmacd  d13, d17, d23            \n\t"
		"fldd   d17, [%3, #40]           \n\t"
		"fmacd  d14, d18, d23            \n\t"
		"fldd   d18, [%3, #48]           \n\t"
		"fmacd  d15, d19, d23            \n\t"
		"fldd   d19, [%3, #56]           \n\t"
		"add    %3, %3, #32              \n\t"
		"fldd   d23, [%4, #56]           \n\t"
		"add    %4, %4, #32              \n\t"
		"                                \n\t"
		"                                \n\t"
		"bgt    .DLOOPLEFT               \n\t"
		"                                \n\t"
		"                                \n\t"
		".DCONSIDERSUB:                  \n\t" // padd
		"                                \n\t"
		"add    r3, %0, %1               \n\t"
		"add    r3, r3, %8               \n\t"
		"                                \n\t"
		"cmp    %2, #0                   \n\t"
		"ble    .DPOSTACC                \n\t"
		"                                \n\t"
		"cmp    r3, #0                   \n\t"
		"ble    .DPRELOOPSUB             \n\t"
		"                                \n\t"
		"add    %3, %3, %8               \n\t"
		"add    %4, %4, %8               \n\t"
		"                                \n\t"
		"fldd   d16, [%3, #0]            \n\t" // prefetch A_even
		"fldd   d17, [%3, #8]            \n\t"
		"fldd   d18, [%3, #16]           \n\t"
		"fldd   d19, [%3, #24]           \n\t"
		"                                \n\t"
		"fldd   d20, [%4, #0]            \n\t" // prefetch B_even
		"fldd   d21, [%4, #8]            \n\t"
		"fldd   d22, [%4, #16]           \n\t"
		"fldd   d23, [%4, #24]           \n\t"
		"                                \n\t"
		".DPRELOOPSUB:                   \n\t" // 
		"                                \n\t"
		"mov    r3, %2                   \n\t" // k_iter
		"                                \n\t"
		"fldd   d24, [%3, #32]           \n\t" // prefetch A_odd
		"fldd   d25, [%3, #40]           \n\t"
		"fldd   d26, [%3, #48]           \n\t"
		"fldd   d27, [%3, #56]           \n\t"
		"                                \n\t"
		"fldd   d28, [%4, #32]           \n\t" // prefetch B_odd
		"fldd   d29, [%4, #40]           \n\t"
		"fldd   d30, [%4, #48]           \n\t"
		"fldd   d31, [%4, #56]           \n\t"
		"                                \n\t"
		".DLOOPSUB:                      \n\t" // main loop 2
		"                                \n\t"
		"                                \n\t"
		"pld    [%3, #128]               \n\t"
		"pld    [%4, #128]               \n\t"
		"                                \n\t"
		"                                \n\t"
		"fnmacd  d0, d16, d20             \n\t"
		"fnmacd  d1, d17, d20             \n\t"
		"fnmacd  d2, d18, d20             \n\t"
		"fnmacd  d3, d19, d20             \n\t"
		"fldd   d20, [%4, #64]           \n\t" // prefetch B_even
		"                                \n\t"
		"fnmacd  d4, d16, d21             \n\t"
		"fnmacd  d5, d17, d21             \n\t"
		"fnmacd  d6, d18, d21             \n\t"
		"fnmacd  d7, d19, d21             \n\t"
		"fldd   d21, [%4, #72]           \n\t"
		"                                \n\t"
		"fnmacd  d8, d16, d22             \n\t"
		"fnmacd  d9, d17, d22             \n\t"
		"fnmacd  d10, d18, d22            \n\t"
		"fnmacd  d11, d19, d22            \n\t"
		"fldd   d22, [%4, #80]           \n\t"
		"                                \n\t"
		"fnmacd  d12, d16, d23            \n\t"
		"fldd   d16, [%3, #64]           \n\t" // prefetch A_even
		"fnmacd  d13, d17, d23            \n\t"
		"fldd   d17, [%3, #72]           \n\t"
		"fnmacd  d14, d18, d23            \n\t"
		"fldd   d18, [%3, #80]           \n\t"
		"fnmacd  d15, d19, d23            \n\t"
		"fldd   d23, [%4, #88]           \n\t"
		"                                \n\t"
		"                                \n\t"
#if defined(CORTEX_A9)
		"pld    [%3, #160]               \n\t"
		"pld    [%4, #160]               \n\t"
#endif
		"                                \n\t"
		"                                \n\t"
		"fnmacd  d0, d24, d28             \n\t"
		"fldd   d19, [%3, #88]           \n\t"
		"fnmacd  d1, d25, d28             \n\t"
		"sub    r3, r3, #1               \n\t" // iter++
		"fnmacd  d2, d26, d28             \n\t"
		"fnmacd  d3, d27, d28             \n\t"
		"fldd   d28, [%4, #96]           \n\t" // prefetch B_odd
		"                                \n\t"
		"fnmacd  d4, d24, d29             \n\t"
		"fnmacd  d5, d25, d29             \n\t"
		"fnmacd  d6, d26, d29             \n\t"
		"fnmacd  d7, d27, d29             \n\t"
		"fldd   d29, [%4, #104]          \n\t"
		"                                \n\t"
		"fnmacd  d8, d24, d30             \n\t"
		"fnmacd  d9, d25, d30             \n\t"
		"cmp    r3, #0                   \n\t" // next iter?
		"fnmacd  d10, d26, d30            \n\t"
		"fnmacd  d11, d27, d30            \n\t"
		"fldd   d30, [%4, #112]          \n\t"
		"                                \n\t"
		"fnmacd  d12, d24, d31            \n\t"
		"fldd   d24, [%3, #96]           \n\t" // prefetch A_odd
		"fnmacd  d13, d25, d31            \n\t"
		"fldd   d25, [%3, #104]          \n\t"
		"fnmacd  d14, d26, d31            \n\t"
		"fldd   d26, [%3, #112]          \n\t"
		"fnmacd  d15, d27, d31            \n\t"
		"fldd   d31, [%4, #120]          \n\t"
		"fldd   d27, [%3, #120]          \n\t"
		"add    %3, %3, #64             \n\t" // increase A
		"add    %4, %4, #64             \n\t" // increase B
		"                                \n\t"
		"                                \n\t"
		"bgt    .DLOOPSUB                \n\t"
		"                                \n\t"
		"                                \n\t"
		".DPOSTACC:                      \n\t"
		"                                \n\t"
		"                                \n\t"
		"fldd   d16, [%5, #0]            \n\t" // load C elements
		"fldd   d17, [%5, #8]            \n\t"
		"fldd   d18, [%5, #16]           \n\t"
		"fldd   d19, [%5, #24]           \n\t"
		"                                \n\t"
		"fldd   d20, [%5, #32]           \n\t"
		"fldd   d21, [%5, #40]           \n\t"
		"fldd   d22, [%5, #48]           \n\t"
		"fldd   d23, [%5, #56]           \n\t"
		"                                \n\t"
		"fldd   d24, [%5, #64]           \n\t"
		"fldd   d25, [%5, #72]           \n\t"
		"fldd   d26, [%5, #80]           \n\t"
		"fldd   d27, [%5, #88]           \n\t"
		"                                \n\t"
		"fldd   d28, [%5, #96]           \n\t"
		"fldd   d29, [%5, #104]          \n\t"
		"fldd   d30, [%5, #112]          \n\t"
		"fldd   d31, [%5, #120]          \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"faddd  d0, d0, d16              \n\t"
		"faddd  d1, d1, d17              \n\t"
		"faddd  d2, d2, d18              \n\t"
		"faddd  d3, d3, d19              \n\t"
		"                                \n\t"
		"faddd  d4, d4, d20              \n\t"
		"faddd  d5, d5, d21              \n\t"
		"faddd  d6, d6, d22              \n\t"
		"faddd  d7, d7, d23              \n\t"
		"                                \n\t"
		"faddd  d8, d8, d24              \n\t"
		"faddd  d9, d9, d25              \n\t"
		"faddd  d10, d10, d26            \n\t"
		"faddd  d11, d11, d27            \n\t"
		"                                \n\t"
		"faddd  d12, d12, d28            \n\t"
		"faddd  d13, d13, d29            \n\t"
		"faddd  d14, d14, d30            \n\t"
		"faddd  d15, d15, d31            \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"fldd   d16, [%7, #0]            \n\t" // load fact elements
		"fldd   d17, [%7, #8]            \n\t"
		"fldd   d18, [%7, #16]           \n\t"
		"fldd   d19, [%7, #24]           \n\t"
		"fldd   d20, [%7, #32]           \n\t"
		"fldd   d21, [%7, #40]           \n\t"
		"fldd   d22, [%7, #48]           \n\t"
		"fldd   d23, [%7, #56]           \n\t"
		"fldd   d24, [%7, #64]           \n\t"
		"fldd   d25, [%7, #72]           \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"fmuld  d0, d0, d16              \n\t"
		"fmuld  d1, d1, d16              \n\t"
		"fmuld  d2, d2, d16              \n\t"
		"fmuld  d3, d3, d16              \n\t"
		"                                \n\t"
		"fstd   d0, [%6, #0]             \n\t" // store result
		"fstd   d1, [%6, #8]             \n\t"
		"fstd   d2, [%6, #16]            \n\t"
		"fstd   d3, [%6, #24]            \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"fnmacd d4, d0, d17              \n\t"
		"fnmacd d5, d1, d17              \n\t"
		"fnmacd d6, d2, d17              \n\t"
		"fnmacd d7, d3, d17              \n\t"
		"                                \n\t"
		"fmuld  d4, d4, d18              \n\t"
		"fmuld  d5, d5, d18              \n\t"
		"fmuld  d6, d6, d18              \n\t"
		"fmuld  d7, d7, d18              \n\t"
		"                                \n\t"
		"fstd   d4, [%6, #32]            \n\t"
		"fstd   d5, [%6, #40]            \n\t"
		"fstd   d6, [%6, #48]            \n\t"
		"fstd   d7, [%6, #56]            \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"fnmacd d8, d0, d19              \n\t"
		"fnmacd d9, d1, d19              \n\t"
		"fnmacd d10, d2, d19             \n\t"
		"fnmacd d11, d3, d19             \n\t"
		"                                \n\t"
		"fnmacd d8, d4, d20              \n\t"
		"fnmacd d9, d5, d20              \n\t"
		"fnmacd d10, d6, d20             \n\t"
		"fnmacd d11, d7, d20             \n\t"
		"                                \n\t"
		"fmuld  d8, d8, d21              \n\t"
		"fmuld  d9, d9, d21              \n\t"
		"fmuld  d10, d10, d21            \n\t"
		"fmuld  d11, d11, d21            \n\t"
		"                                \n\t"
		"fstd   d8, [%6, #64]            \n\t"
		"fstd   d9, [%6, #72]            \n\t"
		"fstd   d10, [%6, #80]           \n\t"
		"fstd   d11, [%6, #88]           \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"fnmacd d12, d0, d22             \n\t"
		"fnmacd d13, d1, d22             \n\t"
		"fnmacd d14, d2, d22             \n\t"
		"fnmacd d15, d3, d22             \n\t"
		"                                \n\t"
		"fnmacd d12, d4, d23             \n\t"
		"fnmacd d13, d5, d23             \n\t"
		"fnmacd d14, d6, d23             \n\t"
		"fnmacd d15, d7, d23             \n\t"
		"                                \n\t"
		"fnmacd d12, d8, d24             \n\t"
		"fnmacd d13, d9, d24             \n\t"
		"fnmacd d14, d10, d24            \n\t"
		"fnmacd d15, d11, d24            \n\t"
		"                                \n\t"
		"fmuld  d12, d12, d25            \n\t"
		"fmuld  d13, d13, d25            \n\t"
		"fmuld  d14, d14, d25            \n\t"
		"fmuld  d15, d15, d25            \n\t"
		"                                \n\t"
		"fstd   d12, [%6, #96]           \n\t"
		"fstd   d13, [%6, #104]          \n\t"
		"fstd   d14, [%6, #112]          \n\t"
		"fstd   d15, [%6, #120]          \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		"                                \n\t"
		: // output operands (none)
		: // input operands
		  "r" (ki_add),		// %0
		  "r" (kl_add),		// %1
		  "r" (ki_sub),		// %2
		  "r" (A),			// %3
		  "r" (B),			// %4
		  "r" (C),			// %5
		  "r" (D),			// %6
		  "r" (fact),		// %7
		  "r" (dA)			// %8
		: // register clobber list
		  "r3",
		  "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
		  "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15",
		  "d16", "d17", "d18", "d19", "d20", "d21", "d22", "d23",
		  "d24", "d25", "d26", "d27", "d28", "d29", "d30", "d31",
		  "memory"
	);
}



void kernel_dtrsm_pp_nt_4x2_lib4(int kadd, int ksub, double *A, double *B, double *C, double *D, int ldc, double *fact)
	{

	const int bs = 4;
	const int d_ncl = 2;
	const int lda = bs;

	int k;

	double
		a_0, a_1, a_2, a_3,
		b_0, b_1,
		c_00=0, c_01=0,
		c_10=0, c_11=0,
		c_20=0, c_21=0,
		c_30=0, c_31=0;
		
	for(k=0; k<kadd-3; k+=4)
		{
		
		a_0 = A[0+lda*0];
		a_1 = A[1+lda*0];
		a_2 = A[2+lda*0];
		a_3 = A[3+lda*0];
		
		b_0 = B[0+lda*0];
		b_1 = B[1+lda*0];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;
		c_20 += a_2 * b_0;
		c_30 += a_3 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;
		c_21 += a_2 * b_1;
		c_31 += a_3 * b_1;


		a_0 = A[0+lda*1];
		a_1 = A[1+lda*1];
		a_2 = A[2+lda*1];
		a_3 = A[3+lda*1];
		
		b_0 = B[0+lda*1];
		b_1 = B[1+lda*1];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;
		c_20 += a_2 * b_0;
		c_30 += a_3 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;
		c_21 += a_2 * b_1;
		c_31 += a_3 * b_1;


		a_0 = A[0+lda*2];
		a_1 = A[1+lda*2];
		a_2 = A[2+lda*2];
		a_3 = A[3+lda*2];
		
		b_0 = B[0+lda*2];
		b_1 = B[1+lda*2];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;
		c_20 += a_2 * b_0;
		c_30 += a_3 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;
		c_21 += a_2 * b_1;
		c_31 += a_3 * b_1;


		a_0 = A[0+lda*3];
		a_1 = A[1+lda*3];
		a_2 = A[2+lda*3];
		a_3 = A[3+lda*3];
		
		b_0 = B[0+lda*3];
		b_1 = B[1+lda*3];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;
		c_20 += a_2 * b_0;
		c_30 += a_3 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;
		c_21 += a_2 * b_1;
		c_31 += a_3 * b_1;
		
		
		A += 16;
		B += 16;

		}
	for(; k<kadd; k++)
		{
		
		a_0 = A[0+lda*0];
		a_1 = A[1+lda*0];
		a_2 = A[2+lda*0];
		a_3 = A[3+lda*0];
		
		b_0 = B[0+lda*0];
		b_1 = B[1+lda*0];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;
		c_20 += a_2 * b_0;
		c_30 += a_3 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;
		c_21 += a_2 * b_1;
		c_31 += a_3 * b_1;


		A += 4;
		B += 4;

		}

	if(ksub>0)
		{
		if(kadd>0)
			{
			A += bs*((d_ncl-kadd%d_ncl)%d_ncl);
			B += bs*((d_ncl-kadd%d_ncl)%d_ncl);
			}
		}

	for(k=0; k<ksub-3; k+=4)
		{
		
		a_0 = A[0+lda*0];
		a_1 = A[1+lda*0];
		a_2 = A[2+lda*0];
		a_3 = A[3+lda*0];
		
		b_0 = B[0+lda*0];
		b_1 = B[1+lda*0];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;
		c_20 -= a_2 * b_0;
		c_30 -= a_3 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;
		c_21 -= a_2 * b_1;
		c_31 -= a_3 * b_1;


		a_0 = A[0+lda*1];
		a_1 = A[1+lda*1];
		a_2 = A[2+lda*1];
		a_3 = A[3+lda*1];
		
		b_0 = B[0+lda*1];
		b_1 = B[1+lda*1];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;
		c_20 -= a_2 * b_0;
		c_30 -= a_3 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;
		c_21 -= a_2 * b_1;
		c_31 -= a_3 * b_1;


		a_0 = A[0+lda*2];
		a_1 = A[1+lda*2];
		a_2 = A[2+lda*2];
		a_3 = A[3+lda*2];
		
		b_0 = B[0+lda*2];
		b_1 = B[1+lda*2];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;
		c_20 -= a_2 * b_0;
		c_30 -= a_3 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;
		c_21 -= a_2 * b_1;
		c_31 -= a_3 * b_1;


		a_0 = A[0+lda*3];
		a_1 = A[1+lda*3];
		a_2 = A[2+lda*3];
		a_3 = A[3+lda*3];
		
		b_0 = B[0+lda*3];
		b_1 = B[1+lda*3];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;
		c_20 -= a_2 * b_0;
		c_30 -= a_3 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;
		c_21 -= a_2 * b_1;
		c_31 -= a_3 * b_1;
		
		
		A += 16;
		B += 16;

		}

	c_00 += C[0+ldc*0];
	c_10 += C[1+ldc*0];
	c_20 += C[2+ldc*0];
	c_30 += C[3+ldc*0];

	c_01 += C[0+ldc*1];
	c_11 += C[1+ldc*1];
	c_21 += C[2+ldc*1];
	c_31 += C[3+ldc*1];
	
	// dtrsm
	double
		a_00, a_10, a_11;
	
	a_00 = fact[0];
	c_00 *= a_00;
	c_10 *= a_00;
	c_20 *= a_00;
	c_30 *= a_00;
	D[0+ldc*0] = c_00;
	D[1+ldc*0] = c_10;
	D[2+ldc*0] = c_20;
	D[3+ldc*0] = c_30;

	a_10 = fact[1];
	a_11 = fact[2];
	c_01 -= c_00*a_10;
	c_11 -= c_10*a_10;
	c_21 -= c_20*a_10;
	c_31 -= c_30*a_10;
	c_01 *= a_11;
	c_11 *= a_11;
	c_21 *= a_11;
	c_31 *= a_11;
	D[0+ldc*1] = c_01;
	D[1+ldc*1] = c_11;
	D[2+ldc*1] = c_21;
	D[3+ldc*1] = c_31;

	}
	
	
	
void kernel_dtrsm_pp_nt_2x4_lib4(int kadd, int ksub, double *A, double *B, double *C, double *D, int ldc, double *fact)
	{

	const int bs = 4;
	const int d_ncl = 2;
	const int lda = bs;

	int k;

	double
		a_0, a_1,
		b_0, b_1, b_2, b_3,
		c_00=0, c_01=0, c_02=0, c_03=0,
		c_10=0, c_11=0, c_12=0, c_13=0;
		
	for(k=0; k<kadd-3; k+=4)
		{
		
		a_0 = A[0+lda*0];
		a_1 = A[1+lda*0];
		
		b_0 = B[0+lda*0];
		b_1 = B[1+lda*0];
		b_2 = B[2+lda*0];
		b_3 = B[3+lda*0];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;

		c_02 += a_0 * b_2;
		c_12 += a_1 * b_2;

		c_03 += a_0 * b_3;
		c_13 += a_1 * b_3;


		a_0 = A[0+lda*1];
		a_1 = A[1+lda*1];
		
		b_0 = B[0+lda*1];
		b_1 = B[1+lda*1];
		b_2 = B[2+lda*1];
		b_3 = B[3+lda*1];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;

		c_02 += a_0 * b_2;
		c_12 += a_1 * b_2;

		c_03 += a_0 * b_3;
		c_13 += a_1 * b_3;


		a_0 = A[0+lda*2];
		a_1 = A[1+lda*2];
		
		b_0 = B[0+lda*2];
		b_1 = B[1+lda*2];
		b_2 = B[2+lda*2];
		b_3 = B[3+lda*2];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;

		c_02 += a_0 * b_2;
		c_12 += a_1 * b_2;

		c_03 += a_0 * b_3;
		c_13 += a_1 * b_3;


		a_0 = A[0+lda*3];
		a_1 = A[1+lda*3];
		
		b_0 = B[0+lda*3];
		b_1 = B[1+lda*3];
		b_2 = B[2+lda*3];
		b_3 = B[3+lda*3];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;

		c_02 += a_0 * b_2;
		c_12 += a_1 * b_2;

		c_03 += a_0 * b_3;
		c_13 += a_1 * b_3;
		
		
		A += 16;
		B += 16;

		}
	for(; k<kadd; k++)
		{
		
		a_0 = A[0+lda*0];
		a_1 = A[1+lda*0];
		
		b_0 = B[0+lda*0];
		b_1 = B[1+lda*0];
		b_2 = B[2+lda*0];
		b_3 = B[3+lda*0];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;

		c_02 += a_0 * b_2;
		c_12 += a_1 * b_2;

		c_03 += a_0 * b_3;
		c_13 += a_1 * b_3;


		A += 4;
		B += 4;

		}

	if(ksub>0)
		{
		if(kadd>0)
			{
			A += bs*((d_ncl-kadd%d_ncl)%d_ncl);
			B += bs*((d_ncl-kadd%d_ncl)%d_ncl);
			}
		}

	for(k=0; k<ksub-3; k+=4)
		{
		
		a_0 = A[0+lda*0];
		a_1 = A[1+lda*0];
		
		b_0 = B[0+lda*0];
		b_1 = B[1+lda*0];
		b_2 = B[2+lda*0];
		b_3 = B[3+lda*0];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;

		c_02 -= a_0 * b_2;
		c_12 -= a_1 * b_2;

		c_03 -= a_0 * b_3;
		c_13 -= a_1 * b_3;


		a_0 = A[0+lda*1];
		a_1 = A[1+lda*1];
		
		b_0 = B[0+lda*1];
		b_1 = B[1+lda*1];
		b_2 = B[2+lda*1];
		b_3 = B[3+lda*1];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;

		c_02 -= a_0 * b_2;
		c_12 -= a_1 * b_2;

		c_03 -= a_0 * b_3;
		c_13 -= a_1 * b_3;


		a_0 = A[0+lda*2];
		a_1 = A[1+lda*2];
		
		b_0 = B[0+lda*2];
		b_1 = B[1+lda*2];
		b_2 = B[2+lda*2];
		b_3 = B[3+lda*2];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;

		c_02 -= a_0 * b_2;
		c_12 -= a_1 * b_2;

		c_03 -= a_0 * b_3;
		c_13 -= a_1 * b_3;


		a_0 = A[0+lda*3];
		a_1 = A[1+lda*3];
		
		b_0 = B[0+lda*3];
		b_1 = B[1+lda*3];
		b_2 = B[2+lda*3];
		b_3 = B[3+lda*3];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;

		c_02 -= a_0 * b_2;
		c_12 -= a_1 * b_2;

		c_03 -= a_0 * b_3;
		c_13 -= a_1 * b_3;
		
		
		A += 16;
		B += 16;

		}

	c_00 += C[0+ldc*0];
	c_10 += C[1+ldc*0];

	c_01 += C[0+ldc*1];
	c_11 += C[1+ldc*1];

	c_02 += C[0+ldc*2];
	c_12 += C[1+ldc*2];

	c_03 += C[0+ldc*3];
	c_13 += C[1+ldc*3];
	
	// dtrsm
	double
		a_00, a_10, a_20, a_30, a_11, a_21, a_31, a_22, a_32, a_33;
	
	a_00 = fact[0];
	c_00 *= a_00;
	c_10 *= a_00;
	D[0+ldc*0] = c_00;
	D[1+ldc*0] = c_10;

	a_10 = fact[1];
	a_11 = fact[2];
	c_01 -= c_00*a_10;
	c_11 -= c_10*a_10;
	c_01 *= a_11;
	c_11 *= a_11;
	D[0+ldc*1] = c_01;
	D[1+ldc*1] = c_11;

	a_20 = fact[3];
	a_21 = fact[4];
	a_22 = fact[5];
	c_02 -= c_00*a_20;
	c_12 -= c_10*a_20;
	c_02 -= c_01*a_21;
	c_12 -= c_11*a_21;
	c_02 *= a_22;
	c_12 *= a_22;
	D[0+ldc*2] = c_02;
	D[1+ldc*2] = c_12;

	a_30 = fact[6];
	a_31 = fact[7];
	a_32 = fact[8];
	a_33 = fact[9];
	c_03 -= c_00*a_30;
	c_13 -= c_10*a_30;
	c_03 -= c_01*a_31;
	c_13 -= c_11*a_31;
	c_03 -= c_02*a_32;
	c_13 -= c_12*a_32;
	c_03 *= a_33;
	c_13 *= a_33;
	D[0+ldc*3] = c_03;
	D[1+ldc*3] = c_13;

	}
	
	
	
void kernel_dtrsm_pp_nt_2x2_lib4(int kadd, int ksub, double *A, double *B, double *C, double *D, int ldc, double *fact)
	{

	const int bs = 4;
	const int d_ncl = 2;
	const int lda = bs;

	int k;

	double
		a_0, a_1,
		b_0, b_1,
		c_00=0, c_01=0,
		c_10=0, c_11=0;
		
	for(k=0; k<kadd-3; k+=4)
		{
		
		a_0 = A[0+lda*0];
		a_1 = A[1+lda*0];
		
		b_0 = B[0+lda*0];
		b_1 = B[1+lda*0];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;


		a_0 = A[0+lda*1];
		a_1 = A[1+lda*1];
		
		b_0 = B[0+lda*1];
		b_1 = B[1+lda*1];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;


		a_0 = A[0+lda*2];
		a_1 = A[1+lda*2];
		
		b_0 = B[0+lda*2];
		b_1 = B[1+lda*2];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;


		a_0 = A[0+lda*3];
		a_1 = A[1+lda*3];
		
		b_0 = B[0+lda*3];
		b_1 = B[1+lda*3];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;
		
		
		A += 16;
		B += 16;

		}
	for(; k<kadd; k++)
		{
		
		a_0 = A[0+lda*0];
		a_1 = A[1+lda*0];
		
		b_0 = B[0+lda*0];
		b_1 = B[1+lda*0];
		
		c_00 += a_0 * b_0;
		c_10 += a_1 * b_0;

		c_01 += a_0 * b_1;
		c_11 += a_1 * b_1;


		A += 4;
		B += 4;

		}

	if(ksub>0)
		{
		if(kadd>0)
			{
			A += bs*((d_ncl-kadd%d_ncl)%d_ncl);
			B += bs*((d_ncl-kadd%d_ncl)%d_ncl);
			}
		}

	for(k=0; k<ksub-3; k+=4)
		{
		
		a_0 = A[0+lda*0];
		a_1 = A[1+lda*0];
		
		b_0 = B[0+lda*0];
		b_1 = B[1+lda*0];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;


		a_0 = A[0+lda*1];
		a_1 = A[1+lda*1];
		
		b_0 = B[0+lda*1];
		b_1 = B[1+lda*1];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;


		a_0 = A[0+lda*2];
		a_1 = A[1+lda*2];
		
		b_0 = B[0+lda*2];
		b_1 = B[1+lda*2];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;


		a_0 = A[0+lda*3];
		a_1 = A[1+lda*3];
		
		b_0 = B[0+lda*3];
		b_1 = B[1+lda*3];
		
		c_00 -= a_0 * b_0;
		c_10 -= a_1 * b_0;

		c_01 -= a_0 * b_1;
		c_11 -= a_1 * b_1;
		
		
		A += 16;
		B += 16;

		}

	c_00 += C[0+ldc*0];
	c_10 += C[1+ldc*0];

	c_01 += C[0+ldc*1];
	c_11 += C[1+ldc*1];
	
	// dtrsm
	double
		a_00, a_10, a_11;
	
	a_00 = fact[0];
	c_00 *= a_00;
	c_10 *= a_00;
	D[0+ldc*0] = c_00;
	D[1+ldc*0] = c_10;

	a_10 = fact[1];
	a_11 = fact[2];
	c_01 -= c_00*a_10;
	c_11 -= c_10*a_10;
	c_01 *= a_11;
	c_11 *= a_11;
	D[0+ldc*1] = c_01;
	D[1+ldc*1] = c_11;

	}
	
	
	


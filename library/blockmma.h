//////////////////////////////////////////////////////////////////////
//                      University of California, Riverside
//
//
//
//                             Copyright 2022
//
////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
//
////////////////////////////////////////////////////////////////////////
//
//   Author:  Hung-Wei Tseng, Yu-Chia Liu
//
//   Description:
//     Header File of CSE202's Transparent RCU Project in User Space
//
////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

#include <blockmma/blockmma.h>
#include <linux/types.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int blockmma(int devfd, float *a, float *b, float *c, int m, int n, int k);
int blockmma_bonus(int devfd, float *a, float *b, float *c, int m, int n, int k);
int blockmma_f128(int devfd, float *a, float *b, float *c, int m, int n, int k, int tile);
int blockmma_sync(int devfd);
int blockmma_f128_accelerator(int devfd);
//int blockmma_free(int devfd, __u64 offset);
    
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////
//                     University of California, Riverside
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
//     APIs of CSE202's Emulated MMA Accelerators in User Space
//
////////////////////////////////////////////////////////////////////////
#include "blockmma.h"

// This function submit tasks to the kernel module.
int blockmma_f128(int devfd, float *a, float *b, float *c, int m, int n, int k, int tile)
{
    struct blockmma_cmd cmd;
    cmd.op = (__u64)0;
    cmd.a = (__u64)a;
    cmd.b = (__u64)b;
    cmd.c = (__u64)c;
    cmd.m = (__u64)m;
    cmd.n = (__u64)n;
    cmd.k = (__u64)k;
    cmd.tile = (__u64)tile;
    while(ioctl(devfd, BLOCKMMA_IOCTL_SEND_TASK, &cmd)== -1);
    return 0;
}

// This function will wait until all updates are presented to the userspace from 
// the kernel module.
int blockmma_sync(int devfd)
{
    struct blockmma_cmd cmd;
    cmd.op = (__u64)0;
    while(ioctl(devfd, BLOCKMMA_IOCTL_SYNC, &cmd) == -1);
    return 0;
}

int counter = 0;
void sigquit();

// This is the function emulating the effect of hardware accelerated 128x128 
// matrix multiplications.
int blockmma_f128_accelerator(int devfd)
{
    struct blockmma_hardware_cmd cmd;
    float *a, *b, *c;
    int i, j, k;
    int tid;
    a = (float *)malloc(128*128*sizeof(float));
    b = (float *)malloc(128*128*sizeof(float));
    c = (float *)malloc(128*128*sizeof(float));
    cmd.op = (__u64)0;
    cmd.a = (__u64)a;
    cmd.b = (__u64)b;
    cmd.c = (__u64)c;
    signal(SIGQUIT, sigquit);
    while(1)
    {
        if((tid=ioctl(devfd, BLOCKMMA_IOCTL_GET_TASK, &cmd))>=0)
        {
            for(i = 0; i < 128; i++)
                for(j = 0; j < 128; j++)
                    for(k = 0; k < 128; k++)
                        c[i*128+j] += a[i*128+k]*b[k*128+j];
            cmd.tid = tid;
            ioctl(devfd, BLOCKMMA_IOCTL_COMP, &cmd);
            counter++;
        }
    }
    exit(1);
    return 0;
}

void sigquit()
{
    printf("Accelerator %d has performed %d tasks\n", getpid(), counter);
    signal(SIGQUIT, sigquit);
    exit(0);
}
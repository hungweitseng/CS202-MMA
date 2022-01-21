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
//     User APIs of CSE202's Block MMA Accelerator Project in User Space
//
////////////////////////////////////////////////////////////////////////


#ifndef RESOURCE_CONTAINER_H
#define RESOURCE_CONTAINER_H

#include <linux/types.h>


struct blockmma_hardware_cmd {
    __u64 op;  // OP: FMMA_128 - 0; IMMA_128 - 1
    __u64 tid;
    __u64 a;
    __u64 b;
    __u64 c;
};

struct blockmma_cmd {
    __u64 op;  // OP: FMMA_128 - 0; IMMA_128 - 1
    __u64 tid;
    __u64 a;
    __u64 b;
    __u64 c;
    __u64 m; // # of Rows in A and # of Cols in C
    __u64 n; // # of Cols in A and # of Rows in B
    __u64 k; // # of Cols in B and # of Cols in C
    __u64 tile; // # of elements in each Row/col of the matrix tile
};

#define BLOCKMMA_IOCTL_AUTHOR _IOWR('N', 0x44, struct blockmma_hardware_cmd)
#define BLOCKMMA_IOCTL_SYNC _IOWR('N', 0x45, struct blockmma_cmd)
#define BLOCKMMA_IOCTL_SEND_TASK  _IOWR('N', 0x46, struct blockmma_cmd)
#define BLOCKMMA_IOCTL_GET_TASK  _IOWR('N', 0x47, struct blockmma_hardware_cmd)
#define BLOCKMMA_IOCTL_COMP _IOWR('N', 0x48, struct blockmma_hardware_cmd)

#endif

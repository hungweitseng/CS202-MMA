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
//     The Interface of the Kernel Module for CSE202's Transparent RCU Project
//
////////////////////////////////////////////////////////////////////////

#include "blockmma.h"

#include <asm/segment.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/mutex.h>

/**
 * Enqueue a task for the caller/accelerator to perform computation.
 */

long blockmma_send_task(struct blockmma_cmd __user *user_cmd)
{
    return 0;
}

/**
 * Return until all outstanding tasks for the calling process are done
 */
int blockmma_sync(struct blockmma_cmd __user *user_cmd)
{
    return 0;
}

/**
 * Return the task id of a task to the caller/accelerator to perform computation.
 */
int blockmma_get_task(struct blockmma_hardware_cmd __user *user_cmd)
{
    return 0;
}


/**
 * Return until the task specified in the command is done.
 */
int blockmma_comp(struct blockmma_hardware_cmd __user *user_cmd)
{
    return 0;
}

/**
 * control function that receive the command in user space and pass arguments to
 * corresponding functions.
 */
int blockmma_ioctl(struct file *filp, unsigned int cmd,
                                unsigned long arg)
{
    switch (cmd) {
    case BLOCKMMA_IOCTL_SYNC:
        return blockmma_sync((void __user *) arg);
    case BLOCKMMA_IOCTL_SEND_TASK:
        return blockmma_send_task((void __user *) arg);
    case BLOCKMMA_IOCTL_GET_TASK:
        return blockmma_get_task((void __user *) arg);
    case BLOCKMMA_IOCTL_COMP:
        return blockmma_comp((void __user *)arg);
    default:
        return -ENOTTY;
    }
}

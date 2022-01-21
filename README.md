# CS202 (2022 Winter) Project: Interacting with "emulated" hardware accelerators

## Overview

Hardware accelerators that implement specialized circuits for compute-intensive algorithms are ubiquitous in modern computers. Famous examples includes Google's Tensor Processing Units and Apple's Nueral Engine. Instead of exposing internal instructions to the rest of the world, these accelerators typically reveal the ``functions'' that they implement and allow users to offload the whole compute kernel through a domain-specific function call. However, as you can imagine, lots of work need to be done to bridge the exposed hardware/software interface to the interface presented to the user.

In this project, you will be playing a role as an engineer with a partner engineer you choose to develop the system software for a hardware accelerator that your start-up company, Universal Chip of Riverside, is going to produce. The accelerator is a set of matrix multiplication units (MMUs) that can perform multiple 128 * 128 * 128 matrix multiplications simultaneously. But as you could imagine, the hardware team is always falling behind and we cannot wait to start our development! Therefore, we have developed an emulated hardware accelerators, essentially a user program performing 128 * 128 * 128 matrix multiplications. With multiple processes of this program running in your system, your system now have multiple emulated MMUs!

UCR also plans to present an API function -- blockmma for user programs. The programmer using this API simply needs to specify which the pointer to three distinct floating-point matrices (two as inputs and one as the result) and the M, N, Ks of these three matrices. The library function (already provided by the language team of UCR) will partition the given matrices into triplets of 128 * 128 submatrices from the three matrices. 

Now, the problem is back to you -- how to provide the appropriate system software support for these accelerators. You will need to implement a Linux kernel module, working as the device driver of the backend accelerators. The device driver receives commands from the user-program/library that are essentailly MMA requests broken down into 128 * 128 * 128 ones and a synchronization request that reflects all local updates on the accelerators to the user program. However, there are a few challenges here: 

(1) Data exchange/update between different virtual address spaces -- remember accelerators are emulated as separate processes, they cannot see the memory content of other user applications directly.

(2) The memory layout -- the user program stores matrices in row-major order that each row is sized as N or K. However, your accelerator only accepts/layout matrices in 128 elements per row.

(3) Task management -- we have multiple MMUs running at the backend. Can you evenly distribute tasks to each? Can you make sure each task is exactly executed only once? Can you distinguish requests from different user processes and distribute results to the correct ones?

(4) Supporting multi processes -- you will have multiple user processes running simultaneously. You have to make sure none of them gets stuck.

In this project, you will be given the prototype of the kernel module with a core.c file in its source directory that only contains empty functions. You're only allowed to turn in and modify this file (as you are working in the kernel module team). We also provide a user-space library that allows an application to interact with this kernel module through the ioctl interface as well as a sample benchmark application that you may extend to test if your kernel module functions correctly.

You are strongly encouraged to work in a group of 2 but not to exceed 3. Groups do the same project as individuals. Both members will receive the same grade. Note that working in groups may or may not make the project easier, depending on how the group interactions work out. If collaboration issues arise, contact your instructor as soon as possible: flexibility in dealing with such issues decreases as the deadline approaches.

**We don't answer questions related to the project during lectures to save everyone's time and promote diversity in designs. Please use office hours instead. Thanks for your understanding.**

## Objective

- Learning UNIX/Linux kernel programming as well as the constraints
- Learning UNIX/Linux system process scheduling
- Learning UNIX/Linux kernel modules
- Learning UNIX/Linux system memory management
- Learning the memory layout of the C programming language

## How to start 

To begin, you need to first form a group and setup the environment for developing your project. You should set up a machine or a virtual machine (highly recommended) using VMWare/VirtualBox (you may obtain the license of VMWare from the CSE department IT/the later is free) with a clean **Ubuntu 20.04** installation using ISO from https://ubuntu.com/download/desktop. Your virtual machine should at least have 4GB of physical memory and 2 processors. After setting up this virtual machine image, don't forget to install essential packages including make and git. It's your task to figure out how to use VMWare/VirtualBox ,Ubuntu Linux commands as well as the skills of accessing, managing github repos. **Google and stackoverflow are your good friends and if you did not show sufficient amount of efforts in looking for solutions but directly asking for help from teaching support staffs, we have the right to refuse giving an answer to your questions.**

Then, you need to clean the code from 
https://github.com/hungweitseng/CS202-MMA.git
and make your own private repository. Please do not fork for the given repository. Otherwise, you will be the public repository. After cloning the code, you will find three directories and a test script. These directories are:

1. kernel_module -- the directory where we have the kernel module code.

2. library -- the directory of the user-space library code.

3. benchmark -- the directory with a sample program using this kernel.

You may now go to the kernel_module directory and type "make" to compile the kernel module and then 
``sudo make install``
to install headers and the module in the right place. You should be able to find a blockmma.ko if your compilation success and this is the binary of the kernel module. 

However, this kernel module isn't in your kernel yet. To get this kernel module loaded into the system kernel, try 
``sudo insmod blockmma.ko``.
Upon success, you should find an "blockmma" device file under /dev directory in your linux system. By default, this device may not be available for non-root users. Therefore, you need to use 
``sudo chmod 777 /dev/blockmma`` command to make it accessible by anyone and any process in the system. 

If you don't want this device to be available in the system anymore, you can use ``sudo rmmod blockmma`` to remove this device.

Now, you can navigate to the library path and again use ``make`` to generate this dynamic link library. You need to then use "sudo make install" to make this library publicly available for the system. You should read the code and figure out how this library interacts with the kernel module. 

No matter you're using VMWare or a real machine, it's always a good practice to control/maintain/backup your work using github.

## Testing

Finally, you can now go to the benchmark directory to get the benchmark program compiled using ``make`` command and use the run.sh script through "source ./run.sh`` in the benchmark folder to test and validate your implementation. You may also extend run.sh for your own testing purpose. (But we're not going to take that in our grading process).

After succesfully making the files, the benchmark will contain four user programs:

(1) benchmark -- the user program that calls the blockmma API function. The argument of the program specifies the size of the sqaured matrices.

(2) accelerators -- a server program receiving arguments specifying the number of accelerators to emulate. The children of these programs are accelerators that keep retrieving requests from your kernel module and perform the tasks. The server program also launches sockets as a route to receive control signals from other user programs. Once it receives a "quit" command from the socket, it will kill all accelerators and each accelerator will report the number of tasks it performed during this period.

(3) accelerators_control -- a user program that simply acts as the sender of commands to accelerators.

(4) benchmark_bonus -- the user program that calls the blockmma_bonus API function -- a more efficient function call that only synchronization results once.

For each run, the script will output the result of each running program. If you see the program output contains "Diff", it means your result is incorrect. If your matrix output is identical to CPU computation, you will see a "Passed" in the output. The accelerators will also report tasks each performed. You should expect each accelerator performs similar amount of tasks. If not, that's definitely something you need to work on.

*It won't work/pass if you have done nothing*

## Your tasks

1. Implementing the blockmma kernel module: it needs the following features:

- blockmma_author (**very important**):  you will need to complete this operation that will show "Your Name" (UCRNetID), SID" in this function. We put your grades based on this. You will receive no credits if we cannot find your names in the output. The ioctl system call will be redirected to blockmma_author function located in src/core.c

- blockmma_send_task: you will need to complete this operation that allows a user program to send memory pointers describing three matrices, their sizes and store them appropriately before the task is finished. These blockmma_send_task requests are invoked by the user-space library using BLOCKMMA_IOCTL_SEND_TASK in the ioctl interface. The ioctl system call will be redirected to blockmma_send_task function located in src/ioctl.c. Upon success, the function should return a task id. Otherwise, returns -1.

- blockmma_get_task: you will need to complete this operation that allows an accelerator to retrieve a task from your kernel module. The accelerator sends three pointers describing three 128 by 128 matrices. Your kernel module needs to let those content visible to the requesting accelerator. These blockmma_get_task requests are invoked by the user-space library using BLOCKMMA_IOCTL_GET_TASK in the ioctl interface. The ioctl system call will be redirected to blockmma_get_task function located in src/core.c. Upon success, the function should return a task id. Otherwise, returns -1;


- blockmma_comp: you will need to complete this operation that allows an accelerator to report/update a task to your kernel module. The accelerator sends three pointers describing three 128 by 128 matrices being used for computation and a task id your previously returned for update. Your kernel module needs to grab the accelerator's result -- the c matrix and store it somewhere before the user program originated the task asking for synchronization. These blockmma_comp requests are invoked by the user-space library using BLOCKMMA_IOCTL_COMP in the ioctl interface. The ioctl system call will be redirected to blockmma_comp function located in src/core.c. Upon success, the function should return a task id. Otherwise, returns -1;

- blockmma_sync: you will need to complete this operation that allows a user program to collect all outstanding requests. The function should places all computation results from accelerators to their original locations in the user program. These blockmma_sync requests are invoked by the user-space library using BLOCKMMA_IOCTL_SYNC in the ioctl interface. The ioctl system call will be redirected to blockmma_sync function located in src/core.c. If all outstanding tasks are updated, the function should return 0. Otherwise, returns -1;


2. Test the developed module: It's your responsibility to test the developed kernel module thoroughly. Our benchmark is just a starting point of your testing. We will generate a different test sequence to test your program when grading. Your module should support an infinite number of user processes and different numbers of accelerators at the b with each container.

3. Grading guidelines: 
- A: 40% -- if we are able to compile your kernel module and at least gets your name(s) printed through the output.
- B: 20% -- if you pass A and the benchmark can pass all single process, single accelerator cases with matrix sizes up to 1024. 
- C: 20% -- if you pass A and B and the benchmark can pass 
  - all single process, single accelerator cases with matrix sizes up to 2048 and fairly distribute tasks among accelerators. 
  - all single process, multiple accelerator cases with matrix sizes up to 2048 and fairly distribute tasks among accelerators.
- D: 15% -- if you pass A and B and C and the benchmark can pass 
  - all single process, single accelerator cases with matrix sizes up to 2048. 
  - all single process, multiple accelerator cases with matrix sizes up to 2048 and fairly distribute tasks among accelerators. 
  - all multiple processes, multiple accelerator cases with matrix sizes up to 2048 and fairly distribute tasks among accelerators
- E: 5% -- if you satify all of A, B, C, D and if we substitute benchmark with benchmark_bonus and it still passes A, B, C, D.

## Deliverables

You only need to (or say you can only) turn in the core.c file from the kernel_module/src. All your modifications should be limited within this file. Exactly 1 member of each group should submit the source code. All group members' names, UCRNetIDs and SIDs should be returned through the blockmma_author function located in core.c.

The due date is 3/3/2022. There won't be any extension and we won't accept late submissions.

You will need to turn in it through eLearn and select your members properly during the submission process.
https://elearn.ucr.edu/courses/31822/assignments/293869

## Reference and hints

1. You should try to figure out the interactions between user-space applications (e.g. benchmark) and the user-space library, the user-space library and the kernel module.

2. Before you start, thinking about the mechanisms within your module in handling requests and data. Through not directly related, reviewing computer architecture content regarding how to handle instructions executing in parallel, especially how modern processors implements "reorder buffers" https://docs.boom-core.org/en/latest/sections/reorder-buffer.html
and simultaneous multithreading will definitely inspire you!

3. You may need to reference the Linux kernel programming guide and Linux Device Drivers, 3rd Edition (https://lwn.net/Kernel/LDD3/) since user-space libraries will not be available for kernel code. 4th edition is not free, but we believe you can find lots of references through Googling.

4. You may find the following kernel library functions useful

- kmalloc/kcalloc/kfree
- mutex_init/mutex_lock/mutex_unlock
- printk
- copy_from_user/copy_to_user
- remap_pfn_range
- virt_to_phys
- rcu_read_lock()/rcu_read_unlock()/synchronize_rcu()/call_rcu()/rcu_assign_pointer()/rcu_dereference()

5. You may need to know these variable

- struct task_struct * current;
- gfp_t GFP_KERNEL;


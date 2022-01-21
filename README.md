# CS202 (2022 Winter) Project: Interacting with "emulated" hardware accelerators

## Overview


## Objective

- Learning UNIX/Linux kernel programming as well as the constraints
- Learning UNIX/Linux system process scheduling
- Learning UNIX/Linux kernel modules
- Learning multithreading programming
- Learning UNIX/Linux interrupt handler implementation
- Learning UNIX/Linux system memory management

## How to start 

To begin, you need to first form a group and setup the environment for developing your project. You should set up a machine or a virtual machine (highly recommended) using VMWare/VirtualBox (you may obtain the license of VMWare from the CSE department IT/the later is free) with a clean Ubuntu <b>20.04</b> installation. Then, you need to clean the code from 
https://github.com/hungweitseng/CS202-MMA.git
and make your own private repository. Please do not fork for the given repository. Otherwise, you will be the public repository. After cloning the code, you will find three directories and a test script. These directories are:

1. kernel_module -- the directory where we have the kernel module code.

2. library -- the directory of the user-space library code.

3. benchmark -- the directory with a sample program using this kernel.

You may now go to the kernel_module directory and type "make" to compile the kernel module and then "sudo make install" to install headers and the module in the right place. You should be able to find a resource_container.ko if your compilation success and this is the binary of the resource_container kernel module. 

However, this kernel module isn't in your kernel yet. To get this kernel module loaded into the system kernel, try "sudo insmod resource_container.ko". Upon success, you should find an "blockmma" device file under /dev directory in your linux system. By default, this device may not be available for non-root users. Therefore, you need to use "sudo chmod 777 /dev/blockmma" command to make it accessible by anyone and any process in the system. 

If you don't want this device to be available in the system anymore, you can use "sudo rmmod resource_container" to remove this device.

Now, you can navigate to the library path and again use "make" to generate this dynamic link library. You need to then use "sudo make install" to make this library publicly available for the system. You should read the code and figure out how this library interacts with the kernel module. 

No matter you're using VMWare or a real machine, it's always a good practice to control/maintain/backup your work using github.

## Testing

Finally, you can now go to the benchmark directory to get the benchmark program compiled and use the compiled script "test.sh" at the base folder to test and validate your implementation. 

For example, if type "./test.sh 1 1 4096", it will generate 1 container which is registered by 1 task, and it will have 4096 bytes data in the shared heap. If type "./test.sh 2 2 4096 1 8192", it will generate 2 containers where the first container has 2 tasks, 4096 bytes data in the shared heap visible by these tasks in the 1st container. For the 2nd container, it will have only one task and contain a 8192B-sized heap. So on and so forth.

For each run, the test script will dump the log from each thread. For threads within the same container, you should expect the same summarization outcome, and all threads within the same container should make roughly equal amount of tasks. You should expect the number of containers and tasks can be large and the size of the heap can be up to 1GB.

## Your tasks

1. Implementing the resource_container kernel module: it needs the following features:

- create: you will need to support create operation that creates a container if the corresponding cid hasn't been assigned yet, and assign the task to the container. These create requests are invoked by the user-space library using ioctl interface. The ioctl system call will be redirected to resource_container_ioctl function located in src/ioctl.c

- delete: you will need to support delete operation that removes tasks from the container. If there is no task in the container, the container should be destroyed as well. These delete requests are invoked by the user-space library using ioctl interface. The ioctl system call will be redirected to resource_container_ioctl function located in src/ioctl.c

- switch: you will need to support Linux process scheduling mechanism to switch tasks between threads.

- lock/unlock: you will need to support locking and unlocking that guarantees only one process can access an object at the same time. These lock/unlock functions are invoked by the user-space library using ioctl interface. The ioctl system call will be redirected to resource_container_ioctl function located in src/ioctl.

- mmap: you will need to support mmap, the interface that user-space library uses to request the mapping of kernel space memory into the user-space memory. The kernel module takes an offset from the user-space library and allocates the requested size associated with that offset. You may consider that offset as an object id. If an object associated with an offset was already created/requested since the kernel module is loaded, the mmap request should assign the address of the previously allocated object to the mmap request. The kernel module interface will call resource_container_mmap() in src/core.c to request an mmap operation. One of the parameters for the resource_container_mmap() is "struct vm_area_struct *vma". This data structure contains page offset, starting virtual address, and etc, those you will need to allocate memory space. You also need to fill 0s in the allocated heap space.

- free: you will need to support delete operation that removes an object from resource_container. These delete requests are invoked by the user-space library using ioctl interface. The ioctl system call will be redirected to resource_container_ioctl function located in src/ioctl.c


2. Test the developed module: It's your responsibility to test the developed kernel module thoroughly. Our benchmark is just a starting point of your testing. We will generate a different test sequence to test your program when grading. Your module should support an infinite number of containers and different numbers of tasks with each container.

## Deliverables

You only need to (or say you can only) turn in the core.c and the ioctl.c file in the kernel_module/src directory as a tarball. All your modifications should be limited within these two files. Exactly 1 member of each group should submit the source code. All group members' names and Unity IDs should be easily found in a single line comment at the beginning of the code in the following format:

// CS202 Project: 1st member's name, 1st member's student ID; 2nd member's name, 2nd member's student ID 

You need to name the tarball as {1st_members_sid}_{2nd_members_sid}_mma.tar.gz

## Reference and hints

1. You should try to figure out the interactions between user-space applications (e.g. benchmark) and the user-space library, the user-space library and the kernel module. You should especially understand how to context switch tasks in the user-space that the functionality is defined in pcontainer_init(), handler(), pcontainer_context_switch_handler() from the user-space library. And you also need to know how to wake up/pause tasks by using wake_up_process()/schedule()/set_current_state(), which are kernel-space functions. Here is the explanation of how to control the status of processes in Linux system: https://www.linuxjournal.com/article/8144

2. You may need to reference the Linux kernel programming guide and Linux Device Drivers, 3rd Edition since user-space libraries will not be available for kernel code.

3. You may find the following kernel library functions useful

- kmalloc/kcalloc/kfree

- mutex_init/mutex_lock/mutex_unlock

- wake_up_process/schedule/set_current_state

- printk

- copy_from_user

- remap_pfn_range

- virt_to_phys


4. You may need to know these variable

- volatile long TASK_INTERRUPTIBLE;

- volatile long TASK_RUNNING;

- struct task_struct *current;

- gfp_t GFP_KERNEL;


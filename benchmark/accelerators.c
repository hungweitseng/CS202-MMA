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
//   Author:  Hung-Wei Tseng and Yu-Chia Liu
//
//   Description:
//     Creating "Emulated Hardware Accelerators"
//
////////////////////////////////////////////////////////////////////////

#include <blockmma.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <malloc.h>
#include <sys/syscall.h>
#include <sys/prctl.h> // prctl(), PR_SET_PDEATHSIG
#include <signal.h> // signals
#include <sys/socket.h>
#include <netinet/in.h>

#define RCVBUFSIZE 256

int main(int argc, char *argv[])
{
    // variable initialization
    unsigned long long msec_time;
    FILE *fp;
    int devfd;
    int i;
    struct timeval current_time;
    int num_of_accelerators = 0;
    int child_pid;
    struct sockaddr_in serverAddress;
    int serverSocket; 
    int clientSocket; 
    struct sockaddr_in clientAddress;
    socklen_t lengthOfAddress;
    fd_set masterSocket;
    fd_set read_fds;
    int status;
    int cliAddressLength;
    int *children;
    char *authors;
    struct timeval timeout, origTimeout;
    char echoBuffer[RCVBUFSIZE]; 
    struct blockmma_hardware_cmd cmd;
    pid_t ppid_before_fork = getpid();
    int recvSize;
    int pagesize = sysconf(_SC_PAGE_SIZE);
    num_of_accelerators = atoi(argv[1]);
    devfd = open("/dev/blockmma", O_RDWR);
    if (devfd < 0)
    {
        fprintf(stderr, "Device open failed");
        exit(1);
    }
    authors = (char *)memalign(pagesize, pagesize);
    cmd.op = (__u64)0;
    cmd.a = (__u64)authors;
    
    ioctl(devfd, BLOCKMMA_IOCTL_AUTHOR, &cmd);
    fprintf(stderr, "Authors: %s\n",authors);
    children = (int *)calloc(num_of_accelerators, sizeof(int));
    for(i = 0; i< num_of_accelerators; i++)
    {
        child_pid = fork();
        if(child_pid == 0) break;
        children[i] = child_pid;
    }
    if (child_pid == 0)
    {
//        int r = prctl(PR_SET_PDEATHSIG, SIGTERM);
//        if (r == -1) { perror(0); exit(1); }
    // test in case the original parent exited just
    // before the prctl() call
//        printf("Accelerator Started\n");
//        if (getppid() != ppid_before_fork)
//            exit(1);
//        printf("Accelerator Started\n");
        blockmma_f128_accelerator(devfd);
//        printf("Accelerator Ended\n");
    }
    else
    {
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;
	// Start the server
        if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {
        perror("Error While Opening Socket -- Another process may block the specified port.\n");
        exit(1);
        }
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
             perror("setsockopt(SO_REUSEADDR) failed");
        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family=AF_INET;
        serverAddress.sin_port=htons(27072);
        serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
        if(bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0)  
  	    perror("bind server error\n");
        if(listen(serverSocket,50) < 0)
        {
  	     perror("Error while listening!");
        }
        FD_ZERO(&masterSocket);
        FD_SET(serverSocket,&masterSocket);
        while(1)
        {
             read_fds = masterSocket;
             memcpy((void*)&timeout, (void*)&origTimeout, sizeof(struct timeval));
             if(select(serverSocket + 1, &read_fds, NULL, NULL, &timeout) > 0)
             {
  	        memset(echoBuffer, 0, RCVBUFSIZE);
  	        cliAddressLength = sizeof(clientAddress);
  	        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &cliAddressLength);
  	        if ((recvSize = recv(clientSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
  	        {
  	                fprintf(stderr,"Error!\n");
  	        }
  	        else
  	        {
  	                while(recvSize > 0)
  	                {
  	                        if(echoBuffer[0]=='b')
  	                        {
  	                        }
  	                        else if(echoBuffer[0]=='q')
    	                        {
    	                          printf("Server Finishing\n");
    	                          for(i = 0;i < num_of_accelerators; i++)
    	                          {
    	                              kill(children[i], SIGQUIT);
                                      while(waitpid(children[i], &status, WNOHANG)!=0);
                                  }
                                  free(children);
    	                          exit(1);
                                }
                                recvSize = recv(clientSocket, echoBuffer, RCVBUFSIZE, 0);
                        }
                }
            }
        }
    }
}

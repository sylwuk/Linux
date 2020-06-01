/*
 * dup_io.c
 *
 *  Created on: May 23, 2020
 *      Author: sylwek
 */
#include <stdio.h>         /* Standard I/O functions */
#include <errno.h>         /* Declares errno and defines error constants */
#include <stdlib.h>        /* Prototypes of commonly used library functions,
                            plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <fcntl.h>
#include "tlpi_hdr.h"

int myDup(int descriptor) {
   return fcntl(descriptor, F_DUPFD, 0);
}

int myDup2(int descriptor, int newDescriptor) {
   if(descriptor == newDescriptor) {
      if(fcntl(descriptor, F_GETFL) < 0) return EBADF;   /* Check if descriptor exists */
      else return descriptor;
   }
   close(newDescriptor);
   return fcntl(descriptor, F_DUPFD, newDescriptor);
}

int main(int argc, char *argv[]) {
   if(argc < 3) {
      fprintf(stderr, "%s <filename> <string>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   int fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
   if(fd < 0) errExit("open");
   int fd2 = myDup(fd);
   if(fd < 0) errExit("myDup");
   int fd3 = myDup2(fd2, STDOUT_FILENO);
   if(fd3 < 0) errExit("myDup2");

   if(write(fd, argv[2], strlen(argv[2])) < strlen(argv[2])) errExit("write fd");
   if(write(fd2, argv[2], strlen(argv[2])) < strlen(argv[2])) errExit("write fd2");
   if(write(fd3, argv[2], strlen(argv[2])) < strlen(argv[2])) errExit("write fd3");

   printf("dupa blada jak szuflada\n");

   if(close(fd) < 0) errExit("close");
   if(close(fd2) < 0) errExit("close");
   if(close(fd3) < 0) errExit("close");

   exit(EXIT_SUCCESS);
}

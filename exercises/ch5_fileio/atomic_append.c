/*
 * atomic_append.c
 *
 * Program that seeks to the end of the file specified as an argument
 * and writes a byte of data. It does it for the number of times specified
 * in the second argument. By default file is open without O_APPEND flag.
 * If the -a option is specified as a third argument file is opened with O_APPEND.
 *
 *  Created on: May 23, 2020
 *      Author: sylwek
 */

#include <sys/types.h>     /* Type definitions used by many programs */
#include <stdio.h>         /* Standard I/O functions */
#include <stdlib.h>        /* Prototypes of commonly used library functions,
                            plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>        /* Prototypes for many system calls */
#include <errno.h>         /* Declares errno and defines error constants */
#include <string.h>        /* Commonly used string handling functions */
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {
   if(argc < 3 || strcmp(argv[1], "--help") == 0) {
      printf("Usage: %s <filename> <num_bytes> [-a]\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   int flags = O_CREAT | O_WRONLY, num_bytes = atoi(argv[2]);
   if(argc > 3 && strcmp(argv[3], "-a") == 0) flags |= O_APPEND;

   int fd = open(argv[1], flags, S_IRUSR | S_IWUSR);
   if(fd < 0) errExit("open");

   while(num_bytes-- > 0) {
      if(lseek(fd, 0, SEEK_END) < 0) errExit("lseek");
      /* If O_APPEND flag is not specified there is a possible race condition here */
      if(write(fd, "a", 1) < 1) errExit("write");
   }
   exit(EXIT_SUCCESS);
}

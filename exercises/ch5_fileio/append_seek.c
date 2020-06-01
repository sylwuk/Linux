/*
 * append_seek.c
 *
 * Program that opens the file specified as an argument,
 * seeks to the beginning of the file and writes a string argument.
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
      printf("Usage: %s <filename> <string>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   int fd = open(argv[1], O_WRONLY | O_APPEND);
   if(fd < 0) errExit("open");

   if(lseek(fd, 0, SEEK_SET) < 0) errExit("lseek");
   if(write(fd, argv[2], strlen(argv[2])) < strlen(argv[2])) errExit("write");
   exit(EXIT_SUCCESS);
}


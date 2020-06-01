/*
 * tee_io.c
 *
 *  Created on: May 19, 2020
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

#ifndef BUF_SIZE           /* Allow cc -D to override definition */
#define BUF_SIZE 1024
#endif

void usage() {
   printf("Usage: ./tee_io [OPTION]... [FILE]...\n");
   printf("Program that copies its standard input to standard output and to the files specified\n");
   printf("Options:\n\t-a append to given files, do not overwrite\n");
}

int main(int argc, char *argv[]) {
   int arg;
   ssize_t numRead;

   int flags = O_CREAT | O_WRONLY;

   while((arg = getopt(argc, argv, "ha")) != -1) {
      switch(arg) {
      case 'h':
         usage();
         exit(EXIT_SUCCESS);
      case 'a':
         flags |= O_APPEND;
         break;
      case '?':
         if(isprint(optopt)) {
            fprintf(stderr, "Unknown option -%c\n", optopt);
         } else {
            fprintf(stderr, "Unknown option character \\x%x\n", optopt);
         }
         exit(EXIT_FAILURE);
      default:
         abort();
      }
   }

   int *descriptors, idx = 1, noOfFiles = argc-optind+1;
   char buffer[BUF_SIZE];
   mode_t filePerms = S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH;  /* rw-rw-rw- */

   // Open all needed files first and save file descriptors in descriptors table
   // to avoid closing and re-opening the files with every write
   descriptors = malloc(noOfFiles * sizeof(*descriptors));
   descriptors[0] = STDOUT_FILENO;
   for(int fileIdx = optind; fileIdx < argc; ++fileIdx) {
      if((descriptors[idx] = open(argv[fileIdx], flags, filePerms)) < 0) {
         free(descriptors);
         errExit("Failure in opening the file %s", argv[fileIdx]);
      }
      ++idx;
   }

   /* Read from standard input, write to standard output and all the open files */
   while((numRead = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0) {
      for(idx = 0; idx < noOfFiles; ++idx) {
         if(write(descriptors[idx], buffer, numRead) != numRead) {
            free(descriptors);
            errExit("Failure when writing");
         }
      }
   }

   /* Close all the open file descriptors. Descriptor 0 is standard output */
   for(idx = 1; idx < noOfFiles; ++idx) {
      if(close(descriptors[idx]) < 0) {
         free(descriptors);
         errExit("Error closing the file");
      }
   }

   free(descriptors);
   exit(EXIT_SUCCESS);
}

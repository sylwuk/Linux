/*
 * t_readv.c
 *
 *  Created on: May 22, 2020
 *      Author: sylwek
 */

#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char * argv[]) {
   int fd;
   struct iovec iov[3];
   struct stat myStruct;         /* First buffer */
   int x;                        /* Second buffer */
#define STR_SIZE 100
   char str[STR_SIZE];           /* Third buffer */
   ssize_t numRead, totRequired = 0;

   if(argc != 2 || strcmp(argv[1], "--help") == 0) usageErr("%s file\n", argv[0]);
   if((fd = open(argv[1], O_RDONLY)) == -1) errExit("Open");

   iov[0].iov_base = &myStruct;
   iov[0].iov_len = sizeof(myStruct);
   totRequired += iov[0].iov_len;

   iov[0].iov_base = &x;
   iov[0].iov_len = sizeof(x);
   totRequired += iov[1].iov_len;

   iov[0].iov_base = &str;
   iov[0].iov_len = sizeof(STR_SIZE);
   totRequired += iov[2].iov_len;

   if((numRead = readv(fd, iov, 3)) == -1) errExit("readv");
   if(numRead < totRequired) printf("Read fewer bytes than requested\n");

   printf("total bytes requested: %ld; bytes read: %ld\n", (long)numRead, (long)totRequired);
   exit(EXIT_SUCCESS);
}

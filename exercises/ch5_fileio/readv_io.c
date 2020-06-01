/*
 * readv_io.c
 *
 *  Created on: May 24, 2020
 *      Author: sylwek
 */
#include <stdio.h>         /* Standard I/O functions */
#include <errno.h>         /* Declares errno and defines error constants */
#include <stdlib.h>        /* Prototypes of commonly used library functions,
                            plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <fcntl.h>
#include <sys/uio.h>
#include "tlpi_hdr.h"

ssize_t myReadv(int fd, const struct iovec *iov, int iovcnt) {
   if(iovcnt < 0) return -1;
      if(iovcnt == 0) return 0;

      ssize_t bytesRead = 0;
      for(int count = 0; count < iovcnt; ++count) {
         if(read(fd, iov[count].iov_base, iov[count].iov_len) < iov[count].iov_len)
            errExit("%d read", count);
         bytesRead += iov[count].iov_len;
      }
      return bytesRead;
}

ssize_t myWritev(int fd, const struct iovec *iov, int iovcnt) {
   if(iovcnt < 0) return -1;
   if(iovcnt == 0) return 0;

   ssize_t bytesWriten = 0;
   for(int count = 0; count < iovcnt; ++count) {
      if(write(fd, iov[count].iov_base, iov[count].iov_len) < iov[count].iov_len)
         errExit("write");
      bytesWriten += iov[count].iov_len;
   }
   return bytesWriten;
}

typedef struct myStruct {
   int a;
   char str[11];
   char letter;
} myStruct;

int main(int argc, char *argv[]) {
   if(argc < 2) {
      fprintf(stderr, "%s <filename>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
   if(fd < 0) errExit("open");

   int digit = 10, digir = 0;
   ssize_t total = 0;
   myStruct obj = {.a = 20, .str = "ala ma kota", .letter = '\n'};
   myStruct objr;
   struct iovec iov[2], iovread[2];

   iov[0].iov_base = &digit;
   total += iov[0].iov_len = sizeof(digit);
   iov[1].iov_base = &obj;
   total += iov[1].iov_len = sizeof(obj);

   if(myWritev(fd, iov, 2) < total) errExit("myWritev");

   iovread[0].iov_base = &digir;
   iovread[0].iov_len = sizeof(digir);
   iovread[1].iov_base = &objr;
   iovread[1].iov_len = sizeof(objr);

   if(lseek(fd, 0, SEEK_SET) < 0) errExit("lseek");;
   if(myReadv(fd, iovread, 2) < total) errExit("myReadv");

   printf("Read: %d %d %s %c\n", digir, objr.a, objr.str, objr.letter);

   if(close(fd) < 0) errExit("close");
   exit(EXIT_SUCCESS);
}


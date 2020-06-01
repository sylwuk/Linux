/*
 * unsetenv.c
 *
 *  Created on: May 28, 2020
 *      Author: sylwek
 */
#include <stdio.h>         /* Standard I/O functions */
#include <stdlib.h>        /* Prototypes of commonly used library functions,
                            plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <string.h>        /* Commonly used string handling functions */
#include "tlpi_hdr.h"

extern char **environ;

static void displayEnv() {
   for(char **ep = environ; *ep != NULL; ++ep) {
      puts(*ep);
   }
}

int setenvMy(const char *name, const char * value, int overwrite) {
   char *variable = getenv(name);
   if(variable) {
      if(overwrite) {
         strcpy(variable, value);            /* Is there enough memory? */
      }
   } else {
#define BUF_SIZE 256
      char* entry = (char *)malloc(256*sizeof(char));    /* Who should free it? */
      *entry = '\0';
      strcat(entry,name);
      strcat(entry,"=");
      strcat(entry,value);
      return putenv(entry);
   }
   return 0;
}

int unsetenvMy(const char *name) {

}

int main(int argc, char *argv[]) {
   if(argc < 2) {
      printf("Usage: %s [name=value]...\n", argv[0]);
      exit(EXIT_SUCCESS);

   }
   clearenv();                /* Erase entire environment */

   for(int j = 1; j < argc; ++j) {
      if(putenv(argv[j]) != 0) errExit("putenv: %s\n", argv[j]);
   }

   displayEnv();
   printf("-------------------------------\n");
   setenvMy("dupa", "pupa", 1);
   setenvMy("dupa", "noga", 0);
   setenvMy("tadziu", "babel", 0);

   displayEnv();

   exit(EXIT_SUCCESS);
}

/*
 * modify_env.c
 *
 * Program modifying process environment.
 *
 *  Created on: May 25, 2020
 *      Author: sylwek
 */
#define _GNU_SOURCE           /* Get various declarations from <stdlib.h>*/
#include <stdlib.h>
#include "tlpi_hdr.h"

extern char **environ;

int main(int argc, char *argv) {

   clearenv();                /* Erase entire environment */

   for(int j = 1; j < argc; ++j) {
      if(putenv(argv[j]) != 0) errExit("putenv: %s\n", argv[j]);
   }

   if(setenv("GREET", "Hello world", 0) == -1) errExit("setenv");

   unsetenv("BYE");

   for(char **ep = environ; ep != NULL; ++ep) {
      puts(*ep);
   }
   exit(EXIT_SUCCESS);
}

/*
 * display_env.c
 *
 *  Created on: May 25, 2020
 *      Author: sylwek
 */
#include "tlpi_hdr.h"

extern char **environ;

int main(int argc, char *argv[]) {
   for(char **ep = environ; ep != NULL; ++ep) {
      puts(*ep);
   }
   exit(EXIT_SUCCESS);
}

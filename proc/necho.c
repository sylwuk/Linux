/*
 * necho.c
 *
 * Program echoing command-line arguments.
 *
 *  Created on: May 24, 2020
 *      Author: sylwek
 */
#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {
   for(int j = 0; j < argc; ++j) {
      printf("argv[%d] = %s\n", j, argv[j]);
   }
   exit(EXIT_SUCCESS);
}


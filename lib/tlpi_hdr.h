/*
 * tlpi_hdr.h
 *
 *  Created on: May 11, 2020
 *      Author: sylwek
 */

#ifndef LIB_TLPI_HDR_H_
#define LIB_TLPI_HDR_H_

#include <sys/types.h>     /* Type definitions used by many programs */
#include <stdio.h>         /* Standard I/O functions */
#include <stdlib.h>        /* Prototypes of commonly used library functions,
                            plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>        /* Prototypes for many system calls */
#include <errno.h>         /* Declares errno and defines error constants */
#include <string.h>        /* Commonly used string handling functions */

#include "get_num.h"       /* Declares functions for handling numeric arguments */
#include "error_functions.h" /* Declares our error-handling functions */

typedef enum {FALSE = 0, TRUE = 1} Boolean;

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif /* LIB_TLPI_HDR_H_ */

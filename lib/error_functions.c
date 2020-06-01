/*
 * error_functions.c
 *
 *  Created on: May 11, 2020
 *      Author: sylwek
 */

#include <stdarg.h>
#include "error_functions.h"
#include "tlpi_hdr.h"
#include "ename.c.inc"              /* Defines ename and MAX_ENAME */

#ifdef __GNUC__
__attribute__ ((__noreturn__))
#endif
static void terminate(Boolean useExit3) {
   char *s = NULL;

   /* Dump core if EF_DUMPCORE environment variable is defined and
    * is a nonempty string; otherwise call exit(3) or _exit(2),
    * depending on the value of 'useExit3'. */

   s = getenv("EF_DUMPCORE");

   if(s != NULL && *s != '\0') {
      abort();
   } else if (useExit3) {
      exit(EXIT_FAILURE);
   } else {
      _exit(EXIT_FAILURE);
   }
}

static void outputError(Boolean useErr, int err, Boolean flushStdout,
                        const char *format, va_list ap) {
#define BUF_SIZE 500
   char buf[BUF_SIZE], usrMsg[BUF_SIZE], errText[BUF_SIZE];

   vsnprintf(usrMsg, BUF_SIZE, format, ap);

   if(useErr) {
      snprintf(errText, BUF_SIZE, " [%s %s]",
               (err > 0 && err <= MAX_ENAME) ?
               ename[err] : "?UNKNOWN?", strerror(err));
   } else {
      snprintf(errText, BUF_SIZE, ":");
   }

   snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, usrMsg);

   if(flushStdout) {
      fflush(stdout);         /* Flush any pending stdout */
   }
   fputs(buf, stderr);
   fflush(stderr);            /* In case stderr is not line-buffered */
}

/**
 * Function that prints a message on standard error.
 * This function prints the error text corresponding to
 * the current value of errno consisting of the error name,
 * such as EPERM, plus the error description as returned by strerror()
 * followed by the formatted output specified in the argument list.
 *
 * @param format        output format, similar as in printf()
 */
void errMsg(const char* format, ...) {
   va_list argList;
   int savedErrno = errno;    /* Save errno in case we change it here */

   va_start(argList, format);
   outputError(TRUE, errno, TRUE, format, argList);
   va_end(argList);

   errno = savedErrno;
}

/**
 * Function that prints a message on standard error.
 * This function prints the error text corresponding to
 * the current value of errno consisting of the error name,
 * such as EPERM, plus the error description as returned by strerror()
 * followed by the formatted output specified in the argument list.
 *
 * Terminates the program with exit() if EF_DUMPCORE environment
 * variable is not set. If EF_DUMPCORE is defined with non empty
 * string value it aborts the program to produce a core dump.
 *
 * @param format        output format, similar as in printf()
 */
void errExit(const char* format, ...) {
   va_list argList;

   va_start(argList, format);
   outputError(TRUE, errno, TRUE, format, argList);
   va_end(argList);

   terminate(TRUE);
}

/**
 * Function that prints a message on standard error.
 * This function prints the error text corresponding to
 * the current value of errno consisting of the error name,
 * such as EPERM, plus the error description as returned by strerror()
 * followed by the formatted output specified in the argument list.
 * It doesn't flush the stdio before printing error message.
 *
 * Terminates the program with _exit() to prevent flushing of stdio or
 * invoking exit handlers when terminating the program.
 *
 * Used for exiting child processes to not flush parents stdio buffers
 * and without invoking parent exit handlers.
 *
 * @param format        output format, similar as in printf()
 */
void err_exit(const char* format, ...) {
   va_list argList;

   va_start(argList, format);
   outputError(TRUE, errno, FALSE, format, argList);
   va_end(argList);

   terminate(FALSE);
}

/**
 * Function that prints a message on standard error.
 * This function prints the error text corresponding to
 * the value of errnum parameter consisting of the error name,
 * such as EPERM, plus the error description as returned by strerror()
 * followed by the formatted output specified in the argument list.
 *
 * Terminates the program with exit() if EF_DUMPCORE environment
 * variable is not set. If EF_DUMPCORE is defined with non empty
 * string value it aborts the program to produce a core dump.
 * Used for threaded programs.
 *
 * @param errnum        Error number
 * @param format        output format, similar as in printf()
 */
void errExitEN(int errnum, const char* format, ...) {
   va_list argList;

   va_start(argList, format);
   outputError(TRUE, errnum, TRUE, format, argList);
   va_end(argList);

   terminate(TRUE);
}

/**
 * This function is used to diagnose general errors, including errors
 * from library functions that don't set errno. It prints the formatted
 * output on standard error and then terminates the program.
 *
 * Terminates the program with exit() if EF_DUMPCORE environment
 * variable is not set. If EF_DUMPCORE is defined with non empty
 * string value it aborts the program to produce a core dump.
 *
 * @param format        output format, similar as in printf()
 */
void fatal(const char* format, ...) {
   va_list argList;

   va_start(argList, format);
   outputError(FALSE, 0, TRUE, format, argList);
   va_end(argList);

   terminate(TRUE);
}

/**
 * This function is used to diagnose errors in command-line argument usage.
 * It takes an argument list in style of printf() and prints the string
 * Usage: followed by the formatted output on standard error,
 * and then terminates the program by calling exit().
 *
 * @param format        output format, similar as in printf()
 */
void usageErr(const char* format, ...) {
   va_list argList;

   fflush(stdout);         /* Flush any pending stdout */

   fprintf(stderr, "Usage: ");
   va_start(argList, format);
   vfprintf(stderr, format, argList);
   va_end(argList);

   fflush(stderr);         /* In case stderr is not line-buffered */
   exit(EXIT_FAILURE);
}

/**
 * This function is used to diagnose errors in command-line arguments.
 * It takes an argument list in style of printf() and prints the string
 * Command-line usage error:: followed by the formatted output on standard error,
 * and then terminates the program by calling exit().
 *
 * @param format        output format, similar as in printf()
 */
void cmdLineErr(const char* format, ...) {
   va_list argList;

   fflush(stdout);         /* Flush any pending stdout */

   fprintf(stderr, "Command-line usage error: ");
   va_start(argList, format);
   vfprintf(stderr, format, argList);
   va_end(argList);

   fflush(stderr);         /* In case stderr is not line-buffered */
   exit(EXIT_FAILURE);
}

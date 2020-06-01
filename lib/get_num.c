/*
 * get_num.c
 *
 *  Created on: May 12, 2020
 *      Author: sylwek
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "get_num.h"


static void gnFail(const char* fname, const char* msg, const char *arg, const char *name) {
   fprintf(stderr, "%s error", fname);
   if(name != NULL) {
      fprintf(stderr, "(in %s)", name);
   }
   fprintf(stderr, ": %s\n", msg);
   if(arg != NULL && *arg != '\0') {
      fprintf(stderr, "       offending text:%s\n", arg);
   }
   exit(EXIT_FAILURE);
}

static long getNum(const char* fname, const char *arg, int flags, const char *name) {
   char *endptr;

   if(arg == NULL ||*arg == '\0') {
      gnFail(fname, "null or empty string", arg, name);
   }

   int base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 :
              (flags & GN_BASE_16) ? 16 : 10;

   errno = 0;
   long result = strtol(arg, &endptr, base);

   if(errno != 0) {
      gnFail(fname, "strtol() failed", arg, name);
   }
   if(*endptr != '\0') {
      gnFail(fname, "nonnumeric characters", arg, name);
   }
   if((flags & GN_NONEG) && result < 0) {
      gnFail(fname, "negative value not allowed", arg, name);
   }
   if((flags & GN_GT_O) && result <= 0) {
      gnFail(fname, "value must be > 0", arg, name);
   }

   return result;
}

long getLong(const char *arg, int flags, const char *name) {
   return getNum("getLong", arg, flags, name);
}

int getInt(const char *arg, int flags, const char *name) {
   long result = getNum("getInt", arg, flags, name);

   if(result > INT_MAX || result < INT_MIN) {
      gnFail("getInt", "integer out of range", arg, name);
   }

   return result;
}

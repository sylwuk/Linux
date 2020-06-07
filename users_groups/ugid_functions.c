/*
 * ugid_functions.c
 *
 *  Created on: Jun 1, 2020
 *      Author: sylwek
 */
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include "ugid_functions.h"      /* Declares functions defined here */

/**
 * Function that returns user name based on user id
 *
 * @param uid        user id
 * @return           name corresponding to uid or NULL on error
 */
char *userNameFromId(uid_t uid) {
   struct passwd *pwd;

   pwd = getpwuid(uid);
   return(pwd == NULL) ? NULL: pwd->pw_name;
}

/**
 *Function that returns used id based on name
 *
 * @param name       user name
 * @return           user id or -1 on error
 */
uid_t userIdFromName(const char *name) {
   struct passwd *pwd;
   uid_t u;
   char *endptr;

   if(name == NULL || *name == '\0') return -1;  /* On NULL or empty string return -1 */

   u = strtol(name, &endptr, 10);    /* As a convenience to caller allow numeric string */
   if(*endptr == '\0') return u;

   pwd = getpwnam(name);
   if(pwd == NULL) return -1;

   return pwd->pw_uid;
}

/**
 * Function that returns group name from id
 *
 * @param gid     group id
 * @return        group name or NULL on error
 */
char *groupNameFromId(gid_t gid) {
   struct group *grp;

   grp = getgrgid(gid);
   return(grp == NULL) ? NULL: grp->gr_name;
}

/**
 *Function that returns group id based on name
 *
 * @param name       group name
 * @return           group id or -1 on error
 */
gid_t groupIdFromName(const char *name) {
   struct group *grp;
   gid_t g;
   char *endptr;

   if(name == NULL || *name == '\0') return -1;  /* On NULL or empty string return -1 */

   g = strtol(name, &endptr, 10);    /* As a convenience to caller allow numeric string */
   if(*endptr == '\0') return g;

   grp = getgrnam(name);
   if(grp == NULL) return -1;

   return grp->gr_gid;
}

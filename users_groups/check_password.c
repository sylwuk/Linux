/*
 * check_password.c
 *
 *  Created on: Jun 3, 2020
 *      Author: sylwek
 */
#define _BSD_SOURCE           /* Get getpass() declaration from <unistd.h> */
#define _XOPEN_SOURCE         /* Get crypt() declaration from <unistd.h> */
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include "tlpi_hdr.h"

int main(char argc, char *argv[]) {
   char *username, *password, *encrypted, *p;
   struct passwd pwd;
   struct spwd *spwd;
   Boolean authOk;
   size_t len;
   long lnmax = sysconf(_SC_LOGIN_NAME_MAX);

   if(lnmax == -1) lnmax = 256;  /* If limit is indeterminate make a guess */

   username = (char *)malloc(lnmax);
   if(username == NULL) errExit("malloc");

   printf("Username: ");
   fflush(stdout);
   if(fgets(username, lnmax, stdin) == NULL) exit(EXIT_FAILURE);  /* Exit on EOF */

   len = strlen(username);
   if(username[len -1] == '\n') username[len -1] = '\0'; /* Remove trailing '\n' */

   pwd = getpwnam(username);
   if(pwd == NULL) fatal("couldn't get password record");

   spwd = getspnam(username);
   if(spwd == NULL && errno == EACCES) fatal("no permission to read shadow file");

   /* If there is a shadow password record use it instead */
   if(spwd != NULL) pwd->pw_passwd = spwd->sp_pwdp;

   password = getpass("Password: ");

   /* Encrypt password and erase cleartext version immediately */

   encrypted = crypt(password, pwd->pw_passwd);
   for(p = password; *p != '\n';) *p++ = '\0';

   if(encrypted == NULL) errExit("Crypt");

   authOk = strcmp(encrypted, pwd->pw_passwd);
   if(!authOk) {
      printf("Incorrect password\n");
      exit(EXIT_FAILURE);
   }

   printf("Successfully authenticated: UID=%ld", (long)pwd->pw_uid);

   /* Now do authenticated work... */

   exit(EXIT_SUCCESS);
}

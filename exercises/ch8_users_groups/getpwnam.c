/*
 * getpwnam.c
 *
 *  Created on: Jun 4, 2020
 *      Author: sylwek
 */
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include "tlpi_hdr.h"

struct passwd *mygetpwnam(const char *name) {
   static struct passwd result;
   struct passwd *current;
   setpwent();
   while((current = getpwent()) != NULL && strcmp(current->pw_name, name) != 0);

   if(current == NULL) return NULL;
   else result = *current;
   return &result;
}

int main(int argc, char *argv[]) {

   if(argc < 2) {
      fprintf(stderr, "Usage: %s <username>\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   struct passwd *pwd = mygetpwnam(argv[1]);
   if(pwd == NULL) printf("User with name: %s nod found\n", argv[1]);
   else printf("User: %s ID:%u HOME:%s SHELL:%s\n", argv[1], pwd->pw_uid, pwd->pw_dir, pwd->pw_shell);;
   exit(EXIT_SUCCESS);
}


/*
 * initgroups.c
 *
 *  Created on: Jun 6, 2020
 *      Author: sylwek
 */
#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grp.h>
#include "ugid_functions.h"      /* userNameFromId() & groupNameFromId() */
#include "tlpi_hdr.h"

#define SG_SIZE (NGROUPS_MAX + 1)

static int getUserGid(const char *user) {
   ssize_t numRead = 0;
   size_t len = 0;
   char *line = NULL, *token = NULL;

   FILE *fp = fopen("/etc/passwd", "r");
   if(fp == NULL) return -1;

   while((numRead = getline(&line, &len,fp)) > 0 &&
         strstr(line, user) == NULL);
   fclose(fp);
   if(numRead > 0)
   {
      token = strtok(line, ":");
      for(int i = 0;i < 3; ++i) token = strtok(NULL, ":");
      return atoi(token);
   } else return -1;
}

int myinitgroups(const char *user, gid_t group) {
   if(setgroups(0, NULL) < 0) return -1;     /* Leave errno unchanged. Use the one set by setgroups */

   gid_t suppGroups[SG_SIZE] = {0};
   ssize_t numRead = 0;
   size_t len = 0;
   char *line = NULL, *token = NULL;
   int index = 0;

   /* Scan /etc/group file for all user entries */
   FILE *fp = fopen("/etc/group", "r");
   if(fp == NULL) return -1;

   /* Scan the file, line by line */
   while((numRead = getline(&line, &len,fp)) > 0) {
      if(strstr(line, user) != NULL) {    /* Check if the user substring is present in line */
         token = strtok(line, ":");
         for(int i = 0;i < 2; ++i) token = strtok(NULL, ":");
         suppGroups[index++] = atoi(token);
      }
   }

   if(fclose(fp) < 0) return -1;
   if((suppGroups[index] = (gid_t)getUserGid(user)) < 0) return -1;
   if(setgroups((size_t)index, suppGroups) < 0) return -1;     /* Leave errno unchanged. Use the one set by setgroups */

   return 0;
}

int main(int argc, char *argv[]) {
   //printf("%d\n", getUserGid("gdm"));
   gid_t suppGroups[SG_SIZE];
   int numGroups = getgroups(SG_SIZE, suppGroups);
   char *p;
   if(numGroups == -1) errExit("getgroups");

   printf("Supplementary groups (%d): ", numGroups);
   for(int j = 0;j < numGroups; ++j) {
      p = groupNameFromId(suppGroups[j]);
      printf("%s (%ld) ", (p == NULL) ? "???" : p, (long) suppGroups[j]);
   }
   printf("\n Initialize groups \n");

   if(myinitgroups("sylwek", 0) < 0) errExit("myinitgroups");

   numGroups = getgroups(SG_SIZE, suppGroups);
   if(numGroups == -1) errExit("getgroups");

   printf("Supplementary groups (%d): ", numGroups);
   for(int j = 0;j < numGroups; ++j) {
      p = groupNameFromId(suppGroups[j]);
      printf("%s (%ld) ", (p == NULL) ? "???" : p, (long) suppGroups[j]);
   }
   printf("\n After Initialization. Clear groups. \n");

   if(setgroups(0, NULL) < 0) errExit("setgroups");

   numGroups = getgroups(SG_SIZE, suppGroups);
   if(numGroups == -1) errExit("getgroups");

   printf("Supplementary groups (%d): ", numGroups);
   for(int j = 0;j < numGroups; ++j) {
      p = groupNameFromId(suppGroups[j]);
      printf("%s (%ld) ", (p == NULL) ? "???" : p, (long) suppGroups[j]);
   }
   printf("\n After clearing. \n");

   if(myinitgroups("sylwek", 0) < 0) errExit("myinitgroups");

   numGroups = getgroups(SG_SIZE, suppGroups);
   if(numGroups == -1) errExit("getgroups");

   printf("Supplementary groups (%d): ", numGroups);
   for(int j = 0;j < numGroups; ++j) {
      p = groupNameFromId(suppGroups[j]);
      printf("%s (%ld) ", (p == NULL) ? "???" : p, (long) suppGroups[j]);
   }
   printf("\n After re-initializing. \n");

   exit(EXIT_SUCCESS);
}

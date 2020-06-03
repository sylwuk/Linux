/*
 * ugid_functions.h
 *
 *  Created on: Jun 1, 2020
 *      Author: sylwek
 */

#ifndef USERS_GROUPS_UGID_FUNCTIONS_H_
#define USERS_GROUPS_UGID_FUNCTIONS_H_

#include "tlpi_hdr.h"

char *userNameFromId(uid_t uid);
uid_t userIdFromName(const char *name);
char *groupNameFromId(gid_t gid);
gid_t groupIdFromName(const char *name);

#endif /* USERS_GROUPS_UGID_FUNCTIONS_H_ */

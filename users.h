#ifndef USERS_H
#define USERS_H

#include "config.h"
#include "file_manager.h"
#include "password.h"
#include <stdbool.h>

bool user_register(const char *username, const char *password);
bool user_login(const char *username, const char *password);
bool user_change_password(const char *username, const char *new_password, const char *old_password);
bool user_delete(const char *username);
bool user_exists(const char *username);
void user_list_all(UserRecord records[], int *count);
bool user_search(const char *username, UserRecord *result);
void user_get_info(const char *username, UserRecord *result);

#endif
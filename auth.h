#ifndef AUTH_H
#define AUTH_H

#include "config.h"
#include "file_manager.h"
#include <stdbool.h>

typedef struct {
    char username[MAX_USERNAME_LENGTH + 1];
    int failed_attempts;
    bool locked;
    time_t lockout_time;
} AccountStatus;

bool auth_attempt_login(const char *username, const char *password);
bool auth_verify_credentials(const char *username, const char *password);
void auth_record_login_attempt(const char *username, bool success);
void auth_check_lockout(const char *username);
void auth_init_lockout(void);
AccountStatus auth_get_status(const char *username);
void auth_set_lockout(const char *username, int attempts);
void auth_reset_failed_attempts(const char *username);

#endif
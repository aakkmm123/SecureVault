#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#endif

#define MAX_USERNAME_LENGTH 31
#define MIN_USERNAME_LENGTH 4
#define MAX_PASSWORD_LENGTH 127
#define MIN_PASSWORD_LENGTH 6
#define MAX_INPUT_LENGTH 256
#define MAX_LINE_LENGTH 1024
#define MAX_HISTORY_ENTRIES 100
#define MAX_LOG_ENTRIES 200
#define FILENAME_LENGTH 256
#define SALT_LENGTH 16
#define MAX_FAILED_ATTEMPTS 3
#define LOCKOUT_DURATION_MINUTES 15

#define USERS_FILE "users.txt"
#define HISTORY_FILE "login_history.txt"
#define LOGS_FILE "security_logs.txt"
#define CONFIG_FILE "config.txt"

#define SUCCESS 1
#define FAILURE 0
#define ERROR -1

#define FILE_MODE_READ "r"
#define FILE_MODE_WRITE "w"
#define FILE_MODE_APPEND "a"
#define FILE_MODE_RB "rb"
#define FILE_MODE_WB "wb"

typedef enum {
    ROLE_USER,
    ROLE_ADMIN
} UserRole;

typedef enum {
    STATUS_OK,
    STATUS_DUPLICATE_USERNAME,
    STATUS_INVALID_INPUT,
    STATUS_LOGIN_FAILED,
    STATUS_ACCOUNT_LOCKED,
    STATUS_PASSWORD_WEAK,
    STATUS_ACCOUNT_NOT_FOUND,
    STATUS_SUCCESS
} StatusCode;

#endif
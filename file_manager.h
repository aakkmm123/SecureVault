#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "config.h"
#include <stdbool.h>

typedef struct {
    char username[MAX_USERNAME_LENGTH + 1];
    PasswordHash password_hash;
    char creation_time[MAX_LINE_LENGTH + 1];
} UserRecord;

typedef struct {
    char timestamp[MAX_LINE_LENGTH + 1];
    char username[MAX_USERNAME_LENGTH + 1];
    char event_type[MAX_LINE_LENGTH + 1];
} HistoryEntry;

typedef struct {
    char timestamp[MAX_LINE_LENGTH + 1];
    char description[MAX_LINE_LENGTH + 1];
} SecurityLog;

bool file_read_users(UserRecord records[], int *count);
bool file_write_users(const UserRecord records[], int count);
bool file_append_user(const UserRecord *record);
bool file_read_history(HistoryEntry entries[], int *count);
bool file_append_history(const HistoryEntry *entry);
bool file_read_logs(SecurityLog entries[], int *count);
bool file_append_log(const SecurityLog *entry);
bool file_delete_user(const char *username);
bool file_rename_temp(const char *temp_name, const char *real_name);
FILE *file_open_read(const char *filename);
FILE *file_open_write(const char *filename);
FILE *file_open_append(const char *filename);
void file_safe_delete(const char *filename);

#endif
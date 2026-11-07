#include "file_manager.h"
#include "utils.h"
#include "password.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool file_read_users(UserRecord records[], int *count) {
    FILE *f = file_open_read(USERS_FILE);
    if (f == NULL) {
        *count = 0;
        return true;
    }

    *count = 0;
    while (*count < MAX_USERS) {
        if (fscanf(f, "%63[^|]|%63[^|]|%10s|%19[^\n]\n",
                     records[*count].username,
                     records[*count].password_hash.hash,
                     records[*count].password_hash.salt,
                     records[*count].creation_time) != 4) {
            break;
        }
        (*count)++;
    }

    safe_fclose(&f);
    return true;
}

bool file_write_users(const UserRecord records[], int count) {
    FILE *f = file_open_write(USERS_FILE);
    if (f == NULL) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        fprintf(f, "%s|%s|%s|%s\n",
                records[i].username,
                records[i].password_hash.hash,
                records[i].password_hash.salt,
                records[i].creation_time);
    }

    safe_fclose(&f);
    return true;
}

bool file_append_user(const UserRecord *record) {
    FILE *f = file_open_append(USERS_FILE);
    if (f == NULL) {
        return false;
    }

    fprintf(f, "%s|%s|%s|%s\n",
            record->username,
            record->password_hash.hash,
            record->password_hash.salt,
            record->creation_time);

    safe_fclose(&f);
    return true;
}

bool file_read_history(HistoryEntry entries[], int *count) {
    FILE *f = file_open_read(HISTORY_FILE);
    if (f == NULL) {
        *count = 0;
        return true;
    }

    *count = 0;
    while (*count < MAX_HISTORY_ENTRIES) {
        if (fscanf(f, "%15[^|]|%63[^|]|%19[^\n]\n",
                     entries[*count].timestamp,
                     entries[*count].username,
                     entries[*count].event_type) != 3) {
            break;
        }
        (*count)++;
    }

    safe_fclose(&f);
    return true;
}

bool file_append_history(const HistoryEntry *entry) {
    FILE *f = file_open_append(HISTORY_FILE);
    if (f == NULL) {
        return false;
    }

    fprintf(f, "%s|%s|%s\n",
            entry->timestamp,
            entry->username,
            entry->event_type);

    safe_fclose(&f);
    return true;
}

bool file_read_logs(SecurityLog entries[], int *count) {
    FILE *f = file_open_read(LOGS_FILE);
    if (f == NULL) {
        *count = 0;
        return true;
    }

    *count = 0;
    while (*count < MAX_LOG_ENTRIES) {
        if (fscanf(f, "%15[^|]|%255[^|]\n",
                     entries[*count].timestamp,
                     entries[*count].description) != 2) {
            break;
        }
        (*count)++;
    }

    safe_fclose(&f);
    return true;
}

bool file_append_log(const SecurityLog *entry) {
    FILE *f = file_open_append(LOGS_FILE);
    if (f == NULL) {
        return false;
    }

    fprintf(f, "%s|%s\n", entry->timestamp, entry->description);

    safe_fclose(&f);
    return true;
}

bool file_delete_user(const char *username) {
    FILE *f = file_open_read(USERS_FILE);
    if (f == NULL) {
        return false;
    }

    UserRecord temp_records[MAX_USERS];
    int count = 0;

    while (1) {
        if (fscanf(f, "%63[^|]|%63[^|]|%10s|%19[^\n]\n",
                    temp_records[count].username,
                    temp_records[count].password_hash.hash,
                    temp_records[count].password_hash.salt,
                    temp_records[count].creation_time) != 4) {
            break;
        }
        /* Skip the record if it matches the username to delete */
        if (strcmp(temp_records[count].username, username) != 0) {
            count++;
        }
    }

    safe_fclose(&f);

    /* Rewrite the file without the deleted user */
    if (!file_write_users(temp_records, count)) {
        return false;
    }

    return true;
}

bool file_rename_temp(const char *temp_name, const char *real_name) {
    if (remove(real_name) != 0) {
        return false;
    }
    if (rename(temp_name, real_name) != 0) {
        return false;
    }
    return true;
}

void file_safe_delete(const char *filename) {
    remove(filename);
}

FILE *file_open_read(const char *filename) {
    FILE *f = fopen(filename, FILE_MODE_READ);
    if (f == NULL) {
        pause_message("Error: Could not open file for reading.");
    }
    return f;
}

FILE *file_open_write(const char *filename) {
    FILE *f = fopen(filename, FILE_MODE_WRITE);
    if (f == NULL) {
        pause_message("Error: Could not create file for writing.");
    }
    return f;
}

FILE *file_open_append(const char *filename) {
    FILE *f = fopen(filename, FILE_MODE_APPEND);
    if (f == NULL) {
        pause_message("Error: Could not open file for appending.");
    }
    return f;
}
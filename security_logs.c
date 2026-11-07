#include "security_logs.h"
#include "file_manager.h"

bool security_log_event(const char *description) {
    SecurityLog entry;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (t != NULL) {
        strftime(entry.timestamp, MAX_LINE_LENGTH + 1, "%Y-%m-%d %H:%M:%S", t);
    } else {
        snprintf(entry.timestamp, MAX_LINE_LENGTH + 1, "unknown");
    }

    strncpy(entry.description, description, MAX_LINE_LENGTH);
    entry.description[MAX_LINE_LENGTH] = '\0';

    return file_append_log(&entry);
}

bool security_logs_view_all(SecurityLog entries[], int *count) {
    return file_read_logs(entries, count);
}

bool security_logs_search(const char *keyword, SecurityLog results[], int *count) {
    SecurityLog all[MAX_LOG_ENTRIES];
    int total;

    if (!file_read_logs(all, &total)) {
        *count = 0;
        return false;
    }

    *count = 0;
    for (int i = 0; i < total; i++) {
        if (strstr(all[i].description, keyword) != NULL) {
            results[*count] = all[i];
            (*count)++;
        }
    }

    return *count > 0;
}
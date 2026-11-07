#include "history.h"
#include "file_manager.h"
#include "utils.h"

bool history_record_login(const char *username, bool success) {
    HistoryEntry entry;
    format_time(entry.timestamp, sizeof(entry.timestamp));
    strncpy(entry.username, username, MAX_USERNAME_LENGTH);
    entry.username[MAX_USERNAME_LENGTH] = '\0';

    if (success) {
        strncpy(entry.event_type, "LOGIN_SUCCESS", MAX_LINE_LENGTH);
    } else {
        strncpy(entry.event_type, "LOGIN_FAILED", MAX_LINE_LENGTH);
    }

    return file_append_history(&entry);
}

bool history_view_user_history(const char *username) {
    HistoryEntry entries[MAX_HISTORY_ENTRIES];
    int count;

    if (!file_read_history(entries, &count)) {
        return false;
    }

    printf("\n=== Login History for %s ===\n", username);
    printf("%-20s | %-15s | %s\n", "Timestamp", "Username", "Event Type");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].username, username) == 0) {
            printf("%-20s | %-15s | %s\n",
                   entries[i].timestamp,
                   entries[i].username,
                   entries[i].event_type);
        }
    }

    if (count == 0) {
        printf("No login history found for %s.\n", username);
    }

    return true;
}

bool history_read_all(HistoryEntry entries[], int *count) {
    return file_read_history(entries, count);
}
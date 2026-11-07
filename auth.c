#include "auth.h"
#include "utils.h"
#include "file_manager.h"
#include "password.h"
#include <stdio.h>
#include <string.h>

static AccountStatus g_lockout_statuses[MAX_USERS];
static int g_initialized = 0;

void auth_init_lockout(void) {
    if (g_initialized) return;
    memset(g_lockout_statuses, 0, sizeof(g_lockout_statuses));
    g_initialized = 1;
}

AccountStatus auth_get_status(const char *username) {
    AccountStatus status;
    memset(&status, 0, sizeof(AccountStatus));

    for (int i = 0; i < MAX_USERS; i++) {
        if (g_lockout_statuses[i].username[0] != '\0' &&
            strcmp(g_lockout_statuses[i].username, username) == 0) {
            status = g_lockout_statuses[i];
            return status;
        }
    }

    /* Check if user exists in users file */
    UserRecord users[MAX_USERS];
    int count;
    if (file_read_users(users, &count)) {
        for (int i = 0; i < count; i++) {
            if (strcmp(users[i].username, username) == 0) {
                /* User exists but has no lockout status */
                status.username[0] = '\0';
                status.failed_attempts = 0;
                status.locked = false;
                return status;
            }
        }
    }

    /* User not found */
    status.username[0] = '\0';
    return status;
}

void auth_check_lockout(const char *username) {
    AccountStatus status = auth_get_status(username);

    if (status.username[0] == '\0') return;

    if (status.locked) {
        time_t now = get_current_time();
        double diff = difftime(now, status.lockout_time);
        int minutes_passed = (int)(diff / 60);

        if (minutes_passed >= LOCKOUT_DURATION_MINUTES) {
            status.locked = false;
            status.failed_attempts = 0;
            auth_set_lockout(username, 0);
        }
    }
}

bool auth_attempt_login(const char *username, const char *password) {
    auth_check_lockout(username);

    AccountStatus status = auth_get_status(username);

    if (status.locked) {
        return false;
    }

    if (!auth_verify_credentials(username, password)) {
        auth_record_login_attempt(username, false);
        return false;
    }

    auth_record_login_attempt(username, true);
    auth_reset_failed_attempts(username);
    return true;
}

bool auth_verify_credentials(const char *username, const char *password) {
    UserRecord users[MAX_USERS];
    int count;

    if (file_read_users(users, &count) != true) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            PasswordHash stored = users[i].password_hash;
            return password_verify(password, &stored);
        }
    }

    return false;
}

void auth_record_login_attempt(const char *username, bool success) {
    AccountStatus status = auth_get_status(username);

    if (status.username[0] == '\0') {
        strncpy(status.username, username, MAX_USERNAME_LENGTH);
        status.username[MAX_USERNAME_LENGTH] = '\0';
        status.failed_attempts = 0;
    }

    if (success) {
        status.locked = false;
        status.failed_attempts = 0;
    } else {
        status.failed_attempts++;
        if (status.failed_attempts >= MAX_FAILED_ATTEMPTS) {
            status.locked = true;
            status.lockout_time = get_current_time();
        }
    }

    /* Update in memory */
    for (int i = 0; i < MAX_USERS; i++) {
        if (g_lockout_statuses[i].username[0] == '\0' ||
            strcmp(g_lockout_statuses[i].username, username) == 0) {
            g_lockout_statuses[i] = status;
            break;
        }
    }

    /* Record in history */
    HistoryEntry entry;
    format_time(entry.timestamp, sizeof(entry.timestamp));
    strncpy(entry.username, username, MAX_USERNAME_LENGTH);
    entry.username[MAX_USERNAME_LENGTH] = '\0';

    if (success) {
        strncpy(entry.event_type, "LOGIN_SUCCESS", MAX_LINE_LENGTH);
    } else {
        strncpy(entry.event_type, "LOGIN_FAILED", MAX_LINE_LENGTH);
    }

    file_append_history(&entry);
}

void auth_set_lockout(const char *username, int attempts) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (strcmp(g_lockout_statuses[i].username, username) == 0) {
            g_lockout_statuses[i].failed_attempts = attempts;
            return;
        }
    }
}

void auth_reset_failed_attempts(const char *username) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (strcmp(g_lockout_statuses[i].username, username) == 0) {
            g_lockout_statuses[i].failed_attempts = 0;
            g_lockout_statuses[i].locked = false;
            return;
        }
    }
}
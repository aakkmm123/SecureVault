#include "users.h"
#include "file_manager.h"
#include "auth.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

bool user_register(const char *username, const char *password) {
    /* Validate username */
    if (strlen(username) < MIN_USERNAME_LENGTH ||
        strlen(username) > MAX_USERNAME_LENGTH) {
        pause_message("Username must be between 4 and 31 characters.");
        return false;
    }

    /* Check for allowed characters in username */
    for (const char *p = username; *p != '\0'; p++) {
        if (!is_valid_character(*p, false)) {
            pause_message("Username contains invalid characters. "
                          "Use only letters, digits, and underscores.");
            return false;
        }
    }

    /* Check for duplicate username */
    UserRecord users[MAX_USERS];
    int count;
    if (file_read_users(users, &count)) {
        for (int i = 0; i < count; i++) {
            if (strcmp(users[i].username, username) == 0) {
                pause_message("Error: Username already exists.");
                return false;
            }
        }
    }

    /* Check password strength */
    char strength_output[128];
    password_strength_check(password, strength_output, sizeof(strength_output));

    if (strstr(strength_output, "WEAK") != NULL) {
        char msg[256];
        snprintf(msg, sizeof(msg),
                 "Password strength check failed: %s", strength_output);
        pause_message(msg);
        /* Still allow registration with weak password for educational purposes,
         * but warn the user */
    }

    /* Generate salt and hash password */
    char salt[SALT_LENGTH + 1];
    password_generate_salt(salt, sizeof(salt));

    PasswordHash hash;
    password_hash_generate(password, salt, hash.hash, sizeof(hash.hash));
    strncpy(hash.salt, salt, SALT_LENGTH);
    hash.salt[SALT_LENGTH] = '\0';

    /* Create user record */
    UserRecord user;
    strncpy(user.username, username, MAX_USERNAME_LENGTH);
    user.username[MAX_USERNAME_LENGTH] = '\0';
    user.password_hash = hash;
    format_time(user.creation_time, sizeof(user.creation_time));

    /* Save user */
    if (!file_append_user(&user)) {
        pause_message("Error: Could not save user to file.");
        return false;
    }

    /* Log registration event */
    SecurityLog log_entry;
    format_time(log_entry.timestamp, sizeof(log_entry.timestamp));
    snprintf(log_entry.description, MAX_LINE_LENGTH,
             "User registered: %s", username);
    file_append_log(&log_entry);

    printf("User '%s' registered successfully.\n", username);
    return true;
}

bool user_login(const char *username, const char *password) {
    /* Check lockout first */
    auth_check_lockout(username);

    /* Verify credentials */
    if (!auth_verify_credentials(username, password)) {
        auth_record_login_attempt(username, false);
        return false;
    }

    /* Record successful login */
    auth_record_login_attempt(username, true);
    return true;
}

bool user_change_password(const char *username, const char *new_password,
                          const char *old_password) {
    /* Verify current password */
    if (!auth_verify_credentials(username, old_password)) {
        pause_message("Error: Current password is incorrect.");
        return false;
    }

    /* Check password strength */
    char strength_output[128];
    password_strength_check(new_password, strength_output, sizeof(strength_output));

    if (strstr(strength_output, "WEAK") != NULL) {
        char msg[256];
        snprintf(msg, sizeof(msg),
                 "New password too weak: %s", strength_output);
        pause_message(msg);
        return false;
    }

    /* Generate salt and hash new password */
    char salt[SALT_LENGTH + 1];
    password_generate_salt(salt, sizeof(salt));

    PasswordHash hash;
    password_hash_generate(new_password, salt, hash.hash, sizeof(hash.hash));
    strncpy(hash.salt, salt, SALT_LENGTH);
    hash.salt[SALT_LENGTH] = '\0';

    /* Update user record in users.txt */
    UserRecord users[MAX_USERS];
    int count;

    if (file_read_users(users, &count) != true) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            /* Update password hash and salt, keep username and creation time */
            users[i].password_hash = hash;

            /* Write all users back */
            if (!file_write_users(users, count)) {
                pause_message("Error: Could not update user password.");
                return false;
            }

            /* Log password change event */
            SecurityLog log_entry;
            format_time(log_entry.timestamp, sizeof(log_entry.timestamp));
            snprintf(log_entry.description, MAX_LINE_LENGTH,
                     "Password changed for user: %s", username);
            file_append_log(&log_entry);

            printf("Password for '%s' changed successfully.\n", username);
            return true;
        }
    }

    pause_message("Error: User not found.");
    return false;
}

bool user_delete(const char *username) {
    /* Confirm deletion */
    char msg[256];
    snprintf(msg, sizeof(msg),
             "Are you sure you want to delete the account for '%s'? (y/n): ", username);
    printf("%s", msg);

    char choice[MAX_INPUT_LENGTH];
    if (read_line(choice, sizeof(choice)) != 0) {
        return false;
    }

    if (tolower(choice[0]) != 'y') {
        printf("Account deletion cancelled.\n");
        return false;
    }

    /* Delete user from file */
    if (!file_delete_user(username)) {
        pause_message("Error: Could not delete user.");
        return false;
    }

    /* Reset lockout status if present */
    auth_reset_failed_attempts(username);

    /* Log deletion event */
    SecurityLog log_entry;
    format_time(log_entry.timestamp, sizeof(log_entry.timestamp));
    snprintf(log_entry.description, MAX_LINE_LENGTH,
             "Account deleted: %s", username);
    file_append_log(&log_entry);

    printf("Account for '%s' deleted successfully.\n", username);
    return true;
}

bool user_exists(const char *username) {
    UserRecord users[MAX_USERS];
    int count;

    if (file_read_users(users, &count)) {
        for (int i = 0; i < count; i++) {
            if (strcmp(users[i].username, username) == 0) {
                return true;
            }
        }
    }

    return false;
}

void user_list_all(UserRecord records[], int *count) {
    *count = 0;
    file_read_users(records, count);
}

bool user_search(const char *username, UserRecord *result) {
    UserRecord users[MAX_USERS];
    int count;

    if (file_read_users(users, &count)) {
        for (int i = 0; i < count; i++) {
            if (strcmp(users[i].username, username) == 0) {
                *result = users[i];
                return true;
            }
        }
    }

    return false;
}

void user_get_info(const char *username, UserRecord *result) {
    memset(result, 0, sizeof(UserRecord));

    UserRecord users[MAX_USERS];
    int count;

    if (file_read_users(users, &count)) {
        for (int i = 0; i < count; i++) {
            if (strcmp(users[i].username, username) == 0) {
                strncpy(result->username, users[i].username, MAX_USERNAME_LENGTH);
                result->username[MAX_USERNAME_LENGTH] = '\0';
                result->password_hash = users[i].password_hash;
                strncpy(result->creation_time, users[i].creation_time, MAX_LINE_LENGTH);
                result->creation_time[MAX_LINE_LENGTH] = '\0';
                return;
            }
        }
    }
}
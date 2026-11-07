#include "config.h"
#include "utils.h"
#include "auth.h"
#include "users.h"
#include "history.h"
#include "security_logs.h"
#include <stdio.h>

static int g_user_role = ROLE_USER;
static char g_logged_in_user[MAX_USERNAME_LENGTH + 1];

void main_menu(void);
void user_menu(void);
void admin_menu(void);
void unauthenticated_menu(void);
void handle_registration(void);
void handle_login(void);
void handle_password_check(void);
void handle_account_management(void);
void handle_change_password(void);
void handle_user_search(void);
void handle_view_history(void);
void handle_view_logs(void);
void handle_admin_view_accounts(void);
void handle_admin_search_users(void);
void handle_admin_view_logs(void);
void handle_logout(void);

int main(void) {
    g_user_role = ROLE_USER;
    g_logged_in_user[0] = '\0';

    while (1) {
        if (g_logged_in_user[0] == '\0') {
            unauthenticated_menu();
        } else if (g_user_role == ROLE_ADMIN) {
            admin_menu();
        } else {
            user_menu();
        }
    }

    return 0;
}

void unauthenticated_menu(void) {
    int choice;

    printf("\n=== SecureVault Terminal Security Management ===\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Password Strength Checker\n");
    printf("4. Administrator Features\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");

    choice = get_user_choice(1, 5);

    switch (choice) {
        case 1:
            handle_registration();
            break;
        case 2:
            handle_login();
            break;
        case 3:
            handle_password_check();
            break;
        case 4:
            if (g_user_role == ROLE_ADMIN) {
                admin_menu();
            } else {
                printf("Please login as administrator first.\n");
                wait_for_keypress();
            }
            break;
        case 5:
            printf("Goodbye!\n");
            exit(0);
    }
}

void user_menu(void) {
    int choice;

    printf("\n=== SecureVault - User Menu ===\n");
    printf("Welcome, %s\n", g_logged_in_user);
    printf("1. Change Password\n");
    printf("2. View Login History\n");
    printf("3. View Security Logs\n");
    printf("4. Account Management\n");
    printf("5. Logout\n");
    printf("Enter your choice: ");

    choice = get_user_choice(1, 5);

    switch (choice) {
        case 1:
            handle_change_password();
            break;
        case 2:
            history_view_user_history(g_logged_in_user);
            break;
        case 3:
            /* View all security logs as authenticated user */
            {
                SecurityLog entries[MAX_LOG_ENTRIES];
                int count;
                if (security_logs_view_all(entries, &count)) {
                    printf("\n=== Security Logs ===\n");
                    printf("%-20s | %s\n", "Timestamp", "Description");
                    printf("----------------------------------------------\n");
                    for (int i = 0; i < count; i++) {
                        printf("%-20s | %s\n", entries[i].timestamp, entries[i].description);
                    }
                    if (count == 0) {
                        printf("No security logs found.\n");
                    }
                }
                wait_for_keypress();
            }
            break;
        case 4:
            handle_account_management();
            break;
        case 5:
            handle_logout();
            break;
    }
}

void admin_menu(void) {
    int choice;

    printf("\n=== SecureVault - Administrator Menu ===\n");
    printf("Welcome, Administrator (%s)\n", g_logged_in_user);
    printf("1. View Account Summaries\n");
    printf("2. Search Users\n");
    printf("3. View Security Logs\n");
    printf("4. Change User Password\n");
    printf("5. User Management\n");
    printf("6. Logout\n");
    printf("Enter your choice: ");

    choice = get_user_choice(1, 6);

    switch (choice) {
        case 1:
            handle_admin_view_accounts();
            break;
        case 2:
            handle_admin_search_users();
            break;
        case 3:
            {
                SecurityLog entries[MAX_LOG_ENTRIES];
                int count;
                if (security_logs_view_all(entries, &count)) {
                    printf("\n=== Security Logs ===\n");
                    printf("%-20s | %s\n", "Timestamp", "Description");
                    printf("----------------------------------------------\n");
                    for (int i = 0; i < count; i++) {
                        printf("%-20s | %s\n", entries[i].timestamp, entries[i].description);
                    }
                    if (count == 0) {
                        printf("No security logs found.\n");
                    }
                }
                wait_for_keypress();
            }
            break;
        case 4:
            handle_change_password();
            break;
        case 5:
            handle_admin_user_management();
            break;
        case 6:
            handle_logout();
            break;
    }
}

void handle_registration(void) {
    char username[MAX_USERNAME_LENGTH + 1];
    char password[MAX_PASSWORD_LENGTH + 1];
    char confirm_password[MAX_PASSWORD_LENGTH + 1];

    printf("\n=== User Registration ===\n");
    printf("Enter new username: ");
    if (read_line(username, sizeof(username)) != 0) return;

    printf("Enter password: ");
    if (read_password(password, sizeof(password)) != 0) return;

    printf("Confirm password: ");
    if (read_password(confirm_password, sizeof(confirm_password)) != 0) return;

    if (strcmp(password, confirm_password) != 0) {
        pause_message("Error: Passwords do not match.");
        return;
    }

    /* Check password strength */
    char strength[128];
    password_strength_check(password, strength, sizeof(strength));
    printf("Password strength: %s\n", strength);

    if (!user_register(username, password)) {
        /* Registration already printed error message */
    }
}

void handle_login(void) {
    char username[MAX_USERNAME_LENGTH + 1];
    char password[MAX_PASSWORD_LENGTH + 1];

    printf("\n=== User Login ===\n");
    printf("Enter username: ");
    if (read_line(username, sizeof(username)) != 0) return;

    printf("Enter password: ");
    if (read_password(password, sizeof(password)) != 0) return;

    if (auth_attempt_login(username, password)) {
        strncpy(g_logged_in_user, username, MAX_USERNAME_LENGTH);
        g_logged_in_user[MAX_USERNAME_LENGTH] = '\0';
        /* Determine role - for now, check if username is "admin" */
        if (strcmp(username, "admin") == 0) {
            g_user_role = ROLE_ADMIN;
        } else {
            g_user_role = ROLE_USER;
        }
        printf("Login successful! Welcome, %s.\n", username);

        /* Log login event */
        SecurityLog log_entry;
        format_time(log_entry.timestamp, sizeof(log_entry.timestamp));
        snprintf(log_entry.description, MAX_LINE_LENGTH,
                 "User logged in: %s", username);
        file_append_log(&log_entry);
    } else {
        printf("Login failed. Invalid username or password.\n");
    }
}

void handle_password_check(void) {
    char password[MAX_PASSWORD_LENGTH + 1];

    printf("\n=== Password Strength Checker ===\n");
    printf("Enter password to check: ");
    if (read_password(password, sizeof(password)) != 0) return;

    char strength[128];
    password_strength_check(password, strength, sizeof(strength));
    printf("Password strength: %s\n", strength);
}

void handle_account_management(void) {
    int choice;

    printf("\n=== Account Management ===\n");
    printf("1. Search for an account\n");
    printf("2. Update account information\n");
    printf("3. Change password\n");
    printf("4. Delete account\n");
    printf("Enter your choice: ");

    choice = get_user_choice(1, 4);

    switch (choice) {
        case 1:
        {
            char username[MAX_USERNAME_LENGTH + 1];
            printf("Enter username to search: ");
            if (read_line(username, sizeof(username)) != 0) return;

            UserRecord user;
            if (user_search(username, &user)) {
                printf("\n=== Account Found ===\n");
                printf("Username: %s\n", user.username);
                printf("Account created: %s\n", user.creation_time);
                /* Do not display password hash */
                printf("Account information displayed above.\n");
            } else {
                printf("Account not found.\n");
            }
            wait_for_keypress();
            break;
        }
        case 2:
            /* Update account - currently limited options */
            printf("Account update functionality coming soon.\n");
            wait_for_keypress();
            break;
        case 3:
            handle_change_password();
            break;
        case 4:
        {
            char username[MAX_USERNAME_LENGTH + 1];
            printf("Enter username to delete: ");
            if (read_line(username, sizeof(username)) != 0) return;

            user_delete(username);
            wait_for_keypress();
            break;
        }
    }
}

void handle_change_password(void) {
    char new_password[MAX_PASSWORD_LENGTH + 1];
    char old_password[MAX_PASSWORD_LENGTH + 1];

    printf("\n=== Change Password ===\n");
    printf("Enter current password: ");
    if (read_password(old_password, sizeof(old_password)) != 0) return;

    printf("Enter new password: ");
    if (read_password(new_password, sizeof(new_password)) != 0) return;

    if (!user_change_password(g_logged_in_user, new_password, old_password)) {
        printf("Failed to change password.\n");
    }
    wait_for_keypress();
}

void handle_user_search(void) {
    char username[MAX_USERNAME_LENGTH + 1];

    printf("Enter username to search: ");
    if (read_line(username, sizeof(username)) != 0) return;

    UserRecord user;
    if (user_search(username, &user)) {
        printf("\n=== Account Found ===\n");
        printf("Username: %s\n", user.username);
        printf("Account created: %s\n", user.creation_time);
        printf("Do not display password hash for security.\n");
    } else {
        printf("Account not found.\n");
    }
    wait_for_keypress();
}

void handle_admin_view_accounts(void) {
    UserRecord users[MAX_USERS];
    int count;

    printf("\n=== Account Summaries ===\n");
    user_list_all(users, &count);

    printf("%-20s | %-30s | %s\n", "Username", "Created", "Role");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-20s | %-30s | %s\n",
               users[i].username,
               users[i].creation_time,
               "User"); /* In a real system, check actual role */
    }

    if (count == 0) {
        printf("No accounts found.\n");
    }

    wait_for_keypress();
}

void handle_admin_search_users(void) {
    char username[MAX_USERNAME_LENGTH + 1];

    printf("Enter username to search: ");
    if (read_line(username, sizeof(username)) != 0) return;

    UserRecord user;
    if (user_search(username, &user)) {
        printf("\n=== User Found ===\n");
        printf("Username: %s\n", user.username);
        printf("Account created: %s\n", user.creation_time);
    } else {
        printf("User not found.\n");
    }

    wait_for_keypress();
}

void handle_admin_view_logs(void) {
    SecurityLog entries[MAX_LOG_ENTRIES];
    int count;

    printf("\n=== Security Event Logs ===\n");
    if (security_logs_view_all(entries, &count)) {
        printf("%-20s | %s\n", "Timestamp", "Description");
        printf("----------------------------------------------\n");
        for (int i = 0; i < count; i++) {
            printf("%-20s | %s\n", entries[i].timestamp, entries[i].description);
        }
        if (count == 0) {
            printf("No security logs found.\n");
        }
    }

    wait_for_keypress();
}

void handle_admin_user_management(void) {
    int choice;

    printf("\n=== Administrator User Management ===\n");
    printf("1. Change user password\n");
    printf("2. Delete user account\n");
    printf("3. Search user\n");
    printf("Enter your choice: ");

    choice = get_user_choice(1, 3);

    switch (choice) {
        case 1:
        {
            char username[MAX_USERNAME_LENGTH + 1];
            char new_password[MAX_PASSWORD_LENGTH + 1];
            char old_password[MAX_PASSWORD_LENGTH + 1];

            printf("Enter username: ");
            if (read_line(username, sizeof(username)) != 0) return;

            printf("Enter current password: ");
            if (read_password(old_password, sizeof(old_password)) != 0) return;

            printf("Enter new password: ");
            if (read_password(new_password, sizeof(new_password)) != 0) return;

            /* Temporarily authenticate as the user */
            UserRecord temp_user;
            if (user_search(username, &temp_user)) {
                if (!user_change_password(username, new_password, old_password)) {
                    printf("Failed to change password.\n");
                }
            } else {
                printf("User not found.\n");
            }
            wait_for_keypress();
            break;
        }
        case 2:
        {
            char username[MAX_USERNAME_LENGTH + 1];
            printf("Enter username to delete: ");
            if (read_line(username, sizeof(username)) != 0) return;

            user_delete(username);
            wait_for_keypress();
            break;
        }
        case 3:
        {
            char username[MAX_USERNAME_LENGTH + 1];
            printf("Enter username to search: ");
            if (read_line(username, sizeof(username)) != 0) return;

            UserRecord user;
            if (user_search(username, &user)) {
                printf("\n=== User Found ===\n");
                printf("Username: %s\n", user.username);
                printf("Account created: %s\n", user.creation_time);
            } else {
                printf("User not found.\n");
            }
            wait_for_keypress();
            break;
        }
    }
}

void handle_logout(void) {
    if (g_logged_in_user[0] != '\0') {
        /* Log logout event */
        SecurityLog log_entry;
        format_time(log_entry.timestamp, sizeof(log_entry.timestamp));
        snprintf(log_entry.description, MAX_LINE_LENGTH,
                 "User logged out: %s", g_logged_in_user);
        file_append_log(&log_entry);
    }

    strncpy(g_logged_in_user, "", MAX_USERNAME_LENGTH);
    g_logged_in_user[MAX_USERNAME_LENGTH] = '\0';
    g_user_role = ROLE_USER;
    printf("You have been logged out.\n");
}
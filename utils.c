#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void wait_for_keypress(void) {
    printf("\nPress Enter to continue...");
    clear_input_buffer();
}

void format_time(char *buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    if (t != NULL) {
        strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", t);
    } else {
        snprintf(buffer, buffer_size, "unknown");
    }
}

time_t get_current_time(void) {
    return time(NULL);
}

bool is_valid_character(char c, bool allow_spaces) {
    if (allow_spaces) {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               (c >= '0' && c <= '9') ||
               c == ' ' || c == '_' || c == '-' || c == '.';
    } else {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               (c >= '0' && c <= '9') ||
               c == '_';
    }
}

int read_line(char *buffer, size_t buffer_size) {
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if (len > 0 && buffer[len - 1] == '\r') {
            buffer[len - 1] = '\0';
        }
        return 0;
    }
    return -1;
}

int read_password(char *buffer, size_t buffer_size) {
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if (len > 0 && buffer[len - 1] == '\r') {
            buffer[len - 1] = '\0';
        }
        return 0;
    }
    return -1;
}

void pause_message(const char *message) {
    if (message != NULL) {
        printf("%s\n", message);
    }
    wait_for_keypress();
}

int get_user_choice(int min, int max) {
    int choice;
    while (1) {
        if (scanf("%d", &choice) == 1) {
            clear_input_buffer();
            if (choice >= min && choice <= max) {
                return choice;
            }
        }
        clear_input_buffer();
        printf("Invalid choice. Enter a number between %d and %d: ", min, max);
    }
}

void truncate_string(char *str, size_t max_len) {
    size_t len = strlen(str);
    if (len > max_len) {
        str[max_len] = '\0';
    }
}

bool file_exists(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f != NULL) {
        fclose(f);
        return true;
    }
    return false;
}

void safe_fclose(FILE **file) {
    if (file != NULL && *file != NULL) {
        fclose(*file);
        *file = NULL;
    }
}
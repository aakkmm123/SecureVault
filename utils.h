#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include <stdbool.h>

void clear_input_buffer(void);
void wait_for_keypress(void);
void format_time(char *buffer, size_t buffer_size);
time_t get_current_time(void);
bool is_valid_character(char c, bool allow_spaces);
int read_line(char *buffer, size_t buffer_size);
int read_password(char *buffer, size_t buffer_size);
void pause_message(const char *message);
int get_user_choice(int min, int max);
void truncate_string(char *str, size_t max_len);
bool file_exists(const char *filename);
void safe_fclose(FILE **file);

#endif
#ifndef PASSWORD_H
#define PASSWORD_H

#include "config.h"
#include <stdbool.h>

typedef struct {
    char hash[65];
    char salt[SALT_LENGTH + 1];
} PasswordHash;

void password_hash_generate(const char *password, const char *salt, char *hash_output, size_t hash_size);
void password_hash_from_string(const char *password_hash_str, PasswordHash *ph);
bool password_verify(const char *password, const PasswordHash *stored_hash);
void password_strength_check(const char *password, char *output, size_t output_size);
bool password_has_minimum_length(const char *password);
bool password_has_uppercase(const char *password);
bool password_has_lowercase(const char *password);
bool password_has_digit(const char *password);
bool password_has_special(const char *password);
void password_generate_salt(char *salt_output, size_t salt_size);

#endif
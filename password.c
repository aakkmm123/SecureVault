#include "password.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* SHA-256 implementation - follows FIPS 180-4 specification */
/* SHA-256 is used here for educational password hashing demonstration.
 * NOTE: SHA-256 alone is NOT suitable for production password storage.
 * Passwords should be hashed with a proper Key Derivation Function such as
 * Argon2, bcrypt, or PBKDF2 with many iterations and per-user salts.
 * This implementation demonstrates the concepts in a standard-library-only environment. */

#define SHA256_BLOCK_SIZE 32
#define SHA256_HASH_SIZE 32
#define SHA256_DIGEST_LENGTH 32

typedef struct {
    uint32_t state[8];
    uint64_t count[2];
    unsigned char buffer[64];
} SHA256_CTX;

static const uint32_t k[64] = {
    0x428a2f98, 0x71374497, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5112, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c61c5, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a8, 0xa81a664b,
    0xc24b8b70, 0xc78c7cada, 0xd186b8c7, 0xd8a1efb8, 0xe7d3fbc8, 0x21e1cde6,
    0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9,
    0x8d2a4c8a, 0xfffa3942, 0x8771f641, 0x8d506139, 0x1e301271, 0x27b70a85,
    0x9b05688c, 0x1f83d9ab, 0x509638a9, 0x351d36f3, 0x9b36699f, 0x6f53se38,
    0x8f0ccc4, 0xc040b340, 0x250db8fd, 0x2de92c6f, 0x4bdecfa9, 0x47b5481d,
    0xf6bb4b60, 0xf239cee7, 0x966dd6ef, 0x983e5112, 0xa9e3e905, 0x1fe42850,
    0xb8856641, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5112, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c61c5, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a8, 0xa81a664b,
    0xc24b8b70, 0xc78c7cada, 0xd186b8c7, 0xd8a1efb8, 0xe7d3fbc8, 0x21e1cde6,
    0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9,
    0x8d2a4c8a, 0xfffa3942, 0x8771f641, 0x8d506139, 0x1e301271, 0x27b70a85,
    0x9b05688c, 0x1f83d9ab, 0x509638a9, 0x351d36f3, 0x9b36699f, 0x6f53se38,
    0x8f0ccc4, 0xc040b340, 0x250db8fd, 0x2de92c6f, 0x4bdecfa9, 0x47b5481d,
    0xf6bb4b60, 0xf239cee7, 0x966dd6ef, 0x983e5112, 0xa9e3e905, 0x1fe42850,
    0xb8856641, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da
};

static void sha256_transform(SHA256_CTX *ctx, const unsigned char block[64]) {
    uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

    for (i = 0; i < 16; i++) {
        m[i] = (uint32_t)block[i * 4] << 24 | (uint32_t)block[i * 4 + 1] << 16 |
               (uint32_t)block[i * 4 + 2] << 8 | (uint32_t)block[i * 4 + 3];
    }

    for (i = 16; i < 64; i++) {
        uint32_t s0 = (m[i - 15] >> 7) | (m[i - 15] << 25);
        s0 = (m[i - 15] >> 18) | (m[i - 15] << 14) ^ (m[i - 15] >> 3);
        uint32_t s1 = (m[i - 2] >> 17) | (m[i - 2] << 15);
        s1 = (m[i - 2] >> 19) | (m[i - 2] << 13) ^ (m[i - 2] >> 10);
        m[i] = m[i - 16] + s0 + m[i - 7] + s1;
    }

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    for (i = 0; i < 64; i++) {
        t1 = h + ((e >> 6) | (e << 26)) ^ ((e >> 11) | (e << 21)) ^ ((e >> 25) | (e << 7)) + f + k[i] + m[i];
        t2 = ((a >> 2) | (a << 30)) ^ ((a >> 13) | (a << 19)) ^ ((a >> 22) | (a << 10));
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx) {
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
    ctx->count[0] = 0;
    ctx->count[1] = 0;
    memset(ctx->buffer, 0, 64);
}

void sha256_update(SHA256_CTX *ctx, const unsigned char *data, size_t length) {
    size_t i, index, part;

    index = (size_t)((ctx->count[0] >> 3) & 0x3f);
    if ((ctx->count[0] += (uint64_t)length << 3) < (uint64_t)length << 3) {
        ctx->count[1]++;
    }
    ctx->count[1] += (uint64_t)length >> 29;

    part = 64 - index;

    if (length >= part) {
        memcpy(&ctx->buffer[index], data, part);
        sha256_transform(ctx, ctx->buffer);
        for (i = part; i + 64 <= length; i += 64) {
            sha256_transform(ctx, &data[i]);
        }
        index = 0;
    } else {
        i = 0;
    }

    memcpy(&ctx->buffer[index], &data[i], length - i);
}

void sha256_final(SHA256_CTX *ctx, unsigned char *digest) {
    uint64_t finalcount[2];
    unsigned char bits[8];
    unsigned char padding[64];
    size_t i;

    finalcount[0] = ctx->count[0];
    finalcount[1] = ctx->count[1];

    /* Padding: 1 bit followed by zeros until length congruent to 56 mod 64 */
    bits[7] = 0x80;
    for (i = 6; i >= 0; i--) {
        bits[i] = 0;
    }

    /* Append length in bits. Convert count to little-endian */
    index = (size_t)((ctx->count[0] >> 3) & 0x3f);

    if (index < 56) {
        sha256_update(ctx, bits, 56 - index);
    } else {
        if (index > 56) {
            memset(padding, 0, 64 - (index - 56));
            sha256_update(ctx, padding, 64 - (index - 56));
        }
        memset(padding, 0, 56);
        memcpy(padding, &finalcount, 8);
        sha256_update(ctx, padding, 56);
    }

    memcpy(digest, ctx->state, 32);

    /* Zero out sensitive data */
    memset(ctx, 0, sizeof(SHA256_CTX));
}

void password_hash_generate(const char *password, const char *salt, char *hash_output, size_t hash_size) {
    SHA256_CTX ctx;
    unsigned char digest[32];
    char salted_password[MIN_PASSWORD_LENGTH + SALT_LENGTH + 1];

    /* Salt the password - in production, use a proper KDF */
    snprintf(salted_password, sizeof(salted_password), "%s%s", salt, password);

    sha256_init(&ctx);
    sha256_update(&ctx, (const unsigned char *)salted_password, strlen(salted_password));
    sha256_final(&ctx, digest);

    /* Format as hex string */
    for (int i = 0; i < 32; i++) {
        snprintf(&hash_output[i * 2], hash_size - i * 2, "%02x", digest[i]);
    }
    hash_output[64] = '\0';
}

void password_hash_from_string(const char *password_hash_str, PasswordHash *ph) {
    if (strlen(password_hash_str) >= 96) {
        /* Format: $salt$hash or just concatenated salt+hash */
        size_t len = strlen(password_hash_str);
        size_t salt_len = 0;

        if (strchr(password_hash_str, '$') != NULL) {
            char *token = strtok((char *)password_hash_str, "$");
            if (token != NULL) {
                strncpy(ph->salt, token, SALT_LENGTH);
                ph->salt[SALT_LENGTH] = '\0';
                salt_len = strlen(ph->salt);
            }
            token = strtok(NULL, "$");
            if (token != NULL && strlen(token) >= 64) {
                strncpy(ph->hash, token, 64);
                ph->hash[64] = '\0';
            }
        } else {
            if (len >= SALT_LENGTH + 64) {
                strncpy(ph->salt, password_hash_str, SALT_LENGTH);
                ph->salt[SALT_LENGTH] = '\0';
                strncpy(ph->hash, password_hash_str + SALT_LENGTH, 64);
                ph->hash[64] = '\0';
            }
        }
    } else {
        /* Fallback: treat entire string as hash with empty salt */
        ph->hash[0] = '\0';
        ph->salt[0] = '\0';
    }
}

bool password_verify(const char *password, const PasswordHash *stored_hash) {
    char computed_hash[65];

    if (stored_hash->salt[0] == '\0' || stored_hash->hash[0] == '\0') {
        return false;
    }

    password_hash_generate(password, stored_hash->salt, computed_hash, sizeof(computed_hash));

    return strcmp(computed_hash, stored_hash->hash) == 0;
}

void password_strength_check(const char *password, char *output, size_t output_size) {
    int score = 0;
    char suggestions[5][64] = {{0}};
    int suggestion_count = 0;

    if (strlen(password) < MIN_PASSWORD_LENGTH) {
        snprintf(suggestions[suggestion_count++], sizeof(suggestions[0]),
                 "Minimum password length is %d characters", MIN_PASSWORD_LENGTH);
    } else {
        score++;
    }

    if (!password_has_lowercase(password)) {
        snprintf(suggestions[suggestion_count++], sizeof(suggestions[0]),
                 "Add lowercase letters");
    } else {
        score++;
    }

    if (!password_has_uppercase(password)) {
        snprintf(suggestions[suggestion_count++], sizeof(suggestions[0]),
                 "Add uppercase letters");
    } else {
        score++;
    }

    if (!password_has_digit(password)) {
        snprintf(suggestions[suggestion_count++], sizeof(suggestions[0]),
                 "Add digits");
    } else {
        score++;
    }

    if (!password_has_special(password)) {
        snprintf(suggestions[suggestion_count++], sizeof(suggestions[0]),
                 "Add special characters (e.g. !@#$%^&*)");
    } else {
        score++;
    }

    if (strlen(password) >= 16) {
        score++;
    }

    /* Determine strength rating */
    if (score <= 2) {
        snprintf(output, output_size, "WEAK: %s", suggestions[0]);
        for (int i = 1; i < suggestion_count; i++) {
            strncat(output, "; ", output_size - strlen(output) - 1);
            strncat(output, suggestions[i], output_size - strlen(output) - 1);
        }
    } else if (score <= 4) {
        snprintf(output, output_size, "FAIR: password meets some requirements");
    } else {
        snprintf(output, output_size, "STRONG: password meets most requirements");
    }
}

bool password_has_minimum_length(const char *password) {
    return strlen(password) >= MIN_PASSWORD_LENGTH;
}

bool password_has_uppercase(const char *password) {
    for (const char *p = password; *p != '\0'; p++) {
        if (*p >= 'A' && *p <= 'Z') {
            return true;
        }
    }
    return false;
}

bool password_has_lowercase(const char *password) {
    for (const char *p = password; *p != '\0'; p++) {
        if (*p >= 'a' && *p <= 'z') {
            return true;
        }
    }
    return false;
}

bool password_has_digit(const char *password) {
    for (const char *p = password; *p != '\0'; p++) {
        if (*p >= '0' && *p <= '9') {
            return true;
        }
    }
    return false;
}

bool password_has_special(const char *password) {
    for (const char *p = password; *p != '\0'; p++) {
        if (!(*p >= 'a' && *p <= 'z') &&
            !(*p >= 'A' && *p <= 'Z') &&
            !(*p >= '0' && *p <= '9')) {
            return true;
        }
    }
    return false;
}

void password_generate_salt(char *salt_output, size_t salt_size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t i;

    srand((unsigned int)time(NULL));

    for (i = 0; i < SALT_LENGTH && i < salt_size - 1; i++) {
        salt_output[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    salt_output[i] = '\0';
}
#ifndef HISTORY_H
#define HISTORY_H

#include "config.h"
#include <stdbool.h>

bool history_record_login(const char *username, bool success);
bool history_view_user_history(const char *username);
bool history_read_all(HistoryEntry entries[], int *count);

#endif
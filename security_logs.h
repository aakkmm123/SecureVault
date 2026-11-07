#ifndef SECURITY_LOGS_H
#define SECURITY_LOGS_H

#include "config.h"
#include <stdbool.h>

bool security_log_event(const char *description);
bool security_logs_view_all(SecurityLog entries[], int *count);
bool security_logs_search(const char *keyword, SecurityLog results[], int *count);

#endif
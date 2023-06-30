
#ifndef OGW_LOGGING_H
#define OGW_LOGGING_H

#include <stdio.h>

#define log_error(fmt_str, ...) printf("Error: " fmt_str "\n", ## __VA_ARGS__)
#define log_warn(fmt_str, ...)  printf("Warning: " fmt_str "\n", ## __VA_ARGS__)
#define log_info(fmt_str, ...)  printf("Info: " fmt_str "\n", ## __VA_ARGS__)

#endif

/*
 * log.h
 * Header file for KVSNS logging interfaces
 */

#ifndef KVSNS_LOG_H
#define KVSNS_LOG_H

#include <stdio.h>

/* @todo : Improve this simplistic implementation of logging. */
#define KVSNS_LOG(...) fprintf(stderr, __VA_ARGS__);

/* @todo: Add more logging levels. */
#define log_err KVSNS_LOG
#define log_info KVSNS_LOG
#define log_debug KVSNS_LOG
#define log_trace KVSNS_LOG

#endif
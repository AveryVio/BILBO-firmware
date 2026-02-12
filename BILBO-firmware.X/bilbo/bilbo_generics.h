#ifndef BILBO_GENERICS_H
#define BILBO_GENERICS_H

#include <bits/alltypes.h>

typedef uint16_t freq_t;

#define TUNE_RANGE_NULL 0
#define TUNE_RANGE_PLACEHOLDER 1
#define TUNE_RANGE_BASS 2
#define TUNE_RANGE_GUITAR 3
#define TUNE_RANGE_HIGH 4

#define TUNE_DIFF_LEVEL 0
#define TUNE_DIFF_UNDER_2 -2
#define TUNE_DIFF_UNDER_1 -1
#define TUNE_DIFF_OVER_2 2
#define TUNE_DIFF_OVER_1 1

typedef struct {
    uint8_t buffer[RN4870_READ_WRITE_BUFFER_SIZE];
    uint8_t length;
} lengthy_buffer;

typedef struct {
    uint8_t code;
} global_error_handle;

typedef struct {
    global_error_handle error_queue[9];
    uint8_t queue_length;
} global_error_queue;

typedef struct {
    uint8_t format;
} global_message_log_entry;

typedef struct {
    global_message_log_entry log[9];
    uint8_t log_length;
} global_message_log;

#define ADC_VREF (1.65f)
#define ADC_MAX_COUNT 4095

#endif
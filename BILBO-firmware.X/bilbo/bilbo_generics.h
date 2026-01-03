#ifndef BILBO_GENERICS_H
#define BILBO_GENERICS_H

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
    uint8_t code;
} global_error_handle;

typedef struct {
    global_error_handle error_queue[9];
    uint8_t queue_length;
} global_error_queue;


#endif
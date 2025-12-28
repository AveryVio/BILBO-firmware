#ifndef BILBO_TUNING_TYPES_H
#define BILBO_TUNING_TYPES_H

typedef struct {
    uint16_t reference_freq;
    uint8_t reference_freq_symbol[4];
    uint8_t semitones;
} tuning_profile;

#endif
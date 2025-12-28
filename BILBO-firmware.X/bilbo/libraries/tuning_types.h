#ifndef BILBO_TUNING_TYPES_H
#define BILBO_TUNING_TYPES_H

typedef struct {
    freq_t freq;
    uint8_t position_in_octive;
} musical_note ;

typedef struct {
    musical_note notes[12];
    int8_t octive_number;
} musical_octive;

typedef struct {
    musical_octive octives[11];
    uint8_t octive_count;
    musical_note reference_note;
    int8_t reference_note_octive;
} tuning_profile;

#endif
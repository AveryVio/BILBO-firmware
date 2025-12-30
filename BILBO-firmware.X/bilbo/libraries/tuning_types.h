#ifndef BILBO_TUNING_TYPES_H
#define BILBO_TUNING_TYPES_H

#define NOTE_DEF_NULL { .freq = 0, .position_in_octive  = 1 }
#define NOTE_DEF_UNDER { .freq = 0, .position_in_octive  = 2 }
#define NOTE_DEF_UNDER_OUT_OF_OCTIVES { .freq = 0, .position_in_octive  = 3 }

#define NOTE_DEF_OVER_ABSOLUTE { .freq = 0xFFFF, .position_in_octive  = 13 }
#define NOTE_DEF_OVER { .freq = 0xFFFF, .position_in_octive  = 14 }
#define NOTE_DEF_OVER_OUT_OF_OCTIVES { .freq = 0xFFFF, .position_in_octive  = 15 }

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
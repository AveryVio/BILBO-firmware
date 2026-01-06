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

#define NOTE_DEF(freqency, pos) { .freq = freqency, .position_in_octive  = pos }

#define NOTE_DEF_NULL { .freq = 0, .position_in_octive  = 0 }

#define NOTE_DEF_UNDER_ABSOLUTE { .freq = 0, .position_in_octive  = 1 }
#define NOTE_DEF_UNDER { .freq = 0, .position_in_octive  = 2 }
#define NOTE_DEF_UNDER_OUT_OF_OCTIVES { .freq = 0, .position_in_octive  = 3 }

#define NOTE_DEF_OVER_ABSOLUTE { .freq = 0xFFFF, .position_in_octive  = 13 }
#define NOTE_DEF_OVER { .freq = 0xFFFF, .position_in_octive  = 14 }
#define NOTE_DEF_OVER_OUT_OF_OCTIVES { .freq = 0xFFFF, .position_in_octive  = 15 }

#define OCTIVE_NULL { .notes = { \
    NOTE_DEF(0, 1),\
    NOTE_DEF(0, 2),\
    NOTE_DEF(0, 3),\
    NOTE_DEF(0, 4),\
    NOTE_DEF(0, 5),\
    NOTE_DEF(0, 6),\
    NOTE_DEF(0, 7),\
    NOTE_DEF(0, 8),\
    NOTE_DEF(0, 9),\
    NOTE_DEF(0, 10),\
    NOTE_DEF(0, 11),\
    NOTE_DEF(0, 12)\
}, .octive_number = 0 }

#define PROFILE_DEF_NULL { .octives = {}, .octive_count = 0, .reference_note = { .freq = 440, .position_in_octive = 10 }, .reference_note_octive = 4 }
#define PROFILE_DEF_DEFAULT { .octives = { (musical_octive) { .notes = { \
    NOTE_DEF(261.625, 1),\
    NOTE_DEF(277.182, 2),\
    NOTE_DEF(293.664, 3),\
    NOTE_DEF(311.126, 4),\
    NOTE_DEF(329.627, 5),\
    NOTE_DEF(349.228, 6),\
    NOTE_DEF(369.994, 7),\
    NOTE_DEF(391.995, 8),\
    NOTE_DEF(415.304, 9),\
    NOTE_DEF(440, 10),\
    NOTE_DEF(466.163, 11),\
    NOTE_DEF(493.883, 12)\
}, .octive_number = 4 } }, .octive_count = 1, .reference_note = NOTE_DEF(440, 10), .reference_note_octive = 4 }

#endif
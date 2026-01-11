#ifndef BILBO_TUNING_H
#define BILBO_TUNING_H

#include <bits/alltypes.h>
#include <math.h>
#include <stdlib.h>

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../libraries/tuning_types.h"

musical_octive calculate_single_octive(musical_note octive_reference_note);

tuning_profile calculate_base_tuning_profile(musical_note reference_note, int8_t reference_note_octive);


musical_octive find_currently_playing_note_octive(freq_t current_note_freq, tuning_profile* profile);

musical_note find_currently_playing_note(freq_t current_note_freq, tuning_profile *profile);


uint8_t calculate_cents(freq_t freq_one, freq_t freq_two);

uint8_t decide_tuning_level_in_cents(freq_t current_note_freq, freq_t calculated_note_freq);

#endif
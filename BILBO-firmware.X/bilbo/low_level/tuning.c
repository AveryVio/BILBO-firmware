#include "tuning.h"

#include <bits/alltypes.h>
#include <math.h>
#include <stdlib.h>

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"
#include "../libraries/tuning_types.h"

musical_octive calculate_single_octive(musical_note octive_reference_note){
    musical_octive calculated_octive;
    
    for(uint8_t j = 1; j < 12; j++){
        calculated_octive.notes[j].freq = (octive_reference_note.freq * (freq_t) pow( 2.00 , ((double) j - (double) octive_reference_note.position_in_octive)/12.00 ));
        calculated_octive.notes[j].position_in_octive = j;
    }
    calculated_octive.notes[12].freq = (octive_reference_note.freq * (freq_t) pow( 2.00 , ((double) 12 - (double) octive_reference_note.position_in_octive)/12.00 ));
    calculated_octive.notes[12].position_in_octive = 12;
    
    return calculated_octive;
}

// calculate profile from scratch
// new profile should be at least 5 octives (this does 7) including the octive of the ref note
tuning_profile calculate_base_tuning_profile(musical_note reference_note, int8_t reference_note_octive){
    tuning_profile profile;
    profile.reference_note = reference_note;
    profile.reference_note_octive = reference_note_octive;
    profile.octive_count = 7;
    
    for(int8_t i = 0; i <= 7; i++){
        musical_note current_reference_note = reference_note;
        current_reference_note.freq = (freq_t) ((double) reference_note.freq / ( pow(2.00,(double) (4 - i))));
        profile.octives[i] = calculate_single_octive(current_reference_note);
        
        /* potential different solution
        musical_octive current_octive;
        for(uint8_t j = 0; j <= 12; j++){
            current_octive.notes[j].freq = (freq_t) (reference_note.freq * pow( 2.00 , ((double) j - (double) reference_note.position_in_octive + ((double) (i - 1) * 12.00 ))/12.00 ));
        }
        current_octive.octive_number = profile_octives[i];
        
        profile.octives[i] = current_octive;*/
    }
    
    return profile;
}

musical_octive find_currently_playing_note_octive(freq_t current_note_freq, tuning_profile* profile){
    for(uint8_t i = 0; i < profile->octive_count; i++){
        if(abs((int8_t) current_note_freq - profile->octives[i].notes[profile->reference_note.position_in_octive].freq) > abs((int8_t) current_note_freq - profile->octives[i + 1].notes[profile->reference_note.position_in_octive].freq)) continue;
        return profile->octives[i];
    }
    return profile->octives[profile->octive_count];
}

/* TODO:
 * handle the error outputs
 */
musical_note find_currently_playing_note(freq_t current_note_freq, tuning_profile *profile){
    if(current_note_freq == 0) return (musical_note) NOTE_DEF_UNDER_ABSOLUTE;
    if(current_note_freq > 30000) return (musical_note) NOTE_DEF_OVER_ABSOLUTE;
    
    freq_t minimum_freq = (freq_t) (profile->reference_note.freq / ( pow(2.00, 5.00 + ((double) (12 - profile->reference_note.position_in_octive) / 12))));
    if(current_note_freq < minimum_freq) return (musical_note) NOTE_DEF_UNDER;
    
    freq_t maximum_freq = (freq_t) (profile->reference_note.freq / ( pow(2.00, -5.00 - ((double) ( profile->reference_note.position_in_octive) / 12))));
    if(current_note_freq > maximum_freq) return (musical_note) NOTE_DEF_OVER;
    
    if(profile->octive_count < 11){
        while(current_note_freq < profile->octives[0].notes[0].freq){
            musical_note new_reference_note;
            new_reference_note = profile->octives[0].notes[profile->reference_note.position_in_octive];
            new_reference_note.freq = new_reference_note.freq / 2;
        
            profile->octive_count += 1;
            for(uint8_t i = profile->octive_count; i > 0 ; i--){
                profile->octives[i] = profile->octives[i - 1];
            }
            profile->octives[0] = calculate_single_octive(new_reference_note);
        }
        while(current_note_freq > profile->octives[profile->octive_count].notes[12].freq) {
            musical_note new_reference_note;
            new_reference_note = profile->octives[profile->octive_count].notes[profile->reference_note.position_in_octive];
            new_reference_note.freq = new_reference_note.freq * 2;
        
            profile->octive_count += 1;
            profile->octives[profile->octive_count] = calculate_single_octive(new_reference_note);
        }
    } else {
        if(current_note_freq < profile->octives[0].notes[0].freq) return (musical_note) NOTE_DEF_UNDER_OUT_OF_OCTIVES;
        if(current_note_freq > profile->octives[profile->octive_count].notes[12].freq) return (musical_note) NOTE_DEF_OVER_OUT_OF_OCTIVES;
    }

    
    musical_octive current_octive = find_currently_playing_note_octive(current_note_freq, profile);
    
    musical_note current_note;
    uint8_t found_note = 0;
    for(uint8_t i = 1; i < 10; i++){
        if(abs((int8_t) current_note_freq - current_octive.notes[i].freq) > abs((int8_t) current_note_freq - current_octive.notes[i + 1].freq)) continue;
        current_note = current_octive.notes[i];
        found_note = 1;
    }
    if(!found_note) {
        if(abs((int8_t) current_note_freq - current_octive.notes[10].freq) > abs((int8_t) current_note_freq - current_octive.notes[11].freq)){
            current_note = current_octive.notes[10];
            found_note = 1;
        }
    }
    if(!found_note) {
        if(abs((int8_t) current_note_freq - current_octive.notes[11].freq) > abs((int8_t) current_note_freq - current_octive.notes[12].freq)){
            current_note = current_octive.notes[11];
            found_note = 1;
        }
    }
    if(!found_note) current_note = current_octive.notes[12];
    
    return current_note;
}

uint8_t calculate_cents(freq_t freq_one, freq_t freq_two){
    return 1200 * log(((double)freq_one)/((double)freq_two));
}

/* tuning
 * orange -> +-7 cents
 * red -> +-14 cents (mabye 13 or 12 idk) 
 */
uint8_t decide_tuning_level_in_cents(freq_t current_note_freq, freq_t calculated_note_freq){
    uint8_t diff_in_cents = calculate_cents(calculated_note_freq, current_note_freq);
    
    if(diff_in_cents >= 14) return TUNE_DIFF_OVER_2;
    if(diff_in_cents >= 7) return TUNE_DIFF_OVER_1;
    if(diff_in_cents <= -7) return TUNE_DIFF_UNDER_1;
    if(diff_in_cents <= -14) return TUNE_DIFF_UNDER_2;
    
    return TUNE_DIFF_LEVEL;
}

void send_tuning_data(){
    tuning_ready = 1;
}
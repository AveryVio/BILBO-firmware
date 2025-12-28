#include "tuning.h"

#include <bits/alltypes.h>
#include <math.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../libraries/tuning_types.h"

uint8_t calculate_cents(freq_t freq_one, freq_t freq_two){
    return 1200 * log(((double)freq_one)/((double)freq_two));
}

musical_octive calculate_single_octive(musical_note octive_reference_note){
    musical_octive calculated_octive;
    
    for(uint8_t j = 0; j <= 12; j++){
        calculated_octive.notes[j].freq = (octive_reference_note.freq * (freq_t) pow( 2.00 , ((double) j - (double) octive_reference_note.position_in_octive)/12.00 ));
    }
    
    return calculated_octive;
}

// calculate profile from scratch
// new profile should be at least 5 octives (this does 7) including the octive of the ref note
tuning_profile calculate_base_tuning_profile(musical_note reference_note, int8_t reference_note_octive){
    // int8_t profile_octives[3] = {reference_note_octive - 1, reference_note_octive, reference_note_octive + 1};
    tuning_profile profile;
    profile.reference_note = reference_note;
    profile.reference_note_octive = reference_note_octive;
    profile.octive = 7;
    
    for(int8_t i = 0; i <= 7; i++){
        musical_note current_reference_note = reference_note;
        current_reference_note.freq = reference_note.freq / (pow(2.00,(double) (4 - i)));
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

/*
 * find which note is the closest
 *  at least 7 octives should be precalculated
 *  if else for bigger or smaller than currently caltulated octives
 *  binary search according to freq
 */

/*
 * if below range
 *      while(freq < minimum freq){
 *          calculate new octt
 *          put new oct into array
 *      }
 */

musical_note find_currently_playing_note(freq_t currently_playing_note_freq, tuning_profile *profile){
    /*handle freq out of bounds*/
    while(currently_playing_note_freq < profile->octives[0].notes[0].freq){
        musical_note new_reference_note;
        new_reference_note = profile->octives[0].notes[profile->reference_note.position_in_octive];
        
        profile->octive_count += 1;
        for(uint8_t i = profile->octive_count; i > 0 ; i--){
            profile->octives[i] = profile->octives[i - 1];
        }
        profile->octives[0] = calculate_single_octive(new_reference_note);
    }
    while(currently_playing_note_freq > profile->octives[profile->octive_count].notes[12].freq) {
        musical_note new_reference_note;
        new_reference_note = profile->octives[profile->octive_count].notes[profile->reference_note.position_in_octive];
        
        profile->octive_count += 1;
        profile->octives[profile->octive_count] = calculate_single_octive(new_reference_note);
    }
    /*binary search for the note*/
}

uint8_t tuning_init(){
}
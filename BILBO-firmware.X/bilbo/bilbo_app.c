#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "bilbo_config.h"
#include "bilbo_generics.h"

#include "low_level/main_frequency_counting.h"
#include "libraries/tuning_types.h"
#include "low_level/tuning.h"
#include "libraries/frop_types.h"
#include "low_level/bt_communication.h"
#include "low_level/frop.h"
#include "low_level/user_buttons.h"
#include "low_level/user_leds.h"

// tuning variables

freq_t current_freq = 0;
uint8_t current_tuning_level_in_cents = 0;
musical_note calculated_note = NOTE_DEF_NULL;
tuning_profile current_profile = PROFILE_DEF_NULL;

int bilbo_init(){return 0;}

int bilbo_tasks(){
    /* TODO:
     * X handle freq
     *      X a variable holds the current freq, the freq is saved there
     * X handle tuning
     *      X recognise the current note
     *      X compare witth the current note
     *      X decide if the note is tuned or how much it's not tuned
     * handle comm
     *      handle incoming stuff
     *      handle outgoing stuff
     * handle buttons
     * handle leds
     */
    
    current_freq = handle_freq_counter(current_freq);
    
    calculated_note = find_currently_playing_note(current_freq, &current_profile);
    current_tuning_level_in_cents = decide_tuning_level_in_cents(current_freq, calculated_note.freq);
    
    //comm
    
    return 0;
}
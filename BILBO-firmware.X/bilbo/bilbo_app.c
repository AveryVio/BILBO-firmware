#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"

#include "libraries/frop_types.h"
#include "low_level/bt_communication.h"
#include "low_level/frop.h"
#include "low_level/main_frequency_counting.h"
#include "low_level/user_buttons.h"
#include "low_level/user_leds.h"

uint16_t current_freq = 0;

int bilbo_init(){}

int bilbo_tasks(){
    /* TODO:
     * X handle freq
     *      X a variable holds the current freq, the freq is saved there
     * handle tuning
     *      recognise the current note
     *      compare witth the current note
     *      decide if the note is tuned or how much it's not tuned
     * handle comm
     *      handle incoming stuff
     *      handle outgoing stuff
     * handle buttons
     * handle leds
     */
    
    current_freq = handle_freq_counter(current_freq);
}
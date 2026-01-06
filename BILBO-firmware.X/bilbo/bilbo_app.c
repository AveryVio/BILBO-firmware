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
uint8_t tuning_range = TUNE_RANGE_GUITAR;

lengthy_buffer bt_incoming_message[RN4870_READ_WRITE_BUFFER_SIZE];
lengthy_buffer bt_outgoing_message[RN4870_READ_WRITE_BUFFER_SIZE];

uint8_t ok_queued = 0;
uint8_t range_changed = 0;


int bilbo_init(){return 0;} /* init_error_queue() global_error_queue init_message_log() */

int bilbo_tasks(){
    /* TODO:
     * X handle freq
     *      X a variable holds the current freq, the freq is saved there
     * X handle tuning
     *      X recognise the current note
     *      X compare witth the current note
     *      X decide if the note is tuned or how much it's not tuned
     * x handle comm incoming
     * handle parsing
     * handle parsing errors
     * handle comm outgoing
     * handle buttons
     * handle multiplexer
     * handle leds
     */
    
    //freq
    current_freq = handle_freq_counter(current_freq);
    
    //tuning
    calculated_note = find_currently_playing_note(current_freq, &current_profile);
    current_tuning_level_in_cents = decide_tuning_level_in_cents(current_freq, calculated_note.freq);
    
    //comm in
    
    //parsing
    if(bt_incoming_message->buffer[0] == '\0'); //skip the rest of parsing
    
    if(bt_incoming_message->buffer[0] != 'F') {
        throw_error(1);
        /*skip rest of parsing*/
    }
    
    uint8_t incoming_message_type = decide_incoming_message_type(bt_incoming_message->buffer);
    
    switch( incoming_message_type ){
        case FROP_MSG_NULL:
            throw_error(2);
            break;
        case FROP_MSG_D_NULL:
            throw_error(20);
            break;
        case FROP_MSG_R_NULL:
            throw_error(20);
            break;
        case FROP_MSG_D_PROFILE_CHANGE: {
            change_profile frop_organised_message;
            for(uint8_t i = 19; i > 0; i--) frop_organised_message.data[i] = bt_incoming_message->buffer[i - 1];
            
            if(validate_profile_change(&frop_organised_message)) break;
            
            musical_note new_note = { .freq = frop_organised_message.structured.block_data_ref, .position_in_octive = frop_organised_message.structured.block_data_ref_pos};
            
            current_profile = calculate_base_tuning_profile(new_note, frop_organised_message.structured.block_data_ref_oct);
            /*queue ok*/
        }
            break;
        case FROP_MSG_R_OK:
            // handle ok of last message
            break;
        case FROP_MSG_R_SHORT_ERROR:
            // handle error of last message
            break;
    }
    
    bt_incoming_message->buffer[0] = '\0';
    
    /*location to be skipped to*/
    
    //errors
    
    //comm out
    /*
     * X send outgoing ok
     * X send outgoing errors
     * X send change range (when used)
     * periodically send tuning (when possible)
     * ...
     */
    if(ok_queued) send_message( build_ok_response().data, 4 );
    
    for(uint8_t i = 0; i < frop_error_queue.queue_length; i++) send_error(0);
    
    if(range_changed) send_message(build_range_change(tuning_range), 9 );

    /*tuning*/
    
    //buttons
    
    //multiplexer
    
    if(range_changed){
        tuning_range++;
        PORT_REGS->GROUP[0].PORT_OUT = (uint32_t)((tuning_range & 0b10U) >> 1U) << MULTIPLEX_1_PIN;
        PORT_REGS->GROUP[0].PORT_OUT = (uint32_t)(tuning_range & 0b1U) << MULTIPLEX_0_PIN;
    }
    
    //leds
    
    return 0;
}
#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "bilbo_config.h"
#include "bilbo_generics.h"
#include "bilbo_globals.h"

#include "low_level/bt_communication.h"
#include "low_level/frop.h"
#include "libraries/frop_types.h"
#include "low_level/main_frequency_counting.h"
#include "low_level/tuning.h"
#include "libraries/tuning_types.h"
#include "low_level/user_buttons.h"
#include "low_level/user_leds.h"
#include "low_level/batt_adc.h"

// tuning variables

freq_t current_freq = 0;
uint8_t current_tuning_level_in_cents = 0;
musical_note calculated_note = NOTE_DEF_NULL;
uint8_t calculated_note_octive = 0;
tuning_profile current_profile = PROFILE_DEF_NULL;
uint8_t tuning_range = TUNE_RANGE_GUITAR;

lengthy_buffer bt_incoming_message;
lengthy_buffer bt_outgoing_message;

global_error_queue frop_error_queue = {.error_queue = { (global_error_handle) {.code = 0}, (global_error_handle) {.code = 0} }, .queue_length = 2};
global_message_log frop_message_log = {.log = { (global_message_log_entry) {.format = 0}, (global_message_log_entry) {.format = 0} }, .log_length = 2};

uint8_t ok_queued = 0;
uint8_t range_changed = 0;
uint8_t tuning_ready = 0;

uint8_t batt_adc_ready = 0;
uint16_t adc_count = 0;
float input_voltage = 0.0;

int bilbo_init(){
    freq_init();
    
    init_tuning(&tuning_ready);
    
    init_bt_communication(&bt_incoming_message);
    
    frop_error_queue = init_error_queue();
    frop_message_log = init_message_log();
    
    button_init();
    led_init();
    
    batt_adc_init(&batt_adc_ready);
    
    return 0;
} /* init_error_queue() global_error_queue init_message_log() \*tc callbackregister* TC3_TimerStart();*/

int bilbo_tasks(){
    /* TODO:
     * X handle freq
     *      X a variable holds the current freq, the freq is saved there
     * X handle tuning
     *      X recognise the current note
     *      X compare witth the current note
     *      X decide if the note is tuned or how much it's not tuned
     * X handle comm incoming
     * handle parsing
     * x handle parsing errors
     * X handle comm outgoing
     * handle buttons
     * X handle multiplexer
     * handle leds
     */
    
    //freq
    current_freq = handle_freq_counter(current_freq);
    
    //tuning
    calculated_note = find_currently_playing_note(current_freq, &current_profile);
    calculated_note_octive = find_currently_playing_note_octive(current_freq, &current_profile).octive_number;
    current_tuning_level_in_cents = decide_tuning_level_in_cents(current_freq, calculated_note.freq);
    
    //comm in
    
    //parsing
    if(bt_incoming_message.buffer[0] == '\0') goto parsing_skipped;
    
    if(bt_incoming_message.buffer[0] != 'F') {
        throw_error(1);
        goto not_a_frop_message;
    }
    
    uint8_t incoming_message_type = decide_incoming_message_type(bt_incoming_message.buffer);
    
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
            for(uint8_t i = 19; i > 0; i--) frop_organised_message.data[i - 1] = bt_incoming_message.buffer[i]; // the offset is intentional, it's because the first character in the buffer is used to mark that the transfer is avalible
            
            if(validate_profile_change(&frop_organised_message)) break;
            
            musical_note new_note = { .freq = frop_organised_message.structured.block_data_ref, .position_in_octive = frop_organised_message.structured.block_data_ref_pos};
            
            current_profile = calculate_base_tuning_profile(new_note, frop_organised_message.structured.block_data_ref_oct);
            
            ok_queued = 1;
        }
            break;
        case FROP_MSG_R_OK: {
            if(frop_message_log.log_length == 0) {
                throw_error(28);
                break;
            }
            for(uint8_t i  = 0; i < frop_message_log.log_length; i++) frop_message_log.log[i] = frop_message_log.log[i + 1];
            frop_message_log.log_length -= 1;
        }
            break;
        case FROP_MSG_R_SHORT_ERROR: {
            if(frop_error_queue.queue_length == 0) {
                throw_error(29);
                break;
            }
            
            short_error_message frop_organised_message;
            for(uint8_t i = 4; i > 0; i--) frop_organised_message.data[i - 1] = bt_incoming_message.buffer[i]; // the offset is intentional, it's because the first character in the buffer is used to mark that the transfer is avalible
            
            if(frop_organised_message.structured.error_code == 18) break;
            if(frop_organised_message.structured.error_code == 19) break;
            
            for(uint8_t i  = 0; i < frop_error_queue.queue_length; i++) frop_error_queue.error_queue[i] = frop_error_queue.error_queue[i + 1];
            frop_error_queue.queue_length -= 1;
        }
            break;
    }
    
    not_a_frop_message:
    
    bt_incoming_message.buffer[0] = '\0';
    
    parsing_skipped:
    
    //errors
    
    //comm out
    if(ok_queued) {
        send_message( build_ok_response().data, 4 );
        ok_queued = 0;
    }
    
    for(uint8_t i = 0; i < frop_error_queue.queue_length; i++) send_error(0);
    
    if(range_changed) {
        send_message(build_range_change(tuning_range).data, 9 );
        range_changed = 0;
    }

    if(tuning_ready){ // to add timer
        tuning_ready = 0;
        send_message(build_tuning_data(calculated_note_octive, calculated_note.position_in_octive, current_freq, current_tuning_level_in_cents).data, 16);
    }
    
    //buttons
    /*to finish*/
    if(eic_mode_butt_flag){
        eic_mode_butt_flag = 0;
        range_changed = 1;
    }
    
    if(eic_bt_butt_flag){
        eic_bt_butt_flag = 0;
        if(eic_bt_butt_long_press == 0){
            // TODO: handle bt pairing
        } else {
            // TODO: toggle on off bluetooth
        }
    }
    
    //multiplexer
    
    if(range_changed){
        tuning_range++;
        PORT_REGS->GROUP[0].PORT_OUT = (uint32_t)((tuning_range & 0b10U) >> 1U) << MULTIPLEX_1_PIN;
        PORT_REGS->GROUP[0].PORT_OUT = (uint32_t)(tuning_range & 0b1U) << MULTIPLEX_0_PIN;
    }
    
    //leds
    /*to finish*/
    handle_tuning_output_leds(current_tuning_level_in_cents);    
    
    handle_range_leds_out(tuning_range);
    
    batt_adc_handle(&batt_adc_ready, &adc_count, &input_voltage);
    
    return 0;
}
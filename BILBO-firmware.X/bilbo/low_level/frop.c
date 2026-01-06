#include "frop.h"

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../libraries/frop_types.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"

short_error_message build_short_error_message(uint8_t error_code) {
    short_error_message message;
    message.structured.start_of_message = 'F';
    message.structured.frop_message_type = 'R';
    message.structured.frop_message_format = 0x2;
    message.structured.error_code = error_code;
    message.structured.end_of_header = 'D';
    return message;
}

ok_response build_ok_response() {
    ok_response message;
    message.structured.start_of_message = 'F';
    message.structured.frop_message_type = 'R';
    message.structured.frop_message_format = 0x1;
    message.structured.end_of_header = 'D';
    return message;
}

tuning_data build_tuning_data(uint8_t note_octive, uint8_t note_position_in_octive, freq_t frequency, uint8_t tune_offset) {
    
    tuning_data message;
    message.structured.start_of_message = 'F';
    message.structured.frop_message_type = 'D';
    message.structured.frop_message_format = 0x10;
    message.structured.number_of_fields = 5;
    message.structured.end_of_header = 'D';
    message.structured.block_length_setting = 1;
    message.structured.block_data_setting = 'T';
    message.structured.block_length_note_oct = 1;
    message.structured.block_data_note_oct = note_octive;
    message.structured.block_length_note_pos = 1;
    message.structured.block_data_note_pos = note_position_in_octive;
    message.structured.block_length_freq = 2;
    message.structured.block_data_freq = frequency;
    message.structured.block_length_diff = 1;
    message.structured.block_data_diff = tune_offset;
    return message;
}

range_change build_range_change(uint8_t range) {
    range_change message;
    message.structured.start_of_message = 'F';
    message.structured.frop_message_type = 'D';
    message.structured.frop_message_format = 0x10;
    message.structured.number_of_fields = 2;
    message.structured.end_of_header = 'D';
    message.structured.block_length_setting = 1;
    message.structured.block_data_setting = 'R';
    message.structured.block_length_range = 1;
    message.structured.block_data_range = range;
    return message;
}

uint8_t decide_incoming_message_type(uint8_t *message){
    if(message[1] == 'D'){
        if(message[6] == 'P') return FROP_MSG_D_PROFILE_CHANGE;
        else return FROP_MSG_D_NULL;
    } else if(message[1] == 'R'){
        if(message[2] == 0x2) return FROP_MSG_R_SHORT_ERROR;
        else if(message[2] == 0x1) return FROP_MSG_R_OK;
        else return FROP_MSG_R_NULL;
    } else return FROP_MSG_NULL;
}


global_error_queue init_error_queue(){
    global_error_queue q;
    for(uint8_t i = 9; i >=0; i--){
        q.error_queue[i].code = 0;
    }
    q.queue_length = 0;
    return q;
}

global_error_queue frop_error_queue = {.error_queue = { (global_error_handle) {.code = 0}, (global_error_handle) {.code = 0} }, .queue_length = 2};

void throw_error(uint8_t error_code){
    frop_error_queue.error_queue[frop_error_queue.queue_length + 1].code = error_code;
}

global_message_log init_message_log(){
    global_message_log l;
    for(uint8_t i = 9; i >=0; i--){
        l.log[i].format = 0;
    }
    l.log_length = 0;
    return l;
}

global_message_log frop_message_log = {.log = { (global_message_log_entry) {.format = 0}, (global_message_log_entry) {.format = 0} }, .log_length = 2};

uint8_t validate_profile_change(change_profile *new_profile_adept){
    // this is not proper sanitation, but i don't have enough time to care to make an effecient way for it. let's just collectively pretend that bluetooth is perfect and has no issues at all
    
    if(new_profile_adept->structured.number_of_fields != 4) {
        throw_error(3);
        return 1;
    }
    
    if(new_profile_adept->structured.length_of_data != 5) {
        throw_error(4);
        return 1;
    }
    
    if(new_profile_adept->structured.header_checksum != 0xB3) {
        throw_error(5);
        return 1;
    }
    
    if(new_profile_adept->structured.block_length_setting != 1) {
        throw_error(6);
        return 1;
    }
    
    if(new_profile_adept->structured.block_length_ref != 2) {
        throw_error(6);
        return 1;
    }
    
    if(new_profile_adept->structured.block_length_ref_pos != 1) {
        throw_error(6);
        return 1;
    }
    
    if(new_profile_adept->structured.block_length_ref_oct != 1) {
        throw_error(6);
        return 1;
    }
    
    
    if((new_profile_adept->structured.block_data_ref_oct < -1) || (new_profile_adept->structured.block_data_ref_oct > 9)) {
        throw_error(11);    
        return 1;
    }
    
    if((new_profile_adept->structured.block_data_ref_pos < 1) || (new_profile_adept->structured.block_data_ref_pos > 11)) {
        throw_error(11);    
        return 1;
    }
    
    if((new_profile_adept->structured.block_data_ref < TUNER_ABSOLUTE_MINIMUM_FREQ) || (new_profile_adept->structured.block_data_ref > TUNER_ABSOLUTE_MAXIMUM_FREQ)) {
        throw_error(11);
        return 1;
    }
    
    return 0;
}
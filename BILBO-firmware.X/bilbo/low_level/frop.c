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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t last_sent_message = FROP_MSG_NULL;

global_error_queue init_error_queue(){
    global_error_queue q;
    for(uint8_t i = 9; i >=0; i--){
        q.error_queue[i].code = 0;
    }
    q.queue_length = 0;
    return q;
}

global_error_queue frop_error_queue = init_error_queue();

short_error_message throw_error(uint8_t error_code, uint8_t source){
    frop_error_queue.error_queue[frop_error_queue.queue_length + 1].code = error_code;
    
    return build_short_error_message(error_code);
}

void send_error(short_error_message *message, uint8_t queue_index){
    SERCOM0_USART_Write(message, 5);
    
    for(uint8_t i = queue_index; i < frop_error_queue.queue_length; i++){
        frop_error_queue.error_queue[i].code = frop_error_queue.error_queue[i + 1].code;
    }
    // last index doesn't have to be fixes since it won't be read.
}
uint8_t validate_profile_change(change_profile *new_profile_adept){
    // this is not proper sanitation, but i don't have enough time to care to make an effecient way for it. let's just collectively pretend that bluetooth is perfect and has no issues at all
    
    if(new_profile_adept->structured.number_of_fields != 3) {
        throw_error(3);
        return 1;
    }
    
    if(new_profile_adept->structured.length_of_data != 4) {
        throw_error(4);
        return 1;
    }
    
    if(new_profile_adept->structured.header_checksum != 0xB1) {
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
    
    if(new_profile_adept->structured.block_length_ref_oct != 1) {
        throw_error(6);
        return 1;
    }
    
    
    if((new_profile_adept->structured.block_data_ref_oct < -1) || (new_profile_adept->structured.block_data_ref_oct > 9)) {
        throw_error(11);    
        return 1;
    }
    
    if((new_profile_adept->structured.block_data_ref < TUNER_ABSOLUTE_MINIMUM_FREQ) || (new_profile_adept->structured.block_data_ref > TUNER_ABSOLUTE_MAXIMUM_FREQ)) {
        throw_error(11);
        return 1;
    }
    
    return 0;
}

/*to be relocated*/

void parse_frop_message(){
    uint8_t message[128];
    message[0] = 'F';
    
    if(message[0] != 'F') {
        throw_error(1);
        return;
    }
    
    uint8_t incoming_message_type = decide_incoming_message_type(message);
    uint8_t error_level_generated = 0;
    
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
            for(uint8_t i = 16; i >= 0; i--) frop_organised_message.data[i] = message[i];
            
            if(validate_profile_change(&frop_organised_message)){
                /*skip to end*/
            }
            /*correct*/

        }
            break;
        case FROP_MSG_R_OK:
            // handle ok of last message
            break;
        case FROP_MSG_R_SHORT_ERROR:
            // handle error of last message
            break;
    }
}
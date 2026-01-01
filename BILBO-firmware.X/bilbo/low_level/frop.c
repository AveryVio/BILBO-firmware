#include "frop.h"

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../libraries/frop_types.h"

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

void parse_frop_message(){
    uint8_t message[128];
    message[0] = 'F';
    
    if(message[0] != 'F');//bad message
    
    uint8_t incoming_message_type = decide_incoming_message_type(message);
    
    switch( incoming_message_type ){
        case FROP_MSG_NULL:
            // throw error
            // handle error
            break;
        case FROP_MSG_D_NULL:
            // throw error
            // handle error
            break;
        case FROP_MSG_R_NULL:
            // throw error
            // handle error
            break;
        case FROP_MSG_D_PROFILE_CHANGE: {
            change_profile frop_organised_message;
            for(uint8_t i = 16; i >= 0; i--) frop_organised_message.data[i] = message[i];
            
            // this is not proper sanitation, but i don't have enough time to care to make an effecient way for it. let's just collectively pretend that bluetooth is perfect and has no issues at all
            
            if(frop_organised_message.structured.number_of_fields != 3); // throw error
            
            if(frop_organised_message.structured.length_of_data != 4); // throw error
            
            if(frop_organised_message.structured.header_checksum != 0xB1); // throw error

            if(frop_organised_message.structured.block_length_setting != 1); // throw error

            
            if(frop_organised_message.structured.block_length_ref != 2); // throw error
            
            if(frop_organised_message.structured.block_length_ref_oct != 1); // throw error

            //check ref oct validity
            //check ref validity
            //check 
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
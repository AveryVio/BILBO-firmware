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
    message.data = {'F', 'R', 0x2, error_code, 'D'};
    return message;
}

ok_response build_ok_response() {
    ok_response message;
    message.data = {'F', 'R', 0x1, 'D'};
    return message;
}

tuning_data build_tuning_data(uint8_t note_octive, uint8_t note_position_in_octive, uint16_t frequency, uint8_t tune_offset) {
    uint8_t frequency_byte_one = (uint8_t)(frequency >> 8);
    uint8_t frequency_byte_two = (uint8_t)frequency;
    
    tuning_data message;
    message.data = {'F', 'D', 0x10, 5, 'D', 1, "T", 1, note_octive, 1, note_position_in_octive, 2, frequency_byte_one, frequency_byte_two, 1, tune_offset};
    return message;
}

range_change build_range_change(uint8_t range) {
    range_change message;
    message.data = {'F', 'D', 0x10, 2, 'D', 1, "R", 1, range};
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
    
    if(message[0] != 'F');//bad message
    
    switch( decide_incoming_message_type(&message) ){
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
        case FROP_MSG_D_PROFILE_CHANGE:
            change_profile frop_organised_message;
            for(uint8_t i = 16; i >= 0; i--) frop_organised_message[i] = message[i];
            
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
            
            break;
        case FROP_MSG_R_OK:
            // handle ok of last message
            break;
        case FROP_MSG_R_SHORT_ERROR:
            // handle error of last message
            break;
    }
}
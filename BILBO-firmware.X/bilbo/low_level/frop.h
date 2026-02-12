#ifndef BILBO_FROP_H
#define BILBO_FROP_H

#include <bits/alltypes.h>

#include "peripheral/sercom/usart/plib_sercom0_usart.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"
#include "../libraries/frop_types.h"

short_error_message build_short_error_message(uint8_t error_code);

ok_response build_ok_response();

tuning_data build_tuning_data(uint8_t note_octive, uint8_t note_position_in_octive, freq_t frequency, uint8_t tune_offset);

range_change build_range_change(uint8_t range);

uint8_t decide_incoming_message_type(uint8_t *message);


global_error_queue init_error_queue();

void throw_error(uint8_t error_code);


global_message_log init_message_log();

uint8_t validate_profile_change(change_profile *new_profile_adept);

#endif

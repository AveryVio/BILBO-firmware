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
    short_error_message message;
    message.data = {'F', 'R', 0x1, 'D'};
    return message;
}

tuning_data build_tuning_data(uint8_t note[2], uint16_t frequency, uint8_t tune_offset) {
    uint8_t frequency_byte_one = (uint8_t)(frequency >> 8);
    uint8_t frequency_byte_two = (uint8_t)frequency;
    short_error_message message;
    message.data = {'F', 'D', 0x10, 4, 'D', 1, "T", 2, note[0], note[1], 2, frequency_byte_one, frequency_byte_two, 1, tune_offset};
    return message;
}

range_change build_range_change(uint8_t range) {
    short_error_message message;
    message.data = {'F', 'D', 0x10, 2, 'D', 1, "T", 1, range};
    return message;
}


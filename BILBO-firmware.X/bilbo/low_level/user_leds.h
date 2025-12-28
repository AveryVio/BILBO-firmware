#ifndef BILBO_USER_LEDS_H
#define BILBO_USER_LEDS_H

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"

extern uint16_t range_leds_timer_value = 0;
extern uint16_t range_leds_on_status = 0;

void handle_tuning_output_leds(int8_t level);

void handle_range_leds_out(uint8_t range);

uint8_t led_init();

#endif
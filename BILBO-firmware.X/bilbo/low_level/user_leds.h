#ifndef BILBO_USER_LEDS_H
#define BILBO_USER_LEDS_H

#include <bits/alltypes.h>

#include "peripheral/port/plib_port.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"

void handle_tuning_output_leds(int8_t level);

void led_init();

#endif
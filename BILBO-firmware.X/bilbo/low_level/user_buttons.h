#ifndef BILBO_USER_BUTTONS_H
#define BILBO_USER_BUTTONS_H

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"

extern volatile uint8_t eic_bt_butt_flag;
extern volatile uint8_t eic_mode_butt_flag;

extern volatile uint8_t eic_bt_butt_long_press;

void button_init();

#endif
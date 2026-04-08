#include "user_leds.h"

#include <bits/alltypes.h>

#include "peripheral/port/plib_port.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"

void handle_tuning_output_leds(int8_t level){
#ifdef POWER_PRESERVE
#else
    (level > 0)? LED_OR1_Set(): LED_OR1_Clear();
    (level < 0)? LED_OR2_Set(): LED_OR2_Clear();
    (level > 1)? LED_RE1_Set(): LED_RE1_Clear();
    (level < -1)? LED_RE2_Set(): LED_RE2_Clear();
#endif
}

void led_init(){
    LED_GREE_Set();
    
    LED_CHROMATIC_Clear();
}
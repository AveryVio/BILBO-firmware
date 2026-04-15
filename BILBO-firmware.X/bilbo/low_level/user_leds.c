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

/* 
 * mode piezo: 0 - LEDB a LEDH - PA24 a PA23
 * mode microphone: 1 - LEDG a LEDC - PA22 a PA21
 */
void handle_sound_input_leds(sound_input_t soundInput, uint8_t second_int){
#ifdef POWER_PRESERVE
#else
    if(soundInput == piezo) {
        LEDB_Set();
        if(second_int == 0) LEDH_Set();
        LEDG_Clear();
        LEDC_Clear();
    }
    else {
        LEDB_Clear();
        LEDH_Clear();
        LEDG_Set();
        if(second_int == 0) LEDC_Set();
    }
#endif
}

void handle_batt_adc_leds(float batt_voltage){
    if(batt_voltage > BATT_VOLTAGE_KO){
        LED_BATT_OK_Set();
        LED_BATT_KO_Clear();
    } else {
        LED_BATT_OK_Clear();
        LED_BATT_KO_Set();
    }
}

void led_init(sound_input_t defaultSoundInput){
    LED_GREE_Set();
    
    handle_sound_input_leds(defaultSoundInput, 0);
    handle_batt_adc_leds(4);
}
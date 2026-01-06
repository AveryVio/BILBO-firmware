#include "user_leds.h"

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"

uint16_t range_leds_timer_value = 0;
uint16_t range_leds_on_status = 0;

void handle_tuning_output_leds(int8_t level){
#ifdef POWER_PRESERVE
#else
    (level > 0)? LED_OR1_Set(): LED_OR1_Clear();
    (level < 0)? LED_OR2_Set(): LED_OR2_Clear();
    (level > 1)? LED_RE1_Set(): LED_RE1_Clear();
    (level < 1)? LED_RE2_Set(): LED_RE2_Clear();
#endif
}

void handle_range_leds_out(uint8_t range){
#ifdef POWER_PRESERVE
    if(range_leds_on_status == 0){
        uint16_t range_leds_timer_temp_value = TC3_Timer16bitCounterGet();
        if(range_leds_timer_temp_value < (range_leds_timer_value + TC3_DELAY(RANGE_BUTTON_BLINK_DEFAULT_TIMER_OFF))){
            LED_BASS_Clear();
            LED_GUITAR_Clear();
            LED_HIGH_Clear();
        }
        else {
            range_leds_on_status = 1;
            range_leds_timer_value = range_leds_timer_temp_value;
        }
    }
    //TODO: soemething to handle the blinking
    else {
        uint16_t range_leds_timer_temp_value = TC3_Timer16bitCounterGet();
        if(range_leds_timer_temp_value < (range_leds_timer_value + TC3_DELAY(RANGE_BUTTON_BLINK_DEFAULT_TIMER_ON))){
            switch (range){
                case TUNE_RANGE_NULL:
                    // something to handle the beggining
                    break;
                case TUNE_RANGE_BASS:
                    LED_BASS_Set();
                    break;
                case TUNE_RANGE_GUITAR:
                    LED_GUITAR_Set();
                    break;
                case TUNE_RANGE_HIGH:
                    LED_HIGH_Set();
                    break;
                default:
                    LED_BASS_Set();
                    LED_GUITAR_Set();
                    LED_HIGH_Set();
                    break;
            }
        }
        else {
            range_leds_on_status = 0;
            range_leds_timer_value = range_leds_timer_temp_value;
        }
    }
#else
    if(range_leds_on_status == 0){
        LED_BASS_Clear();
        LED_GUITAR_Clear();
        LED_HIGH_Clear();
    }
    else {
        switch (range){
            case TUNE_RANGE_NULL:
                // something to handle the beggining
                break;
            case TUNE_RANGE_BASS:
                LED_BASS_Set();
                break;
            case TUNE_RANGE_GUITAR:
                LED_GUITAR_Set();
                break;
            case TUNE_RANGE_HIGH:
                LED_HIGH_Set();
                break;
            default:
                LED_BASS_Set();
                LED_GUITAR_Set();
                LED_HIGH_Set();
                break;
        }
    }
#endif
}

void led_init(){
    LED_GREE_Set();
}
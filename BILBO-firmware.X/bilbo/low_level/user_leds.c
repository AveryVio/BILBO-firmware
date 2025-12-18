
#include "../bilbo_config.h"

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#define TUNE_RANGE_NULL 0
#define TUNE_RANGE_PLACEHOLDER 1
#define TUNE_RANGE_BASS 2
#define TUNE_RANGE_GUITAR 3
#define TUNE_RANGE_HIGH 4

void handle_tuning_output_leds(uint8_t level){
    
}

void handle_range_leds_out(uint8_t range){
    /*TODO: soemething to handle the blinking*/
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

int led_init(){
}
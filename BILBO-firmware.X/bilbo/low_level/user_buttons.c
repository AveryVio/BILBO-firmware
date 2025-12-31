#include "user_buttons.h"

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"

volatile uint8_t eic_bt_butt_flag = 0;
volatile uint8_t eic_mode_butt_flag = 0;

volatile uint8_t eic_bt_butt_second_interrupt = 0;
volatile uint8_t eic_bt_butt_long_press = 0;

volatile uint16_t tc_bt_butt_timer_value = 0;

void eic_bt_butt_callback(){
    if(eic_bt_butt_second_interrupt == 0){
        eic_bt_butt_second_interrupt = 1;
        tc_bt_butt_timer_value = TC3_Timer16bitCounterGet();
        return;
    }
    eic_bt_butt_second_interrupt = 0;
    eic_bt_butt_flag = 1;
    if(tc_bt_butt_timer_value < (TC3_Timer16bitCounterGet() + TC3_DELAY(BT_BUTTON_LONG_PRESS_DEFAULT_DELAY))){
        eic_bt_butt_long_press = 0;
        return;
    }
    eic_bt_butt_long_press = 1;
    
}
void eic_mode_butt_callback(){
    eic_mode_butt_flag = 1;
}

uint8_t handle_button_input(){
    // handle mode button input
    if(eic_mode_butt_flag){
        eic_mode_butt_flag = 0;
        //switch mode
    }
    
    //switch
    if(eic_bt_butt_flag){
        eic_bt_butt_flag = 0;
        if(eic_bt_butt_long_press == 0){
            // handle bt pairing
        } else {
            // toggle on off bluetooth
        }
    }
}

uint8_t button_init(){
    EIC_CallbackRegister(BT_BUTT_PIN, eic_bt_butt_callback, (uintptr_t) NULL);
    EIC_CallbackRegister(MODE_BUTT_PIN, eic_mode_butt_callback, (uintptr_t) NULL);
    
    //TC3_TimerCallbackRegister(tc_bt_butt_timer_callback, (uintptr_t) NULL);
    TC3_Timer16bitPeriodSet(TC3_DELAY(BT_BUTTON_LONG_PRESS_DEFAULT_DELAY));
    TC3_TimerStart();
}
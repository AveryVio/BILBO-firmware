#include "main_frequency_counting.h"

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc4.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"


volatile uint16_t tc4_comparator_timer_flag = 0;

volatile uint16_t eic_comparator_out_flag = 0;

void eic_comparator_out_callback(){
    eic_comparator_out_flag = 1;
}

void tc4_comparator_timer_callback(TC_TIMER_STATUS status, uintptr_t context){
    tc4_comparator_timer_flag = 1;
}

freq_t handle_freq_counter(freq_t previous_freq){ // to relocate into tc4 callback
    if(tc4_comparator_timer_flag){
        freq_t eic_freq_intermediary = eic_comparator_out_flag * 10;
        return eic_freq_intermediary;
    }
    else {
        return previous_freq;
    }   
}

void freq_init(){
    TC4_TimerInitialize();
    TC4_TimerCallbackRegister(tc4_comparator_timer_callback, (uintptr_t) NULL);
    TC4_TimerStart();
    
    EIC_CallbackRegister(COMPARATOR_OUT_PIN, eic_comparator_out_callback, (uintptr_t) NULL);
}
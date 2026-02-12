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
    freq_t *output_freq_var = (freq_t *) context;
    
    freq_t eic_freq_intermediary = eic_comparator_out_flag * 10;
    
    *output_freq_var = eic_freq_intermediary;
}

void freq_init(freq_t *output_freq_var){
    TC4_TimerInitialize();
    TC4_TimerCallbackRegister(tc4_comparator_timer_callback, (uintptr_t) output_freq_var);
    TC4_TimerStart();
    
    EIC_CallbackRegister(COMPARATOR_OUT_PIN, eic_comparator_out_callback, (uintptr_t) NULL);
}
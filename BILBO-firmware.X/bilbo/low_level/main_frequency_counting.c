#include "main_frequency_counting.h"

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"

volatile uint16_t tcc1_comparator_timer_flag = 0;

volatile uint16_t eic_comparator_out_flag = 0;

void eic_comparator_out_callback(){
    eic_comparator_out_flag = 1;
}

void tcc1_comparator_timer_callback(){
    tcc1_comparator_timer_flag = 1;
}

freq_t handle_freq_counter(freq_t previous_freq){
    if(tcc1_comparator_timer_flag){
        freq_t eic_freq_intermediary = eic_comparator_out_flag * 10;
        return eic_freq_intermediary;
    }
    else {
        return previous_freq;
    }   
}

uint8_t freq_init(){
    EIC_CallbackRegister(COMPARATOR_OUT_PIN, eic_comparator_out_callback, (uintptr_t) NULL);
}
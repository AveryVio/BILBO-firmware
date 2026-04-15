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
    eic_comparator_out_flag += 1;
}

void tc4_comparator_timer_callback(TC_TIMER_STATUS status, uintptr_t context){
    freq_t *output_freq_var = (freq_t *) context;
    
    freq_t eic_freq_intermediary = eic_comparator_out_flag * 10;
    
    int32_t diff1 = (int32_t)eic_freq_intermediary - (int32_t)*output_freq_var;
    int32_t step1 = diff1 >> FREQ_BIT_SHIFT_1;
    if (step1 == 0 && diff1 > 0) step1 = 1;
    else if (step1 == 0 && diff1 < 0) step1 = -1;
    
    int32_t diff2 = ((int32_t)*output_freq_var - (int32_t)*(output_freq_var + 1)) >> FREQ_BIT_SHIFT_2;
    int32_t step2 = diff2 >> FREQ_BIT_SHIFT_2;
    if (step2 == 0 && diff2 > 0) step2 = 1;
    else if (step2 == 0 && diff2 < 0) step2 = -1;
    
    int32_t diff3 = ((int32_t)*(output_freq_var + 1) - (int32_t)*(output_freq_var + 2)) >> FREQ_BIT_SHIFT_3;
    int32_t step3 = diff3 >> FREQ_BIT_SHIFT_3;
    if (step3 == 0 && diff3 > 0) step3 = 1;
    else if (step3 == 0 && diff3 < 0) step3 = -1;

    int32_t calculation_intermediary = (int32_t)*output_freq_var + step1 + step2 + step3;
    
    if (calculation_intermediary < 0) {
        calculation_intermediary = eic_freq_intermediary;
    }
    
    *(output_freq_var + 2) = *(output_freq_var + 1);
    *(output_freq_var + 1) = *output_freq_var;
    *output_freq_var = (freq_t) calculation_intermediary;
    
    eic_comparator_out_flag = 0;
}

sound_input_t switchSoundInput(sound_input_t previousInput){
    if(previousInput == piezo){
        EIC_CallbackRegister(microphone, eic_comparator_out_callback, (uintptr_t) NULL);
        return microphone;
    }
    else {
        EIC_CallbackRegister(piezo, eic_comparator_out_callback, (uintptr_t) NULL);
        return piezo;
    }
}

void freq_init(freq_t (*output_freq_var)[]){
    TC4_TimerInitialize();
    TC4_TimerCallbackRegister(tc4_comparator_timer_callback, (uintptr_t) output_freq_var);
    TC4_TimerStart();
    
    EIC_CallbackRegister(PIEZO_IN_PIN, eic_comparator_out_callback, (uintptr_t) NULL);
}
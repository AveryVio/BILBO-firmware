#include "batt_adc.h"

#include <bits/alltypes.h>

#include "peripheral/port/plib_port.h"
#include "peripheral/adc/plib_adc.h"
#include "peripheral/adc/plib_adc_common.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"

void batt_adc_callback(ADC_STATUS status, uintptr_t context){
    uint8_t *batt_adc_ready = (uint8_t *) context;
    
    (*batt_adc_ready) = 1;
}

void batt_adc_init(uint8_t *batt_adc_ready){
    ADC_CallbackRegister(batt_adc_callback, (uintptr_t) batt_adc_ready);
}

void batt_adc_handle(uint8_t *result_ready, uint16_t *adc_count, float *input_voltage){
    ADC_ConversionStart();

    if(*result_ready == true) {
        *result_ready = false;
            *input_voltage = (float)*adc_count * ADC_VREF / ADC_MAX_COUNT;
        }
}
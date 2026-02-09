#ifndef BILBO_BATT_ADC_H
#define BILBO_BATT_ADC_H

#include <bits/alltypes.h>

#include "peripheral/port/plib_port.h"
#include "peripheral/adc/plib_adc.h"
#include "peripheral/adc/plib_adc_common.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"

void batt_adc_init(uint8_t *batt_adc_ready);

void batt_adc_handle(uint8_t *result_ready, uint16_t *adc_count, float *input_voltage);

#endif
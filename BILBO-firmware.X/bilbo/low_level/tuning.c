#include "tuning.h"

#include <bits/alltypes.h>
#include <math.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"

uint8_t calculate_cents(freq_t freq_one, freq_t freq_two){
    return 1200 * log(((double)freq_one)/((double)freq_two));
}



uint8_t tuning_init(){
}
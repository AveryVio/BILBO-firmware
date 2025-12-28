#ifndef BILBO_FREQ_COUNTING_H
#define BILBO_FREQ_COUNTING_H

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"

uint16_t handle_freq_counter(uint16_t previous_freq);

int freq_init();

#endif
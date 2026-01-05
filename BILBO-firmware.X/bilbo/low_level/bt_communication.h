#ifndef BILBO_BT_COMMUNICATION_H
#define BILBO_BT_COMMUNICATION_H

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"


void init_bt_communication();

#endif
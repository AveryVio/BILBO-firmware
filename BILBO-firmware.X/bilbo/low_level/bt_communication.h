#ifndef BILBO_BT_COMMUNICATION_H
#define BILBO_BT_COMMUNICATION_H

#include <bits/alltypes.h>

#include "peripheral/sercom/usart/plib_sercom0_usart.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"

#include "frop.h"

void send_error(uint8_t queue_index);

void send_message(uint8_t *message_data, uint8_t message_length);

void init_bt_communication();

#endif
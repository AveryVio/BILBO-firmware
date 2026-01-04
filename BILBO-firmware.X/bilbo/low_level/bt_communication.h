#ifndef BILBO_BT_COMMUNICATION_H
#define BILBO_BT_COMMUNICATION_H

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"

extern uint8_t bt_usart_read_buffer[RN4870_READ_WRITE_BUFFER_SIZE];
extern uint8_t bt_usart_read_buffer_length;


void bt_usart_read_handler();

void init_bt_communication();

#endif
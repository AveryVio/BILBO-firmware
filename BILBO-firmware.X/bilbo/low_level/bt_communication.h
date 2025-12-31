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


void bt_usart_read_handler(){
    if(bt_usart_read_done){
        // TODO: parse the bt data
        bt_usart_read_done = 0;
        bt_temp_buffer_rx_index = 0;
    }
}

void init_bt_communication(){
    bt_start_transparent_uart();
    SERCOM0_USART_ReadCallbackRegister(bt_usart_read_callback, (uintptr_t) NULL);
    SERCOM0_USART_ReadThresholdSet(1);
    SERCOM0_USART_ReadNotificationEnable(true, false);
}

#endif
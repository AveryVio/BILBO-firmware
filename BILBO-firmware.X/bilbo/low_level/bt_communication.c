#include "../bilbo_config.h"
#include "../bilbo_generics.h"

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"

uint8_t bt_usart_read_buffer[RN4870_READ_BUFFER_SIZE];

uint8_t bt_temp_buffer[RN4870_READ_BUFFER_SIZE];
uint16_t bt_temp_buffer_rx_index = 0;
uint8_t bt_usart_read_done = 0;

void bt_usart_read_callback(SERCOM_USART_EVENT event, uintptr_t context){
    if (event == SERCOM_USART_EVENT_READ_THRESHOLD_REACHED){
        uint32_t number_of_bytes_available = SERCOM0_USART_ReadCountGet();
        
        if (number_of_bytes_available == 0) return;
        
        if (number_of_bytes_available > RN4870_READ_BUFFER_SIZE) number_of_bytes_available = RN4870_READ_BUFFER_SIZE;
        SERCOM0_USART_Read(bt_temp_buffer, number_of_bytes_available);
        
        if (bt_usart_read_done) return;
        for (uint32_t i = 0; i < number_of_bytes_available; i++){
            uint8_t c = (uint8_t)bt_temp_buffer[i];
            
            if ((c == '\n') || (c == '\r')){
                bt_usart_read_buffer[bt_temp_buffer_rx_index] = '\0';
                bt_usart_read_done = 1;
                bt_temp_buffer_rx_index = 0;
            }
            else{
                if (bt_temp_buffer_rx_index < (RN4870_READ_BUFFER_SIZE - 1)){
                    bt_usart_read_buffer[bt_temp_buffer_rx_index++] = c;
                }
            }
        }
    }
}

void bt_usart_read_handler(){
    if(bt_usart_read_done){
        // TODO: parse the bt data
        bt_usart_read_done = 0;
        bt_temp_buffer_rx_index = 0;
    }
}

void init_bt_communication(){
    SERCOM0_USART_ReadCallbackRegister(bt_usart_read_callback, (uintptr_t) NULL);
    SERCOM0_USART_ReadThresholdSet(1);
    SERCOM0_USART_ReadNotificationEnable(true, false);
}
#include "bt_communication.h"

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"
#include "frop.h"

uint8_t bt_temp_buffer[RN4870_READ_WRITE_BUFFER_SIZE];
uint16_t bt_temp_buffer_rx_index = 0;

void bt_usart_read_callback(SERCOM_USART_EVENT event, uintptr_t context){
    lengthy_buffer *bt_usart_read_buffer = (lengthy_buffer *) context;
    
    if (event == SERCOM_USART_EVENT_READ_THRESHOLD_REACHED){
        uint32_t number_of_bytes_available = SERCOM0_USART_ReadCountGet();
        
        if (number_of_bytes_available == 0) return;
        
        if (number_of_bytes_available > RN4870_READ_WRITE_BUFFER_SIZE) number_of_bytes_available = RN4870_READ_WRITE_BUFFER_SIZE;
        SERCOM0_USART_Read(bt_temp_buffer, number_of_bytes_available);
        
        if (bt_usart_read_buffer->buffer[0] != '\0') return;
        for (uint32_t i = 0; i < number_of_bytes_available; i++){
            uint8_t c = (uint8_t)bt_temp_buffer[i];
            
            if ((c == '\n') || (c == '\r')){
                bt_usart_read_buffer->length = bt_temp_buffer_rx_index - 1;
                bt_usart_read_buffer->buffer[0] = 0x65;
                bt_temp_buffer_rx_index = 1; // the offset is intentional, it's because the first character in the buffer is used to mark that the transfer is avalible
            }
            else{
                if (bt_temp_buffer_rx_index < (RN4870_READ_WRITE_BUFFER_SIZE - 1)) bt_usart_read_buffer->buffer[bt_temp_buffer_rx_index++] = c;
            }
        }
    }
}

uint8_t bt_start_transparent_uart(){
    SERCOM0_USART_Write((uint8_t*)"$$$\n", 4);
    
    uint8_t cmdmode_success_check[5];
    cmdmode_success_check[0] = 1;
    SERCOM0_USART_Read(cmdmode_success_check, 4);
    if(cmdmode_success_check[0] != 'C') return 1;
    if(cmdmode_success_check[1] != 'M') return 1;
    if(cmdmode_success_check[2] != 'D') return 1;
    if(cmdmode_success_check[3] != '>') return 1;
    if(cmdmode_success_check[4] != '\n') return 1;
    
    SERCOM0_USART_Write((uint8_t*) "&R\n", 3);
    
    uint8_t rndmac_success_check[4];
    rndmac_success_check[0] = 1;
    SERCOM0_USART_Read(rndmac_success_check, 4);
    if(rndmac_success_check[0] != 'A') return 1;
    if(rndmac_success_check[1] != 'O') return 1;
    if(rndmac_success_check[2] != 'K') return 1;
    if(rndmac_success_check[3] != '\n') return 1;
    
    SERCOM0_USART_Write((uint8_t*) "S-,BILBO\n", 9);
    
    uint8_t devname_success_check[4];
    devname_success_check[0] = 1;
    SERCOM0_USART_Read(devname_success_check, 4);
    if(devname_success_check[0] != 'A') return 1;
    if(devname_success_check[1] != 'O') return 1;
    if(devname_success_check[2] != 'K') return 1;
    if(devname_success_check[3] != '\n') return 1;
    
    SERCOM0_USART_Write((uint8_t*) "---\n", 4);
    
    return 0;
}

void send_error(uint8_t queue_index){
    SERCOM0_USART_Write(build_short_error_message(frop_error_queue.error_queue[queue_index].code).data, 5);
    
    for(uint8_t i = queue_index; i < frop_error_queue.queue_length; i++){
        frop_error_queue.error_queue[i].code = frop_error_queue.error_queue[i + 1].code;
    }
    // last index doesn't have to be fixed since it won't be read.
    
    frop_error_queue.queue_length -= 1;
}

void send_message(uint8_t *message_data, uint8_t message_length){
    SERCOM0_USART_Write(message_data, message_length);
    
    frop_message_log.log[frop_message_log.log_length + 1].format = message_data[2];    
    frop_message_log.log_length += 1;
}

void init_bt_communication(lengthy_buffer *buffer){
    buffer->buffer[0] = '\0';
    bt_start_transparent_uart();
    SERCOM0_USART_ReadCallbackRegister(bt_usart_read_callback, (uintptr_t) buffer /*put in reference to struct of buffer*/);
    SERCOM0_USART_ReadThresholdSet(1);
    SERCOM0_USART_ReadNotificationEnable(true, false);
}
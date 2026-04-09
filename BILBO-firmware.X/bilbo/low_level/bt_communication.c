#include "bt_communication.h"

#include <bits/alltypes.h>
#include <string.h>

#include "peripheral/sercom/usart/plib_sercom0_usart.h"
#include "peripheral/port/plib_port.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"
#include "../bilbo_globals.h"

#include "frop.h"

extern global_error_queue frop_error_queue;
extern global_message_log frop_message_log;

uint8_t bt_temp_buffer[RN4870_READ_WRITE_BUFFER_SIZE];
uint16_t bt_temp_buffer_rx_index = 0;

uint8_t transparent_mode = 0;

const char **current_cmd_sequence = NULL;
uint8_t current_cmd_length = 0;
uint8_t current_cmd_index = 0;
uint32_t cmd_delay_counter = 0;

static const char *seq_init[]  = {"$$$", "&R\r", "S-,BILBO\r", "---\r"};
static const char *seq_pair[]  = {"$$$", "A,0020,001E\r", "---\r"};
static const char *seq_sleep[] = {"$$$", "K,1\r", "O,0\r"};

void bt_usart_read_callback(SERCOM_USART_EVENT event, uintptr_t context){
    lengthy_buffer *bt_usart_read_buffer = (lengthy_buffer *) context;
    
    if (bt_usart_read_buffer->buffer[0] != '\0') return;
    
    if (event == SERCOM_USART_EVENT_READ_THRESHOLD_REACHED){
        uint32_t number_of_bytes_available = SERCOM0_USART_ReadCountGet();
        
        if (number_of_bytes_available == 0) return;
        
        if (number_of_bytes_available > RN4870_READ_WRITE_BUFFER_SIZE) number_of_bytes_available = RN4870_READ_WRITE_BUFFER_SIZE;
        SERCOM0_USART_Read(bt_temp_buffer, number_of_bytes_available);
        
        for (uint32_t i = 0; i < number_of_bytes_available; i++){
            uint8_t c = (uint8_t)bt_temp_buffer[i];
            
            if (c == '\n'){
                bt_usart_read_buffer->length = bt_temp_buffer_rx_index - 1; // intentionally does not add in the \n and does not include the terminator in the length (it's not needed for normal operations)
                bt_usart_read_buffer->buffer[bt_temp_buffer_rx_index] = '\0';
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

void init_change_bt_states(const char **sequence, uint8_t length) {
    current_cmd_sequence = sequence;
    current_cmd_length = length;
    current_cmd_index = 0;
    cmd_delay_counter = 0;
    transparent_mode = 0;
}

void bt_communication_tasks() {
    if (transparent_mode) return;
    
    if (cmd_delay_counter > 0) {
        cmd_delay_counter--;
        return;
    }
    

    if (current_cmd_sequence != NULL) {
        if (current_cmd_index < current_cmd_length) {
            const char *cmd = current_cmd_sequence[current_cmd_index];
            SERCOM0_USART_Write((uint8_t*)cmd, strlen(cmd));
            current_cmd_index++;
            
            // TODO: Make it use tc3 instead
            cmd_delay_counter = 50000; 
        } else {
            current_cmd_sequence = NULL;
            transparent_mode = true;
        }
    }
}

void bt_trigger_pairing() {
    if (transparent_mode) {
        init_change_bt_states(seq_pair, 3);

        cmd_delay_counter = 50000; 
    }
}

void bt_trigger_sleep() {
    if (transparent_mode) {
        init_change_bt_states(seq_sleep, 3);

        cmd_delay_counter = 50000; 
    }
}

void bt_trigger_wakeup() {
    if (!transparent_mode) return;

    //RN_UART_RX_IND_Clear(); // cannot wake from sleep
    
    // TODO: Make it use tc3 instead maybe
    for(volatile uint32_t i = 0; i < 10000000; i++);
    
    //RN_UART_RX_IND_Set(); // cannot wake from sleep
    
    for(volatile uint32_t i = 0; i < 10000000; i++);
    
    // this is because the sleep mode clears the config memory. If we want to resume transparent bt, we need to init again
    init_change_bt_states(seq_init, 4);
    
    // TODO: Make it use tc3 instead
    cmd_delay_counter = 500000; 
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
    bt_temp_buffer_rx_index = 1;
    
    SERCOM0_USART_ReadCallbackRegister(bt_usart_read_callback, (uintptr_t) buffer);
    SERCOM0_USART_ReadThresholdSet(1);
    SERCOM0_USART_ReadNotificationEnable(true, false);
    
    init_change_bt_states(seq_init, 4);
}
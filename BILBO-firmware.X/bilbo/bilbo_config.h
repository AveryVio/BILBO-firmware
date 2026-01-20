#ifndef BILBO_CONFIG_H
#define BILBO_CONFIG_H

// enable power preserving features
//#define POWER_PRESERVE

// (input time in ms) * (clock freq) / (prescaler * 1000)
//
// to use this, calculate:
// ---(clock freq) / (prescaler * 1000) should be precalculated for less compute usage  
#define TC3_DELAY(TIME_MS) (TIME_MS*187)

#define BT_BUTTON_LONG_PRESS_DEFAULT_DELAY 300
#define RANGE_BUTTON_BLINK_DEFAULT_TIMER_ON 100
#define RANGE_BUTTON_BLINK_DEFAULT_TIMER_OFF 500

#define RN4870_READ_WRITE_BUFFER_SIZE 64U

#define SERCOM0_USART_READ_BUFFER_SIZE      RN4870_READ_WRITE_BUFFER_SIZE
#define SERCOM0_USART_READ_BUFFER_9BIT_SIZE     (RN4870_READ_WRITE_BUFFER_SIZE >> 1U)

#define SERCOM0_USART_WRITE_BUFFER_SIZE     RN4870_READ_WRITE_BUFFER_SIZE
#define SERCOM0_USART_WRITE_BUFFER_9BIT_SIZE  (RN4870_READ_WRITE_BUFFER_SIZE >> 1U)

#define TUNER_ABSOLUTE_MINIMUM_FREQ 7
#define TUNER_ABSOLUTE_MAXIMUM_FREQ 65534

#endif
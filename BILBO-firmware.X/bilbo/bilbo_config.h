// enable power preserving features
//#define POWER_PRESERVE

// -(input time in ms * (clock freq / prescaler))  
//
//--disguised: (input time in ms)/(period of(clock freq / 1000 / prescaler))  
//
// ---(clock freq / 1000 / prescaler) should be precalculated for less compute usage  
#define TC3_DELAY(TIME_MS) (TIME_MS*187)

#define BT_BUTTON_LONG_PRESS_DEFAULT_DELAY 300
#define RANGE_BUTTON_BLINK_DEFAULT_TIMER_ON 100
#define RANGE_BUTTON_BLINK_DEFAULT_TIMER_OFF 500


// blutooth ring buffer

// mic sample delta times ring buffer
#define MIC_RING_BUFFER_SIZE 100
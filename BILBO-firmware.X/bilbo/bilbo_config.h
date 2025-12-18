
// user buttons

// -(input time in ms * (clock freq / prescaler))  
//
//--disguised: (input time in ms)/(period of(clock freq / 1000 / prescaler))  
//
// ---clock freq / 1000 / prescaler) should be precalculated for less compute usage  
#define BT_BUTTON_LONG_PRESS_DELAY(TIME_MS) (TIME_MS*187)
#define BT_BUTTON_LONG_PRESS_DEFAULT_DELAY BT_BUTTON_LONG_PRESS_DELAY(300)


// blutooth ring buffer

// mic sample delta times ring buffer
#define MIC_RING_BUFFER_SIZE 100
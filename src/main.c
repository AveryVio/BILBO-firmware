#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#include "../BILBO-firmware.X/bilbo/bilbo_app.h"

int main ( void ){
    SYS_Initialize ( NULL );
    
    bilbo_init();

    while ( true ){
        SYS_Tasks ( );
        bilbo_tasks();
    }

    return ( EXIT_FAILURE );
}


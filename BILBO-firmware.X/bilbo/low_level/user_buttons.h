#ifndef BILBO_USER_BUTTONS_H
#define BILBO_USER_BUTTONS_H

#include <bits/alltypes.h>

#include "peripheral/eic/plib_eic.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#include "../bilbo_config.h"
#include "../bilbo_generics.h"

int handle_button_input();

int button_init();

#endif
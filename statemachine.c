//Author: Lisa, Date: 18.09.2026

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware_defines.h"
#include "statemachine.h"


SYSTEM_STATE_t system_state = SYSTEM_IDLE_S;
//commands:


// START_DISCHARGE_PULSE 
// START_CHARGE_PULSE
// START_DISCHARGE_CONT
// START_CHARGE_CONT



static void process_command(const char *command){

        if (strcmp(command,"START_DISCHARGE_PULSE")==0) {
            system_state = DISCHARGE_PULSE_S;
        }

        if (strcmp(command,"START_CHARGE_PULSE")==0) {
            system_state = CHARGE_PULSE_S;
        }

        if (strcmp(command, "START_DISCHARGE_CONT")==0) {
            system_state = DISCHARGE_CONT_S;
        }

        if (strcmp(command, "START_CHARGE_CONT")==0) {
            system_state = CHARGE_CONT_S;
        }

        if (strcmp(command, "STOP")==0) {
            system_state = SYSTEM_IDLE_S;
        }
}

void update_state(void){
    static char command[32];
    static size_t length = 0;
    static bool overflow = false;

    // Bound work per call, even if input arrives continuously.
    for (unsigned i = 0; i < 64; ++i) {
        int ch = getchar_timeout_us(0);
        if (ch == PICO_ERROR_TIMEOUT) {
            return;
        }

        if (isspace((unsigned char)ch)) {
            if (length > 0 && !overflow) {
                command[length] = '\0';
                process_command(command);
                length = 0;
                return;
            }
            length = 0;
            overflow = false;
        } else if (length < sizeof(command) - 1) {
            command[length++] = (char)ch;
        } else {
            overflow = true;
        }
    }
}


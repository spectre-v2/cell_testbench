//Author: Lisa, Date: 18.09.2026

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/error.h"
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

        else if (strcmp(command,"START_CHARGE_PULSE")==0) {
            system_state = CHARGE_PULSE_S;
        }

        else if (strcmp(command, "START_DISCHARGE_CONT")==0) {
            system_state = DISCHARGE_CONT_S;
        }

        else if (strcmp(command, "START_CHARGE_CONT")==0) {
            system_state = CHARGE_CONT_S;
        }

        else if (strcmp(command, "STOP")==0) {
            system_state = SYSTEM_IDLE_S;
        }

        else{
            system_state = SYSTEM_IDLE_S;
            printf("invalid command: %s \n");
            return;
        }

   printf("recieved %s command.", command);
        
}

void update_state(void){

    static char command[32];
    static uint8_t character_count = 0;

    int32_t recieved_char = getchar_timeout_us(0);

    if(recieved_char ==  PICO_ERROR_TIMEOUT){
        return; //nothing in standard input buffer
    }

    bool recieved_enter = recieved_char == '\n' || recieved_char == '\r';

    if(recieved_enter) {
        command[character_count] = '\0';
        process_command(command);
        character_count = 0;
        return;
    }

    if(character_count < sizeof(command)-1){
        command[character_count] = (char)recieved_char;
        character_count++;
    }
}


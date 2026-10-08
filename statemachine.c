//Author: Lisa, Date: 18.09.2026

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/error.h"
#include "hardware_defines.h"
#include "statemachine.h"
#include "tasks.h"


SYSTEM_STATE_t system_state = SYSTEM_IDLE_S;
static void get_terminal_command();
static void parse_terminal_command(char* command);

//commands:
// START_DISCHARGE_PULSE 
// START_CHARGE_PULSE
// START_DISCHARGE_CONT
// START_CHARGE_CONT

void update_statemachine(){

    get_terminal_command();

    switch(system_state){
        case DISCHARGE_PULSE_S:
            gpio_put(PIN_CHARGE_RELAY,0);
            tasks[task_led_toggle].task_enabled = true;
            tasks[task_led_toggle].interval_ms = 1;
            tasks[task_update_charge_relay].task_enabled = false;
            tasks[task_update_discharge_relay].task_enabled = true;
            tasks[task_print_data].task_enabled = true;
            tasks[task_read_voltages].task_enabled = true;
            tasks[task_calculate_temperatures].task_enabled = true;

        break;

        case CHARGE_PULSE_S:
            gpio_put(PIN_DISCHARGE_RELAY,0);
            tasks[task_led_toggle].task_enabled = true;
            tasks[task_led_toggle].interval_ms = 10;
            tasks[task_update_discharge_relay].task_enabled = true;
            tasks[task_update_charge_relay].task_enabled = true;
            tasks[task_print_data].task_enabled = true;
            tasks[task_read_voltages].task_enabled = true;
            tasks[task_calculate_temperatures].task_enabled = true;
        break;

        case DISCHARGE_CONT_S:
        //todo
            gpio_put(PIN_CHARGE_RELAY,1);
            gpio_put(PIN_DISCHARGE_RELAY,0);
            tasks[task_read_voltages].task_enabled = true;
            tasks[task_led_toggle].task_enabled = true;
            tasks[task_led_toggle].interval_ms = 100;
            tasks[task_calculate_temperatures].task_enabled = true;
            tasks[task_update_charge_relay].task_enabled = false;
            tasks[task_update_discharge_relay].task_enabled = false;
        break;

        case CHARGE_CONT_S:
        //todo
            gpio_put(PIN_DISCHARGE_RELAY,0);
            gpio_put(PIN_CHARGE_RELAY,1);
            tasks[task_read_voltages].task_enabled = true;
            tasks[task_led_toggle].task_enabled = true;
            tasks[task_led_toggle].interval_ms = 100;
            tasks[task_calculate_temperatures].task_enabled = true;
            tasks[task_update_charge_relay].task_enabled = false;
            tasks[task_update_discharge_relay].task_enabled = false;
        break;

        case SYSTEM_IDLE_S:
            gpio_put(PIN_CHARGE_RELAY,0);
            gpio_put(PIN_DISCHARGE_RELAY,0);
            tasks[task_led_toggle].task_enabled = true;
            tasks[task_led_toggle].interval_ms = 2000;
            tasks[task_print_data].task_enabled = false;
            tasks[task_update_charge_relay].task_enabled = false;
            tasks[task_update_discharge_relay].task_enabled = false;
        break;

        
    }

}

static void parse_terminal_command( char *command){

        if (strcmp(command,"start_discharge_pulse")==0) {
            system_state = DISCHARGE_PULSE_S;
        }

        else if (strcmp(command,"start_charge_pulse")==0) {
            system_state = CHARGE_PULSE_S;
        }

        else if (strcmp(command, "start_discharge_cont")==0) {
            system_state = DISCHARGE_CONT_S;
        }

        else if (strcmp(command, "start_charge_cont")==0) {
            system_state = CHARGE_CONT_S;
        }

        else if (strcmp(command, "stop")==0) {
            system_state = SYSTEM_IDLE_S;
        }

        else{
            system_state = SYSTEM_IDLE_S;
            printf("\n invalid command: %s \n", command);
            return;
        }

   printf("\n recieved %s command.\n", command);
        
}

static void get_terminal_command(void){

    static char command[32];
    static uint8_t character_count = 0;

    int32_t recieved_char = getchar_timeout_us(0);

    if(recieved_char ==  PICO_ERROR_TIMEOUT){
        return; //nothing in standard input buffer
    }

    if(recieved_char == '\b' || recieved_char == 127){
        if(character_count>0){
            character_count --;
            printf("\b \b");
        }
        return;
    }
    putchar(recieved_char);

    bool recieved_enter = recieved_char == '\n' || recieved_char == '\r';

    if(recieved_enter) {
        command[character_count] = '\0';
        parse_terminal_command(command);
        character_count = 0;
        return;
    }

    if(character_count < sizeof(command)-1){
        command[character_count] = (char)recieved_char;
        character_count++;
    }

}

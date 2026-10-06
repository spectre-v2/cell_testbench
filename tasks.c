#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include "hardware_defines.h"
#include "tasks.h"
#include "statemachine.h"
#include "ads_driver.h"
#include "volt_temp_calc.h"


// Relay phase durations in milliseconds.
#define RELAY_CHARGE_ON_TIME 3000
#define RELAY_CHARGE_OFF_TIME 3000

#define RELAY_DISCHARGE_ON_TIME 3000
#define RELAY_DISCHARGE_OFF_TIME 3000

#define TASK_COUNT (sizeof(tasks) / sizeof(tasks[0]))

static uint64_t system_start_time_us;
static ADS_FULL_SAMPLE_VOLTAGES_t voltages;
static full_sample_temperatures_t temperatures;

static void read_voltages(void);
static void calculate_temperatures(void);


//main struct containing all tasks
task_t tasks[] = {
    {.service_routine = read_voltages, .interval_us = 10000},
    {.service_routine = calculate_temperatures, .interval_us = 10000},
    {.service_routine = led_toggle, .interval_us = 100000},
    {.service_routine = print_data, .interval_us = 10000},
    {.service_routine = update_state, .interval_us = 10000},
    {.service_routine = update_relays, .interval_us = 10000}

};

void task_scheduler_init(void){

    system_start_time_us = time_us_64();

    for (uint8_t task = 0; task<TASK_COUNT; task++){
        tasks[task].next_due_us = system_start_time_us + tasks[task].interval_us;
    }
}

void task_scheduler_tick(){

    uint64_t system_time_now = time_us_64();

    for(uint8_t task = 0; task < TASK_COUNT; task++){

            if(tasks[task].next_due_us <= system_time_now){
                tasks[task].service_routine();
                tasks[task].next_due_us = system_time_now+ tasks[task].interval_us;
            }
    }
}

static void read_voltages(void){
    ADS_GET_VOLTAGES(&voltages);
}

static void calculate_temperatures(void){
    convert_voltages_to_temperatures(&temperatures, &voltages);
}

void print_data(){
    printf("t_ms: %" PRIu64, timestamp_ms());
    printf("V_Cell: %f", voltages.data_fields.ch_1_voltage);
    printf("Ch1: %f" , temperatures.data_fields.ch_1_temp);
    printf("Ch2: %f" , temperatures.data_fields.ch_2_temp);
    printf("Ch3: %f" , temperatures.data_fields.ch_3_temp);
    printf("Ch4: %f" , temperatures.data_fields.ch_4_temp);
    printf(" \n");
}

uint64_t timestamp_ms(){
    return (time_us_64()- system_start_time_us)/1000;
}

void led_toggle(){
    gpio_xor_mask(1<<LED_PIN);
}

void update_relays(void){
    static SYSTEM_STATE_t previous_state = SYSTEM_IDLE_S;
    static bool relay_on = false;
    static uint64_t next_switch_us = 0;

    const uint64_t now = time_us_64();
    const bool state_changed = system_state != previous_state;

    if (state_changed) {
        gpio_put(PIN_CHARGE_RELAY, 0);
        gpio_put(PIN_DISCHARGE_RELAY, 0);
        relay_on = false;
        previous_state = system_state;
    }

    uint relay_pin;
    uint32_t on_time_ms;
    uint32_t off_time_ms;

    switch (system_state) {
    case CHARGE_PULSE_S:
        relay_pin = PIN_CHARGE_RELAY;
        on_time_ms = RELAY_CHARGE_ON_TIME;
        off_time_ms = RELAY_CHARGE_OFF_TIME;
        break;
    case DISCHARGE_PULSE_S:
        relay_pin = PIN_DISCHARGE_RELAY;
        on_time_ms = RELAY_DISCHARGE_ON_TIME;
        off_time_ms = RELAY_DISCHARGE_OFF_TIME;
        break;
    case CHARGE_CONT_S:
        gpio_put(PIN_DISCHARGE_RELAY, 0);
        gpio_put(PIN_CHARGE_RELAY, 1);
        return;
    case DISCHARGE_CONT_S:
        gpio_put(PIN_CHARGE_RELAY, 0);
        gpio_put(PIN_DISCHARGE_RELAY, 1);
        return;
    case SYSTEM_IDLE_S:
    default:
        gpio_put(PIN_CHARGE_RELAY, 0);
        gpio_put(PIN_DISCHARGE_RELAY, 0);
        return;
    }

    if (state_changed || now >= next_switch_us) {
        relay_on = !relay_on;
        gpio_put(relay_pin, relay_on);
        // Time each phase from the actual switch; do not catch up missed edges.
        next_switch_us = now + (uint64_t)(relay_on ? on_time_ms : off_time_ms) * 1000;
    }
}

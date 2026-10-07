#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include "hardware_defines.h"
#include "tasks.h"
#include "statemachine.h"
#include "ads_driver.h"
#include "volt_temp_calc.h"




#define TASK_COUNT (sizeof(tasks) / sizeof(tasks[0]))

static ADS_FULL_SAMPLE_VOLTAGES_t voltages;
static full_sample_temperatures_t temperatures;


//relay properties for pulse generation
static bool relay_charge_on = false;
static bool relay_discharge_on = false;

static uint64_t relay_charge_last_switch_ms = 0;
static uint64_t relay_discharge_last_switch_ms = 0;

// Relay phase durations in milliseconds.
#define RELAY_CHARGE_ON_TIME 3000
#define RELAY_CHARGE_OFF_TIME 3000

#define RELAY_DISCHARGE_ON_TIME 3000
#define RELAY_DISCHARGE_OFF_TIME 3000

//main struct containing all tasks
task_t tasks[] = {
    {.service_routine = read_voltages, .interval_ms = 10},
    {.service_routine = calculate_temperatures, .interval_ms = 10},
    {.service_routine = led_toggle, .interval_ms = 100},
    {.service_routine = print_data, .interval_ms = 10},
    {.service_routine = update_state, .interval_ms = 10},
    {.service_routine = update_charge_relay, .interval_ms = 100},
    {.service_routine = update_discharge_relay, .interval_ms = 100},
};

void task_scheduler_init(void){

    uint64_t time_now = time_ms_64();

    for (uint8_t task = 0; task<TASK_COUNT; task++){
        tasks[task].next_due_ms = time_now + tasks[task].interval_ms;
    }
}

void task_scheduler_tick(){

    uint64_t time_now = time_us_64();

    for(uint8_t task = 0; task < TASK_COUNT; task++){

            if(tasks[task].next_due_ms <= time_now){
                tasks[task].service_routine();
                tasks[task].next_due_ms = time_now + tasks[task].interval_ms;
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


uint64_t time_ms_64(){
    return time_us_64()/1000;
}

void led_toggle(){
    gpio_xor_mask(1<<LED_PIN);
}

void update_charge_relay(void){
    uint64_t time_now_ms = time_ms_64();

    uint32_t relay_phase_ms;

    if(relay_charge_on) relay_phase_ms = RELAY_CHARGE_ON_TIME;
    if(relay_charge_off) relay_phase_ms = RELAY_CHARGE_OFF_TIME;

    uint64_t elapsed_time_ms = time_now_ms - relay_charge_last_switch_ms;

    if (elapsed_time_ms >= relay_phase_ms){
        relay_charge_on = !relay_charge_on;
        gpio_put(PIN_CHARGE_RELAY, relay_charge_on);
        relay_charge_last_switch_ms = time_now_ms;
    }

}

void update_discharge_relay(void){

    uint64_t time_now = time_ms_64();
    
    uint32_t elapsed_time_ms = time_now - relay_discharge_last_switch_ms;

    uint32_t relay_phase_ms;

    if(relay_discharge_on) relay_phase_ms = RELAY_DISCHARGE_ON_TIME;
    if(relay_discharge_off) relay_phase_ms = RELAY_DISCHARGE_OFF_TIME;
    
    if(elapsed_time_ms >= relay_phase_ms){
        relay_discharge_on = !relay_discharge_on;
        gpio_put(PIN_DISCARGE_RELAY, relay_discharge_on );
        relay_discharge_last_switch_ms = time_now_ms;
    }
}



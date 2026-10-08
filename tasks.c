#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include "hardware_defines.h"
#include "tasks.h"
#include "statemachine.h"
#include "ads_driver.h"
#include "volt_temp_calc.h"

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

static void read_voltages(void);
static void calculate_temperatures(void);
static void led_toggle(void);
static void print_data(void);
static void update_charge_relay(void);
static void update_discharge_relay(void);

//main task array
task_t tasks[]= {
      [task_update_statemachine] = {
        .service_routine = update_statemachine,
        .interval_ms = 10,
        .task_enabled = true
    },
    [task_read_voltages] = {
        .service_routine = read_voltages,
        .interval_ms = 10,
        .task_enabled = false
    },
    [task_calculate_temperatures] = {
        .service_routine = calculate_temperatures,
        .interval_ms = 10,
        .task_enabled = false
    },
  
    [task_led_toggle] = {
        .service_routine = led_toggle,
        .interval_ms = 1000,
        .task_enabled = true
    },
    [task_print_data] = {
        .service_routine = print_data,
        .interval_ms = 10,
        .task_enabled = false
    },
    [task_update_charge_relay] = {
        .service_routine = update_charge_relay,
        .interval_ms = 10,
        .task_enabled = false
    },
    [task_update_discharge_relay] = {
        .service_routine = update_discharge_relay,
        .interval_ms = 10,
        .task_enabled = false
    }
        
};


void task_scheduler_init(void){

    uint64_t time_now = timestamp_ms();

    for (uint8_t task = 0; task<TASK_COUNT; task++){
        tasks[task].next_due_ms = time_now + tasks[task].interval_ms;
    }
}

void task_scheduler_tick(){
    
    uint64_t time_now = timestamp_ms();

    for(uint8_t task = 0; task < TASK_COUNT; task++){

            if(tasks[task].next_due_ms <= time_now && tasks[task].task_enabled){
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

static void print_data(){
    printf(" t_ms: % " PRIu64, timestamp_ms());
    printf(" V_Cell: %f ", voltages.data_fields.ch_1_voltage);

    printf(" Ch1: %f " , temperatures.data_fields.ch_2_temp);
    printf(" Ch2: %f " , temperatures.data_fields.ch_3_temp);
    printf(" Ch3: %f " , temperatures.data_fields.ch_4_temp);
    printf(" Ch4: %f " , temperatures.data_fields.ch_5_temp);
    printf(" \n");
}

uint64_t timestamp_ms(void){
    return time_us_64()/1000;
}

static void led_toggle(){
    gpio_xor_mask(1<<LED_PIN);
}

static void update_charge_relay(void){
    uint64_t time_now_ms = timestamp_ms();

    uint32_t relay_phase_ms = relay_charge_on
        ? RELAY_CHARGE_ON_TIME : RELAY_CHARGE_OFF_TIME;

    uint64_t elapsed_time_ms = time_now_ms - relay_charge_last_switch_ms;

    if (elapsed_time_ms >= relay_phase_ms){
        relay_charge_on = !relay_charge_on;
        gpio_put(PIN_CHARGE_RELAY, relay_charge_on);
        relay_charge_last_switch_ms = time_now_ms;
    }

}

static void update_discharge_relay(void){

    uint64_t time_now_ms = timestamp_ms();
    
    uint64_t elapsed_time_ms = time_now_ms - relay_discharge_last_switch_ms;

    uint32_t relay_phase_ms = relay_discharge_on
        ? RELAY_DISCHARGE_ON_TIME : RELAY_DISCHARGE_OFF_TIME;
    
    if(elapsed_time_ms >= relay_phase_ms){
        relay_discharge_on = !relay_discharge_on;
        gpio_put(PIN_DISCHARGE_RELAY, relay_discharge_on );
        relay_discharge_last_switch_ms = time_now_ms;
    }
}


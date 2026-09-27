// Author: Leif Stechmann
// Date: 19.9.2026

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware_defines.h"
#include "ads_driver.h"
#include "statemachine.h"
#include "ssd1309.h"
#include "volt_temp_calc.h"

#define TICK_TIMER_INTERVAL_MS 1


volatile uint32_t system_ticks = 0;

//structs
ADS_FULL_SAMPLE_VOLTAGES_t voltages;
full_sample_temperatures_t temperatures;

//tasks
void print_data(void);
void led_toggle(void);
void read_voltages(void);
void calculate_temperatures(void);


typedef struct {

    void (*task_service_routine)(void);
    uint32_t task_interval_ticks;
    uint32_t task_last_execution_ticks;

}task_t;



task_t tasks[] = {
    {.task_service_routine = read_voltages, .task_interval_ticks = 10},
    {.task_service_routine = calculate_temperatures, .task_interval_ticks = 10},
    {.task_service_routine = led_toggle, .task_interval_ticks = 100},
    {.task_service_routine = print_data, .task_interval_ticks = 10}
};

#define TASK_COUNT (sizeof(tasks) / sizeof(tasks[0]))


void task_scheduler_tick(){

    
    for(size_t task = 0; task < TASK_COUNT; task++){


        tasks[task].task_last_execution_ticks++;
            if(tasks[task].task_interval_ticks == tasks[task].task_last_execution_ticks){
                tasks[task].task_service_routine();
                tasks[task].task_last_execution_ticks = 0;
            }
    }

}

int main()
{
    stdio_init_all();
    ssd_init();
    //statemachine_entry();
    _ADS_INIT(ADS_SPS_100);
    _ADS_WAIT_FOR_DRDY();

    add_repeating_timer_ms(TICK_TIMER_INTERVAL_MS, timer_tick_increment, NULL, &timer0);

    while(1){
        task_scheduler_tick();
    }
    
}


void timer_tick_increment(){
    system_ticks++;
}



//wrapper damit der task scheduler immer gleich sein kann
static void read_voltages(void){
    ADS_GET_VOLTAGES(&voltages);
}

static void calculate_temperatures(void){
    convert_voltages_to_temperatures(&temperatures, &voltages);
}


void print_data(){
            for(uint8_t channel=0; channel<ADS_CHANNEL_COUNT; channel ++){
                printf(" C%u:%f", channel , temperatures.data_array[channel]);

            }
        printf(" \n");
}


void led_toggle(){

    gpio_xor_mask(1<<LED_PIN);

}

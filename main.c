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
#include "tasks.h"


void init_gpios(){

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(PIN_CHARGE_RELAY);
    gpio_set_dir(PIN_CHARGE_RELAY, GPIO_OUT);
    gpio_put(PIN_CHARGE_RELAY, 0); 

    gpio_init(PIN_DISCHARGE_RELAY);
    gpio_set_dir(PIN_DISCHARGE_RELAY, GPIO_OUT);
    gpio_put(PIN_DISCHARGE_RELAY, 0); 

}

int main()
{
    stdio_init_all();
    //ssd_init();
    init_gpios();


    _ADS_INIT(ADS_SPS_100);
    _ADS_WAIT_FOR_DRDY();
    task_scheduler_init();
  
    while(1){
        task_scheduler_tick();
    }
    
}


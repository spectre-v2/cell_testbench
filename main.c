// Author: Leif Stechmann
// Date: 19.9.2026

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware_defines.h"
#include "ads_driver.h"
#include "statemachine.h"
#include "ssd1309.h"



int main()
{
    stdio_init_all();
    ssd_init();
    //statemachine_entry();
    _ADS_INIT(ADS_SPS_100);
    _ADS_WAIT_FOR_DRDY();


    while(1){
        
            ADS_FULL_SAMPLE_VOLTAGES_t voltages = ADS_GET_VOLTAGES();

            for(uint8_t channel=0; channel<ADS_CHANNEL_COUNT; channel ++){
                printf(" C%u:%f", channel , voltages.data_array[channel]);

            }
        printf(" \n");

        led_toggle();

    
    }
    
}


void led_toggle(){

    gpio_xor_mask(1<<LED_PIN);

}

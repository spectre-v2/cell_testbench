// Author: Leif Stechmann
// Date: 19.9.2026

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware_defines.h"
#include "ads_driver.h"
#include "statemachine.h"



int main()
{
    stdio_init_all();

    spi_init(ADS_SPI_PORT, ADS_SPI_DATARATE);
    spi_set_format(ADS_SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(ADS_PIN_RX, GPIO_FUNC_SPI);
    gpio_set_function(ADS_PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(ADS_PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(ADS_PIN_TX, GPIO_FUNC_SPI);
    
    
    gpio_set_dir(ADS_PIN_CS, GPIO_OUT);
    gpio_put(ADS_PIN_CS, 1);
  
    //statemachine_entry();

    while(1){

        ADS_FULL_SAMPLE_VOLTAGES_t voltages;
        voltages = ADS_GET_VOLTAGES();
       // full_sample_temperatures_t temperatures = convert_voltages_to_temperatures(&voltages);
        for(uint8_t channel = 0; channel < ADS_CHANNEL_COUNT; channel ++){
            printf("Channel %u: V\n ", channel+1, voltages.data_array[channel]);
        }
        sleep_ms(1000);


        



    }


}

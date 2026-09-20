// Author: Leif Stechmann
// Date: 19.9.2026

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware_defines.h"
//#include "ads_driver.h"
#include "statemachine.h"



int main()
{
    stdio_init_all();

    spi_init(ADS_SPI_PORT, ADS_SPI_DATARATE);
    gpio_set_function(ADS_PIN_RX, GPIO_FUNC_SPI);
    gpio_set_function(ADS_PIN_CS,   GPIO_FUNC_SPI);
    gpio_set_function(ADS_PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(ADS_PIN_TX, GPIO_FUNC_SPI);
    
    
    gpio_set_dir(ADS_PIN_CS, GPIO_OUT);
    gpio_put(ADS_PIN_CS, 1);
  
    statemachine_entry();

}

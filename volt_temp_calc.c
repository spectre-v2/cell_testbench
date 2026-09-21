

//standard C headers
#include <stdio.h>
#include <stdint.h>

//pico headers
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware_defines.h"

//project modules
#include "ads_driver.h"
#include "volt_temp_calc.h"

#define TEMP_CALC_R_FIX_0 10000 // Ohm
#define TEMP_CALC_R_FIX_1 10000 // Ohm
#define TEMP_CALC_R_FIX_2 10000 // Ohm
#define TEMP_CALC_R_FIX_3 10000 // Ohm
#define TEMP_CALC_R_FIX_4 10000 // Ohm
#define TEMP_CALC_R_FIX_5 10000 // Ohm
#define TEMP_CALC_R_FIX_6 10000 // Ohm
#define TEMP_CALC_R_FIX_7 10000 // Ohm

#define TEMP_CALC_NTC_R_NO 10000 //Ohm


void calc_R_ntc();


void calc_temperatures(ADS_FULL_SAMPLE_VOLTAGES_t* voltages){
    
    full_sample_temperatures_t temperatures;

    for (uint8_t channel; channel < ADS_CHANNEL_COUNT; channel ++){
        temperatures.data_array[channel]= voltages->data_array[channel];
    }


}

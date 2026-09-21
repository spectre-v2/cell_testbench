

//standard C headers
#include <stdio.h>
#include <stdint.h>
#include <math.h>

//pico headers
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware_defines.h"

//project modules
#include "ads_driver.h"
#include "volt_temp_calc.h"

#define TEMP_CALC_DIVIDER_VOLTAGE 5 //Volt
#define TEMP_CALC_R_FIX 10000 // Ohm
#define TEMP_CALC_NTC_R_NOMINAL 10000 //Ohm
#define TEMP_CALC_NTC_T_NOMINAL 298.15 //Kelvin
#define TEMP_CALC_NTC_BETA 3435 //Kelvin



/**
 * @brief Calculates NTC temperature using the beta equation.
 * @param ntc_voltage Voltage across the lower NTC resistor, in volts.
 * @return Temperature in kelvin.
 * @pre 0 < ntc_voltage < TEMP_CALC_DIVIDER_VOLTAGE; not checked by this function.
 * @verbatim
 *
 * 			TEMP_CALC_DIVIDER_VOLTAGE
 * 			 │
 * 			 │
 * 			 █ R_FIX
 * 			 │
 * 			 ┣─────── ntc_voltage
 * 			 │
 * 			 █ R_NTC
 * 			 │
 * 			 │
 * 			GND
 *
 * @endverbatim
 * @see https://de.wikipedia.org/wiki/Hei%C3%9Fleiter
 */

float temperature_from_voltage(float ntc_voltage){

    float r_ntc = ((ntc_voltage*TEMP_CALC_R_FIX)/(TEMP_CALC_DIVIDER_VOLTAGE-ntc_voltage));

    float temperature;
    //https://de.wikipedia.org/wiki/Hei%C3%9Fleiter

    temperature = 1.0f/(   (1.0f/TEMP_CALC_NTC_T_NOMINAL) + (1.0f/TEMP_CALC_NTC_BETA) * log(r_ntc/TEMP_CALC_NTC_R_NOMINAL)  );
    return temperature;
}


/**
 * @brief Samples all eight channel voltages and calculates their NTC temperatures.
 * @return Temperatures in kelvin; array index 0 corresponds to AIN0.
 * @pre The ADC is ready; all channels use the NTC divider shown above and valid input voltages.
 */

full_sample_temperatures_t sample_temperatures(){
    
    ADS_FULL_SAMPLE_VOLTAGES_t voltages = ADS_GET_VOLTAGES();
    full_sample_temperatures_t temperatures;

    for (uint8_t channel =0; channel < ADS_CHANNEL_COUNT; channel ++){
        
        temperatures.data_array[channel]= temperature_from_voltage(voltages.data_array[channel]);
    }

    return temperatures;

}

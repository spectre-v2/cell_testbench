#pragma once

#include <stdint.h>

typedef union {

struct {
        float ch_1_temp;
        float ch_2_temp;
        float ch_3_temp;
        float ch_4_temp;
        float ch_5_temp;
        float ch_6_temp;
        float ch_7_temp;
        float ch_8_temp;

}data_fields;

float data_array[8];

}full_sample_temperatures_t;

/**
 * @brief Calculates NTC temperature using the beta equation.
 * @param ntc_voltage Voltage across the lower NTC resistor, in volts.
 * @return Temperature in kelvin.
 * @pre Input voltage is strictly between zero and the divider supply voltage.
 */
float temperature_from_voltage(float ntc_voltage);

/**
 * @brief Samples all eight channels and calculates their NTC temperatures.
 * @return Temperatures in kelvin; array index 0 corresponds to AIN0.
 * @pre The ADC is ready and all channels use the configured NTC divider.
 */
full_sample_temperatures_t sample_temperatures(void);

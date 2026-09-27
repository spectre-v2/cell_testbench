#pragma once

#include <stdint.h>
#include "ads_driver.h"

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
 * @brief Calculates NTC temperatures for the six configured temperature channels.
 * @param[out] temperatures Receives temperatures in kelvin; index 0 corresponds to AIN0.
 * @param[in] voltages Previously sampled channel voltages.
 * @pre The ADC is ready and all channels use the configured NTC divider.
 */
void convert_voltages_to_temperatures(full_sample_temperatures_t *temperatures,
                                     ADS_FULL_SAMPLE_VOLTAGES_t *voltages);

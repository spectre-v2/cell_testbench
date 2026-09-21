#pragma once

#include <stdint.h>

typedef union {

struct {
        int32_t ch_1_res_temp;
        int32_t ch_2_res_temp;
        int32_t ch_3_res_temp;
        int32_t ch_4_res_temp;
        int32_t ch_5_res_temp;
        int32_t ch_6_res_temp;
        int32_t ch_7_res_temp;
        int32_t ch_8_res_temp;

}data_fields;

int32_t data_array[8];

}full_sample_temperatures_t;
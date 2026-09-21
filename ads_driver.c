//Author: Leif Stechmann
//Date: 21.09.2026

//standard C headers
#include <stdio.h>
#include <stdint.h>

//pico headers
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware_defines.h"

//project modules
#include <ads_driver.h>


void cs_low(){
    gpio_put(ADS_PIN_CS, 0);
}
void cs_high(){
    sleep_us(ADS_TIME_BEFORE_CS_1);
    gpio_put(ADS_PIN_CS, 1);
}


//-----------------------------------------basic operations----------------------------------------------
uint8_t _ADS_READ_REG(uint8_t reg_adr){
   
    uint8_t cmd[2] = {
        ADS_CMD_RREG | reg_adr,
        0b00000000
    };

    uint8_t reg_read_buffer;
    cs_low();
    spi_write_blocking(ADS_SPI_PORT, cmd, 2 );
    sleep_us(ADS_TIME_SPI_T6);
    spi_read_blocking(ADS_SPI_PORT,0x00, &reg_read_buffer, 1);
    cs_high();
    return reg_read_buffer;
}

void _ADS_WRITE_REG(uint8_t reg_adr, uint8_t reg_cont){
        
        uint8_t cmd[3] = {
        ADS_CMD_WREG | reg_adr,
        0b00000000,
        reg_cont
    };
    cs_low();
    spi_write_blocking(ADS_SPI_PORT, cmd, 3);
    cs_high();
}

void _ADS_SEND_CMD(uint8_t cmd){
    cs_low();
    spi_write_blocking(ADS_SPI_PORT, &cmd, 1);
    cs_high();
}


void _ADS_SET_CHANNEL(uint8_t channel){

      ADS_REG_MUX_t reg_mux = {
        .data_fields.PSEL = channel,
        .data_fields.NSEL = 0b1000
    };

    _ADS_WRITE_REG(ADS_REG_ADR_MUX, reg_mux.raw_data);
}

bool _ADS_READY(){
   ADS_REG_STATUS_t tmp_reg_stat;
   tmp_reg_stat.raw_data = _ADS_READ_REG(ADS_REG_ADR_STATUS);

   return (bool)!tmp_reg_stat.data_fields.DRDYn;
}

//-----------------------initial register configuration-----------------------------

void _ADS_INIT(){
    ADS_REG_STATUS_t reg_stat = {
        .data_fields.ORDER = 0,
        .data_fields.ACAL   =   0,
        .data_fields.BUFEN = 0
    };

    ADS_REG_MUX_t reg_mux = {
        .data_fields.PSEL = 0,
        .data_fields.NSEL = 0b1000
    };

    ADS_REG_ADCON_t reg_adcon = {
        .data_fields.CLK = 0b00,
        .data_fields.SDCS  =   0b00,
        .data_fields.PGA  =   0b000

    };

    ADS_REG_DRATE_t reg_drate = {
        .raw_data = ADS_SPS_1k
    };


_ADS_SEND_CMD(ADS_CMD_RESET);
sleep_us(ADS_TIME_CAL);
_ADS_WRITE_REG(ADS_REG_ADR_STATUS, reg_stat.raw_data);
_ADS_WRITE_REG(ADS_REG_ADR_MUX, reg_mux.raw_data);
_ADS_WRITE_REG(ADS_REG_ADR_ADCON, reg_adcon.raw_data);
_ADS_WRITE_REG(ADS_REG_ADR_DRATE, reg_drate.raw_data);

}

//--------------------------------------sampling-----------------------------------------------

void _ADS_SAMPLE_CHANNEL(uint8_t channel, int32_t* sample_data){

    uint8_t rdata_cmd = ADS_CMD_RDATA;
    uint8_t sample_data_array[3];

    
    _ADS_SET_CHANNEL(channel);
    _ADS_SEND_CMD(ADS_CMD_SYNC);
    sleep_us(ADS_TIME_SAMPLE);
    _ADS_SEND_CMD(ADS_CMD_WAKEUP);

    while(!_ADS_READY()) {
        sleep_us(ADS_TIME_SAMPLE_DELAY);
    }

    cs_low();
    spi_write_blocking(ADS_SPI_PORT, &rdata_cmd, 1);
    sleep_us(ADS_TIME_SPI_T6);
    spi_read_blocking(ADS_SPI_PORT, 0x00, sample_data_array, 3);
    cs_high();

    bool negative=  (sample_data_array[0] & 0b10000000);

    *sample_data=    ((uint32_t)sample_data_array[0] <<16) |
                    ((uint32_t)sample_data_array[1] << 8) |
                    (uint32_t)sample_data_array[2];

    if(negative){
        *sample_data = (int32_t)((uint32_t)*sample_data | (0b11111111u << 24));
    }
    
}

void _ADS_FULL_SAMPLE(ADS_FULL_SAMPLE_RAW_t *full_sample_result){

for(uint8_t channel = 0; channel < ADS_CHANNEL_COUNT ; channel ++){
    _ADS_SAMPLE_CHANNEL(channel, &full_sample_result->data_array[channel]);
    }

}


void _ADS_SAMPLE_TO_VOLTAGE(ADS_FULL_SAMPLE_RAW_t* raw_data, ADS_FULL_SAMPLE_VOLTAGES_t* voltages){
    for(uint8_t channel = 0; channel < ADS_CHANNEL_COUNT; channel ++){

        voltages->data_array[channel]= ADS_VREFP * (raw_data->data_array[channel]/ADS_ADC_MAX);
    }
}



//main API

ADS_FULL_SAMPLE_VOLTAGES_t ADS_GET_VOLTAGES(){

    ADS_FULL_SAMPLE_VOLTAGES_t voltage_data;
    ADS_FULL_SAMPLE_RAW_t raw_data;

    _ADS_FULL_SAMPLE(&raw_data);
    _ADS_SAMPLE_TO_VOLTAGE(&raw_data, &voltage_data);

    return voltage_data;

}







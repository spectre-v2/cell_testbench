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


/** @brief Selects the ADC by driving chip select LOW. */
void cs_low(){
    gpio_put(ADS_PIN_CS, 0);
}
/** @brief Waits for the CS hold time, then drives chip select HIGH. */
void cs_high(){
    sleep_us(ADS_TIME_CS_DELAY);
    gpio_put(ADS_PIN_CS, 1);
}


/**
 * @brief Reads a single ADC register.
 * @param reg_adr Register address (0x00 to 0x0A).
 * @return Register byte read from the ADC.
 */
uint8_t _ADS_READ_REG(uint8_t reg_adr){
   
    uint8_t cmd[2] = {
        ADS_CMD_RREG | reg_adr,
        0b00000000
    };

    uint8_t reg_read_buffer;
    cs_low();
    spi_write_blocking(ADS_SPI_PORT, cmd, 2 );
    sleep_us(ADS_TIME_SPI_MSG_DELAY);
    spi_read_blocking(ADS_SPI_PORT,0x00, &reg_read_buffer, 1);
    cs_high();
    return reg_read_buffer;
}

/**
 * @brief Writes a single ADC register.
 * @param reg_adr Address of the writable register.
 * @param reg_cont New register value.
 */
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

/**
 * @brief Sends a single command in its own CS transaction.
 * @param cmd Command byte; the caller handles additional command delays.
 */
void _ADS_SEND_CMD(uint8_t cmd){
    cs_low();
    spi_write_blocking(ADS_SPI_PORT, &cmd, 1);
    cs_high();
}


/**
 * @brief Selects an analog input relative to AINCOM.
 * @param channel Channel number from 0 to 7.
 */
void _ADS_SET_CHANNEL(uint8_t channel){

      ADS_REG_MUX_t reg_mux = {
        .data_fields.PSEL = channel,
        .data_fields.NSEL = 0b1000 //AINCOM
    };

    _ADS_WRITE_REG(ADS_REG_ADR_MUX, reg_mux.raw_data);
}

/**
 * @brief Idles until DRDY gpio goes low, signaling sample data is ready.
 */
void _ADS_WAIT_FOR_DRDY(){
    while(gpio_get(ADS_PIN_DRDY) != 0){
        __asm volatile ("nop");
    }
}

/**
 * @brief Resets and configures the ADC, then starts self-calibration.
 * @pre SPI and CS are initialized. Wait for calibration to finish before sampling.

 */
void _ADS_INIT(uint8_t samples_per_second){
    ADS_REG_STATUS_t reg_stat = {
        .data_fields.ORDER = 0,
        .data_fields.ACAL   =   0,
        .data_fields.BUFEN = 1
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
        .raw_data = samples_per_second
    };


_ADS_SEND_CMD(ADS_CMD_RESET);
_ADS_WAIT_FOR_DRDY();

_ADS_WRITE_REG(ADS_REG_ADR_STATUS, reg_stat.raw_data);
_ADS_WRITE_REG(ADS_REG_ADR_MUX, reg_mux.raw_data);
_ADS_WRITE_REG(ADS_REG_ADR_ADCON, reg_adcon.raw_data);
_ADS_WRITE_REG(ADS_REG_ADR_DRATE, reg_drate.raw_data);

_ADS_SEND_CMD(ADS_CMD_SELFCAL);
_ADS_WAIT_FOR_DRDY();


}


/**
 * @brief Samples one channel relative to AINCOM using blocking transfers.
 * @param channel Channel number from 0 to 7.
 * @param[out] sample_data Valid pointer receiving the sign-extended ADC raw value.
 * @pre The ADC is configured and calibration has completed.
 * @note Waits for data readiness without a timeout.
 */
void _ADS_SAMPLE_CHANNEL(uint8_t channel, int32_t* sample_data){

    uint8_t rdata_cmd = ADS_CMD_RDATA;
    uint8_t sample_data_array[3];

    
    _ADS_WAIT_FOR_DRDY();

    _ADS_SET_CHANNEL(channel);
    _ADS_SEND_CMD(ADS_CMD_SYNC);
    sleep_us(10);
    _ADS_SEND_CMD(ADS_CMD_WAKEUP);

 
    _ADS_WAIT_FOR_DRDY();

    cs_low();
    spi_write_blocking(ADS_SPI_PORT, &rdata_cmd, 1);
    sleep_us(ADS_TIME_SPI_MSG_DELAY);
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


/**
 * @brief Samples all eight channels sequentially and converts their readings to voltages.
 * @return Channel voltages in volts relative to AINCOM; array index 0 corresponds to AIN0.
 * @pre The ADC is ready for sampling; PGA = 1 and VREFN = 0 V.
 */
ADS_FULL_SAMPLE_VOLTAGES_t ADS_GET_VOLTAGES(void){

    ADS_FULL_SAMPLE_VOLTAGES_t voltage_data;

    for(uint8_t channel = 0; channel < ADS_CHANNEL_COUNT; channel ++){

        int32_t channel_raw_data;
        _ADS_SAMPLE_CHANNEL(channel, &channel_raw_data);

        //The factor 2.0f comes from the ADS1256 internal scaling.
        voltage_data.data_array[channel]= 2.0f * ADS_VREFP * ((float)channel_raw_data/ADS_ADC_MAX);
    }

    return voltage_data;

}







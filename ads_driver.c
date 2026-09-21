//Author: Leif Stechmann
//Date: 21.09.2026

#include <stdint.h>
#include <ads_driver.h>
#include "hardware/spi.h"
#include "hardware_defines.h"

uint8_t _ADS_SPI_READ_REG(uint8_t reg_adr){
   
    uint8_t cmd[2] = {
        ADS_CMD_RREG | reg_adr,
        0b00000000
    };


    uint8_t reg_read_buffer;
    spi_write_blocking(ADS_SPI_PORT, cmd, 2 );
    sleep_us(1);
    spi_read_blocking(ADS_SPI_PORT,0x00, &reg_read_buffer, 1);
    return reg_read_buffer;
}

//Author: Leif Stechmann
//Date: 19.9.2026


#pragma once
#include <hardware/i2c.h>#

#define ADS_SPI_PORT spi0

#define ADS_PIN_RX  4
#define ADS_PIN_CS   6
#define ADS_PIN_SCK  2
#define ADS_PIN_TX 3
#define ADS_PIN_DRDY 5

#define SSD_I2C_PORT i2c

#define SSD_PIN_SCL 2
#define SSD_PIN_SDA 1



#define LED_PIN 25

#define ADS_SPI_DATARATE 1000*1000
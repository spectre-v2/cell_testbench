//Author: Leif Stechmann
//Date: 19.9.2026

#pragma once
// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define ADS_SPI_PORT spi0
#define ADS_PIN_MISO 16
#define ADS_PIN_CS   17
#define ADS_PIN_SCK  18
#define ADS_PIN_MOSI 19

#define ADS_SPI_DATARATE 1000*1000
// Author: Leif Stechmann
// Date: 19.9.2026

#pragma once
#include <stdbool.h>
#include <stdint.h>


//timing constants
#define ADS_TIME_SPI_T6 10 //us
#define ADS_TIME_BEFORE_CS_1 2 
#define ADS_TIME_SAMPLE_DELAY 1000 //us
#define ADS_TIME_SAMPLE 10 //us
#define ADS_TIME_CAL 1000 //us

//sample rates
#define ADS_SPS_30k 0b11110000 
#define ADS_SPS_15k 0b11100000
#define ADS_SPS_7k5 0b11010000
#define ADS_SPS_3k75 0b11000000
#define ADS_SPS_2k 0b10110000
#define ADS_SPS_1k 0b10100001
#define ADS_SPS_500 0b10010010
#define ADS_SPS_100 0b10000010
#define ADS_SPS_60 0b01110010
#define ADS_SPS_50 0b01100011
#define ADS_SPS_30 0b01010011
#define ADS_SPS_25 0b01000011
#define ADS_SPS_15 0b00110011
#define ADS_SPS_10 0b00100011
#define ADS_SPS_5 0b00010011
#define ADS_SPS_2_5 0b00000011

//Register addresses
#define ADS_REG_ADR_STATUS 0x00u
#define ADS_REG_ADR_MUX    0x01u
#define ADS_REG_ADR_ADCON  0x02u
#define ADS_REG_ADR_DRATE  0x03u
#define ADS_REG_ADR_IO     0x04u
#define ADS_REG_ADR_OFC0   0x05u
#define ADS_REG_ADR_OFC1   0x06u
#define ADS_REG_ADR_OFC2   0x07u
#define ADS_REG_ADR_FSC0   0x08u
#define ADS_REG_ADR_FSC1   0x09u
#define ADS_REG_ADR_FSC2   0x0Au

//commands
#define ADS_CMD_WAKEUP  0b00000000  //Completes SYNC and Exits Standby Mode
#define ADS_CMD_RDATA   0b00000001  //Read Data
#define ADS_CMD_RDATAC  0b00000011 //Read Data Continuously
#define ADS_CMD_SDATAC  0b00001111 //Stop Read Data Continuously
#define ADS_CMD_RREG    0b00010000 //Read from REG (mask last 4 bits)
#define ADS_CMD_WREG    0b01010000 //Write to REG
#define ADS_CMD_SELFCAL 0b11110000 //Offset and Gain Self-Calibration
#define ADS_CMD_SELFOCAL    0b11110001 //Offset Self-Calibration
#define ADS_CMD_SELFGCAL    0b11110010 //Gain Self-Calibration
#define ADS_CMD_SYSOCAL 0b11110011 //System Offset Calibration
#define ADS_CMD_SYSGCAL 0b11110100 //System Gain Calibration
#define ADS_CMD_SYNC    0b11111100 //Synchronize the A/D Conversion
#define ADS_CMD_STANDBY 0b11111101  //Begin Standby Mode
#define ADS_CMD_RESET   0b11111110 //Reset to Power-Up Values

//register data types
typedef union {
    struct {
        uint8_t DRDYn : 1; /* Nur lesbar; 0 = Daten bereit. */
        uint8_t BUFEN : 1;
        uint8_t ACAL  : 1;
        uint8_t ORDER : 1;
        uint8_t ID0   : 1; /* ID0 bis ID3: nur lesbar. */
        uint8_t ID1   : 1;
        uint8_t ID2   : 1;
        uint8_t ID3   : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_STATUS_t;

/* MUX: Reset = 0x01 (AINP = AIN0, AINN = AIN1). */
typedef union {
    struct {
        uint8_t NSEL    :   4;
        uint8_t PSEL    :   4;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_MUX_t;

/* ADCON: Reset = 0x20; Bit 7 ist reserviert und muss 0 bleiben. */
typedef union {
    struct {
        uint8_t PGA     : 3;
        uint8_t SDCS    : 2;
        uint8_t CLK     : 2;
        uint8_t reserved : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_ADCON_t;

/* DRATE: Reset = 0xF0 (30000 SPS bei fCLKIN = 7,68 MHz). */
typedef union {
    struct {
        uint8_t DR0 : 1;
        uint8_t DR1 : 1;
        uint8_t DR2 : 1;
        uint8_t DR3 : 1;
        uint8_t DR4 : 1;
        uint8_t DR5 : 1;
        uint8_t DR6 : 1;
        uint8_t DR7 : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_DRATE_t;

/* IO: Reset = 0xE0; DIRn: 0 = Ausgang, 1 = Eingang. */
typedef union {
    struct {
        uint8_t DIO0 : 1;
        uint8_t DIO1 : 1;
        uint8_t DIO2 : 1;
        uint8_t DIO3 : 1;
        uint8_t DIR0 : 1;
        uint8_t DIR1 : 1;
        uint8_t DIR2 : 1;
        uint8_t DIR3 : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_IO_t;

/*
 * OFC und FSC bestehen jeweils aus drei Bytes: Byte 0 = LSB, Byte 2 = MSB.
 * Ihre Resetwerte haengen von der Kalibrierung ab und sind nicht fest 0.
 */

/* OFC0: Offset-Kalibrierung, Bits 0-7. */
typedef union {
    struct {
        uint8_t OFC00 : 1;
        uint8_t OFC01 : 1;
        uint8_t OFC02 : 1;
        uint8_t OFC03 : 1;
        uint8_t OFC04 : 1;
        uint8_t OFC05 : 1;
        uint8_t OFC06 : 1;
        uint8_t OFC07 : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_OFC0_t;

/* OFC1: Offset-Kalibrierung, Bits 8-15. */
typedef union {
    struct {
        uint8_t OFC08 : 1;
        uint8_t OFC09 : 1;
        uint8_t OFC10 : 1;
        uint8_t OFC11 : 1;
        uint8_t OFC12 : 1;
        uint8_t OFC13 : 1;
        uint8_t OFC14 : 1;
        uint8_t OFC15 : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_OFC1_t;

/* OFC2: Offset-Kalibrierung, Bits 16-23. */
typedef union {
    struct {
        uint8_t OFC16 : 1;
        uint8_t OFC17 : 1;
        uint8_t OFC18 : 1;
        uint8_t OFC19 : 1;
        uint8_t OFC20 : 1;
        uint8_t OFC21 : 1;
        uint8_t OFC22 : 1;
        uint8_t OFC23 : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_OFC2_t;

/* FSC0: Verstaerkungskalibrierung, Bits 0-7. */
typedef union {
    struct {
        uint8_t FSC00 : 1;
        uint8_t FSC01 : 1;
        uint8_t FSC02 : 1;
        uint8_t FSC03 : 1;
        uint8_t FSC04 : 1;
        uint8_t FSC05 : 1;
        uint8_t FSC06 : 1;
        uint8_t FSC07 : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_FSC0_t;

/* FSC1: Verstaerkungskalibrierung, Bits 8-15. */
typedef union {
    struct {
        uint8_t FSC08 : 1;
        uint8_t FSC09 : 1;
        uint8_t FSC10 : 1;
        uint8_t FSC11 : 1;
        uint8_t FSC12 : 1;
        uint8_t FSC13 : 1;
        uint8_t FSC14 : 1;
        uint8_t FSC15 : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_FSC1_t;

/* FSC2: Verstaerkungskalibrierung, Bits 16-23. */
typedef union {
    struct {
        uint8_t FSC16 : 1;
        uint8_t FSC17 : 1;
        uint8_t FSC18 : 1;
        uint8_t FSC19 : 1;
        uint8_t FSC20 : 1;
        uint8_t FSC21 : 1;
        uint8_t FSC22 : 1;
        uint8_t FSC23 : 1;
    } data_fields;

    uint8_t raw_data;
} ADS_REG_FSC2_t;


// struct containing all sampled data of all channels 
typedef struct {
        int32_t ch_1_res_bin;
        int32_t ch_2_res_bin;
        int32_t ch_3_res_bin;
        int32_t ch_4_res_bin;
        int32_t ch_5_res_bin;
        int32_t ch_6_res_bin;
        int32_t ch_7_res_bin;
        int32_t ch_8_res_bin;

}ADS_FULL_SAMPLE_RESULT_t;

/** @brief Setzt Chip Select auf LOW. */
void cs_low(void);

/** @brief Setzt Chip Select auf HIGH. */
void cs_high(void);

/** @brief Liest ein Register. */
uint8_t _ADS_READ_REG(uint8_t reg_adr);

/** @brief Schreibt ein Register. */
void _ADS_WRITE_REG(uint8_t reg_adr, uint8_t reg_cont);

/** @brief Sendet einen einzelnen Befehl. */
void _ADS_SEND_CMD(uint8_t cmd);

/** @brief Waehlt Kanal 0 bis 7 gegen AINCOM aus. */
void _ADS_SET_CHANNEL(uint8_t channel);

/** @brief Prueft im Statusregister, ob Messdaten bereitstehen. */
bool _ADS_READY(void);

/** @brief Setzt den ADC zurueck und konfiguriert seine Register. */
void _ADS_INIT(void);

/** @brief Misst einen Kanal blockierend und liefert den ADC-Rohwert. */
int32_t _ADS_SAMPLE_CHANNEL(uint8_t channel);

/** @brief Misst alle acht Kanaele nacheinander. */
ADS_FULL_SAMPLE_RESULT_t _ADS_FULL_SAMPLE(void);

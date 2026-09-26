#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1309.h"
#include "hardware_defines.h"
#include "font.h"


uint8_t ssd_config[]={
    SSD_CTRL_CMD,
    SSD_COMMAND_POWER_ON,
    SSD_COMMAND_SEG_INVERT_ON,
    SSD_COMMAND_COM_INVERT_ON,
    SSD_COMMAND_ADR_MODE, SSD_ADR_MODE_HOR,
    SSD_COMMAND_SET_COL_ADR, 0x00, 0x7f,
    SSD_COMMAND_SET_PAGE_ADR, 0X00, 0X07,
    SSD_COMMAND_ON
};


static uint8_t framebuffer[SSD_FRAME_SIZE] = {0};

static uint8_t current_page = 0;
static uint8_t current_column = 0;


void ssd_update_display(){
    
    uint8_t tx_buffer[SSD_FRAME_SIZE + 1];
    tx_buffer[0] = SSD_CTRL_DAT;
    memcpy(&tx_buffer[1], &framebuffer, sizeof(framebuffer));

    i2c_write_blocking(SSD_I2C_PORT, SSD_DEVICE_ADDRESS, framebuffer, sizeof(framebuffer), false);
}


void ssd_clear(){

    framebuffer = {0};
    ssd_update_display();

}

   


void ssd_init(){
    
    i2c_init(SSD_I2C_PORT,SSD_I2C_FREQ);
    gpio_set_function(SSD_PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(SSD_PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SSD_PIN_SDA);
    gpio_pull_up(SSD_PIN_SCL);

    i2c_write_blocking(SSD_I2C_PORT, SSD_DEVICE_ADDRESS, ssd_config, sizeof(ssd_config), false);
}

void ssd_write_symbol( *text){

    
    
}

void ssd_write_text(char *line){
    

}

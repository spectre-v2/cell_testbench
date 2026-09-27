#include <stdio.h>
#include <string.h>
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

    i2c_write_blocking(SSD_I2C_PORT, SSD_DEVICE_ADDRESS, tx_buffer, sizeof(tx_buffer), false);
}


void ssd_clear(){

    memset(framebuffer, 0, sizeof(framebuffer));
    current_page = 0;
    current_column = 0;
    ssd_update_display();

}


void ssd_init(){
    
    i2c_init(SSD_I2C_PORT,SSD_I2C_FREQ);
    gpio_set_function(SSD_PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(SSD_PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SSD_PIN_SDA);
    gpio_pull_up(SSD_PIN_SCL);

    i2c_write_blocking(SSD_I2C_PORT, SSD_DEVICE_ADDRESS, ssd_config, sizeof(ssd_config), false);

    ssd_clear();
}

void ssd_write_symbol(char symbol){

    uint16_t framebuffer_target_pos = current_column + ( current_page * SSD_WIDTH_PIXEL);

    memcpy(&framebuffer[framebuffer_target_pos], &orbitron[symbol], SSD_SYMBOL_WIDTH);

        current_column = current_column + SSD_SYMBOL_WIDTH;

    if (current_column==SSD_WIDTH_PIXEL){
        current_page++;
        current_column = 0;
    }

    if (current_page==SSD_HEIGHT_PAGES){
        current_page = 0;
    }
    
    
}



void ssd_write_text(char *text){

    uint8_t text_pos = 0;

    while(text[text_pos] != '\0'){

        ssd_write_symbol(text[text_pos]);
        text_pos++;

    }
    
}

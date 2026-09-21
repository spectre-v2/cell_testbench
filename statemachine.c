//Author: Lisa, Date: 18.09.2026

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"


//Required commands:

// START_DISCHARGE_PULSE 
// START_CHARGE_PULSE
// START_DISCHARGE_CONT
// START_CHARGE_CONT


//Temperature sampling API: full_sample_temperatures_t sample_temperatures();


#define LED_PIN 1

#define PIN_CHARGE_RELAY 4
#define PIN_DISCHARGE_RELAY 5

/*

int statemachine_entry()
{
  
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_function(LED_PIN,GPIO_FUNC_SIO);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    char wort[20];

    while (scanf("%19s", wort)==1) {
        if (strcmp(wort,"lisa")==0) {
        printf("LED blinkt!\n");

    for (int i = 1; i <= 10; i++){

        gpio_put(LED_PIN,true);
        sleep_ms(200);
        gpio_put(LED_PIN,false);
        sleep_ms(200);
    }


}

*/


int statemachine_entry(){
    const uint RELAY_PIN = 10;
    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);
    gpio_put(RELAY_PIN, 0); 

    const uint RELAY_PIN2 = 20;
    gpio_init(RELAY_PIN2);
    gpio_set_dir(RELAY_PIN2, GPIO_OUT);
    gpio_put(RELAY_PIN2, 0); 


    char wort[20];

    while (scanf("%19s", wort)==1) {

        if (strcmp(wort,"lisa")==0) {
            printf("LED blinkt!\n");

            for (int i = 1; i <= 4; i++) {
                gpio_put(RELAY_PIN, 1);
                sleep_ms(1000);
                gpio_put(RELAY_PIN, 0); 
                sleep_ms(1000);
            }
        }

        if (strcmp(wort,"leif")==0) {
        printf("LED 2 blinkt!\n");

        for (int i = 1; i <= 8; i++) {
        gpio_put(RELAY_PIN2, 1);
        sleep_ms(500);
        gpio_put(RELAY_PIN2, 0); 
        sleep_ms(500);
    }
}


}



}
//Autor: Lisa , Datum: 18.09.2026

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"


#define LED_PIN 16

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
    }}


}

}

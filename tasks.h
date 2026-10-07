#pragma once
#include <stdint.h>

//task struct
typedef struct {

    void (*service_routine)(void);
    uint32_t interval_ms;
    uint64_t next_due_ms;

}task_t;

//tasks
void print_data(void);
void led_toggle(void);
void init_relays(void);
void update_relays(void);
void task_scheduler_init(void);
void task_scheduler_tick(void);
uint64_t timestamp_ms(void);

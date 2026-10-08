#pragma once
#include <stdint.h>

//single task struct
typedef struct {

    void (*service_routine)(void);
    uint32_t interval_ms;
    uint64_t next_due_ms;
    bool task_enabled;
}task_t;

//task identifier
typedef enum{
    task_update_statemachine,
    task_read_voltages, 
    task_calculate_temperatures,
    task_led_toggle, 
    task_print_data, 
    task_update_charge_relay, 
    task_update_discharge_relay,
    TASK_COUNT
}task_id_t;

extern task_t tasks[];

//public functions
 void task_scheduler_init(void);
 void task_scheduler_tick(void);
 uint64_t timestamp_ms(void);

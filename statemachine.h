#pragma once

typedef enum {
    SYSTEM_IDLE_S,
    CHARGE_PULSE_S,
    CHARGE_CONT_S,
    DISCHARGE_PULSE_S,
    DISCHARGE_CONT_S
}SYSTEM_STATE_t;

extern SYSTEM_STATE_t system_state;
void update_statemachine(void);

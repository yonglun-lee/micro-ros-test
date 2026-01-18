#include "valves.h"
#include "pins.h"

void ValveManager::init() {
    pinMode(PIN_VALVE_1, OUTPUT);
    pinMode(PIN_VALVE_2, OUTPUT);
    pinMode(PIN_VALVE_3, OUTPUT);
    close_all();
    log_d("Valves Initialized");
}

void ValveManager::set_valve(int id, bool open) {
    int pin = -1;
    switch (id) {
        case 1: pin = PIN_VALVE_1; break;
        case 2: pin = PIN_VALVE_2; break;
        case 3: pin = PIN_VALVE_3; break;
        default: log_e("Invalid Valve ID: %d", id); return;
    }
    digitalWrite(pin, open ? HIGH : LOW);
    log_i("Valve %d %s", id, open ? "OPEN" : "CLOSED");
}

void ValveManager::close_all() {
    digitalWrite(PIN_VALVE_1, LOW);
    digitalWrite(PIN_VALVE_2, LOW);
    digitalWrite(PIN_VALVE_3, LOW);
}
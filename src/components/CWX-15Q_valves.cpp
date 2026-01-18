/**
 * @file valves.cpp
 * @brief Driver for CWX-15Q Motorized Valves.
 */

#include "driver/gpio.h"
#include "pins.h"

class ValveManager {
public:
    void init() {
        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << PIN_VALVE_1) | (1ULL << PIN_VALVE_2) | (1ULL << PIN_VALVE_3);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);
        
        // Default close
        close_all();
    }

    void set_valve(int id, bool open) {
        gpio_num_t pin;
        switch (id) {
            case 1: pin = PIN_VALVE_1; break;
            case 2: pin = PIN_VALVE_2; break;
            case 3: pin = PIN_VALVE_3; break;
            default: return;
        }
        gpio_set_level(pin, open ? 1 : 0);
    }

    void close_all() {
        gpio_set_level(PIN_VALVE_1, 0);
        gpio_set_level(PIN_VALVE_2, 0);
        gpio_set_level(PIN_VALVE_3, 0);
    }
};

ValveManager valve_driver;
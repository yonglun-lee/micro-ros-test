/**
 * @file pump.h
 * @brief Header for JY300 Water Pump Driver.
 */
#pragma once
#include <Arduino.h>

class Pump {
public:
    Pump();
    
    /**
     * @brief Initialize the pump hardware (PWM).
     */
    void init();

    /**
     * @brief Set the pump effort (speed).
     * @param effort Normalized speed from 0.0 (stop) to 1.0 (full speed).
     */
    void set_effort(float effort);

    /**
     * @brief Stop the pump immediately.
     */
    void stop();

private:
    bool is_enabled;
    float current_duty;
};
/**
 * @file linear_actuator.h
 * @brief Header for LD20 Linear Actuator Driver.
 */
#pragma once
#include <Arduino.h>

class LinearActuator {
public:
    /**
     * @brief Initialize Motor PWM and Potentiometer ADC.
     */
    void init();

    /**
     * @brief Drive the actuator.
     * @param speed Speed from -100 (Retract) to 100 (Extend). 0 to stop.
     */
    void drive(int speed);

    /**
     * @brief Get the raw potentiometer feedback.
     * @return ADC value (0-4095).
     */
    int get_position_raw();
};
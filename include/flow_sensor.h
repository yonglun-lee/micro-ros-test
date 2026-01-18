/**
 * @file flow_sensor.h
 * @brief Header for K24 RS485 Flow Sensor.
 */
#pragma once
#include <Arduino.h>

class FlowSensor {
public:
    /**
     * @brief Initialize RS485 UART.
     */
    void init();

    /**
     * @brief Read the current flow rate via Modbus.
     * @return Flow rate in L/min, or -1.0f on error.
     */
    float read_flow_rate();

private:
    uint16_t calculate_crc(uint8_t *buffer, uint16_t length);
};
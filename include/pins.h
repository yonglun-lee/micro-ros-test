/**
 * @file pins.h
 * @brief Hardware Pin Definitions for ESP32 Sprayer.
 */

#pragma once
#include <Arduino.h>

// User Inputs
#define PIN_BOOT_BUTTON     0
#define PIN_LED_SIG         18

// Liquid Level
#define PIN_ADC_LEVEL       23

// Moteck LD3 Linear Actuator (MD1)
#define PIN_ACTUATOR_POT    34
#define PIN_ACTUATOR_PWM    26
#define PIN_ACTUATOR_DIR    22

// JY300 Water Pump (MD2)
#define PIN_PUMP_PWM        19

// Valves (CWX-15Q)
#define PIN_VALVE_1         25
#define PIN_VALVE_2         33
#define PIN_VALVE_3         32

// RS485 Flow Sensor (K24)
// Note: Wiring confirmed based on standard ESP32 RS485 modules
#define PIN_RS485_TX        15 // DE/RE Control
#define PIN_RS485_DI        21 // TX -> DI
#define PIN_RS485_RO        36 // RX -> RO
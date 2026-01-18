/**
 * @file pins.h
 * @brief Hardware Pin Definitions for ESP32 Sprayer.
 */

#pragma once
#include "driver/gpio.h"

// User Inputs
#define PIN_BOOT_BUTTON     GPIO_NUM_0
#define PIN_LED_SIG         GPIO_NUM_18

// Liquid Level
#define PIN_ADC_LEVEL       GPIO_NUM_23

// Moteck LD3 Linear Actuator (MD1)
#define PIN_ACTUATOR_POT    GPIO_NUM_34
#define PIN_ACTUATOR_PWM    GPIO_NUM_26
#define PIN_ACTUATOR_DIR    GPIO_NUM_22

// JY300 Water Pump (MD2)
#define PIN_PUMP_PWM        GPIO_NUM_19

// Valves (CWX-15Q)
#define PIN_VALVE_1         GPIO_NUM_25
#define PIN_VALVE_2         GPIO_NUM_33
#define PIN_VALVE_3         GPIO_NUM_32

// RS485 Flow Sensor (K24)
#define PIN_RS485_TX        GPIO_NUM_15 // TTL_DRE (Direction) - Check wiring vs schematic
#define PIN_RS485_DI        GPIO_NUM_21 // TX -> DI
#define PIN_RS485_RO        GPIO_NUM_36 // RX -> RO
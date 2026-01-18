/**
 * @file config.h
 * @brief System configuration and constants.
 */

#pragma once

// Micro-ROS Configuration
#define ROS_DOMAIN_ID 0
#define ROS_NODE_NAME "sprayer_node"

// Task Priorities (Optional in Arduino, but good for reference)
#define ROS_TASK_PRIORITY 5
#define MOTOR_TASK_PRIORITY 4
#define SENSOR_TASK_PRIORITY 3

// Component Settings
#define PUMP_PWM_FREQ_HZ 5000
#define ACTUATOR_PWM_FREQ_HZ 10000
#define FLOW_SENSOR_BAUD 9600

// Flow Sensor matches Serial2 on ESP32
#define FLOW_SENSOR_UART 2
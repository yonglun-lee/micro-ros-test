/**
 * @file main.cpp
 * @brief Main entry point for Firmware (Arduino Framework).
 */

#include <Arduino.h>
#include <micro_ros_platformio.h>

#include "ros_context.h"
#include "pump.h"
#include "valves.h"
#include "flow_sensor.h"
#include "linear_actuator.h"
#include "config.h"

// Global Driver Instances
Pump pump_driver;
ValveManager valve_driver;
FlowSensor flow_driver;
LinearActuator actuator_driver;
RosContext ros_ctx;

// ROS Functions
extern void setup_subscribers();
extern void setup_publishers();
extern void setup_services();
extern void pump_callback(const void * msgin);
extern void valve_service_callback(const void * req, void * res);
extern void timer_callback(rcl_timer_t * timer, int64_t last_call_time);

void setup() {
    Serial.begin(115200);
    
    // 1. Hardware Init
    log_i("Initializing Hardware...");
    pump_driver.init();
    valve_driver.init();
    actuator_driver.init();
    flow_driver.init();

    // 2. Transport Init
    log_i("Connecting to WiFi: %s", ESP32_WIFI_SSID);
    IPAddress agent_ip;
    agent_ip.fromString(MICROROS_AGENT_IP);
    
    // FIX: Cast string literals to (char*) to resolve ISO C++ warning
    set_microros_wifi_transports(
        (char*)ESP32_WIFI_SSID,
        (char*)ESP32_WIFI_PWD,
        agent_ip,
        MICROROS_AGENT_PORT
    );

    // 3. Micro-ROS Init
    log_i("Initializing Micro-ROS...");
    rcl_allocator_t allocator = rcl_get_default_allocator();
    rclc_support_init(&ros_ctx.support, 0, NULL, &allocator);

    rclc_node_init_default(&ros_ctx.node, "sprayer_node", "", &ros_ctx.support);

    // 4. Create Entities
    setup_publishers();
    setup_subscribers();
    setup_services();
    
    // 5. Executor
    rclc_executor_init(&ros_ctx.executor, &ros_ctx.support.context, 4, &allocator);
    rclc_executor_add_timer(&ros_ctx.executor, &ros_ctx.timer);
    rclc_executor_add_subscription(&ros_ctx.executor, &ros_ctx.pump_sub, &ros_ctx.pump_msg, &pump_callback, ON_NEW_DATA);
    rclc_executor_add_service(&ros_ctx.executor, &ros_ctx.valve_srv, &ros_ctx.valve_req, &ros_ctx.valve_res, &valve_service_callback);
    
    log_i("Setup Complete.");
}

void loop() {
    rclc_executor_spin_some(&ros_ctx.executor, RCL_MS_TO_NS(100));
    delay(10);
}
/**
 * @file main.cpp
 * @brief Main entry point for Firmware.
 */

#include <Arduino.h> // Or esp_system.h if pure IDF
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ros_context.h"

// Drivers
#include "components/pump.cpp" // Ideally include header, but including cpp for single-file context
#include "components/valves.cpp"
#include "components/flow_sensor.cpp"
#include "components/linear_actuator.cpp"

// ROS Setup Functions
extern void setup_subscribers();
extern void setup_publishers();
extern void setup_services();
extern void pump_callback(const void * msgin);
extern void valve_service_callback(const void * req, void * res);
extern void timer_callback(rcl_timer_t * timer, int64_t last_call_time);

RosContext ros_ctx;

extern "C" void app_main() {
    // 1. Hardware Init
    pump_driver.init();
    valve_driver.init();
    actuator_driver.init();
    flow_driver.init();

    // 2. Transport Init (WiFi/Serial)
    // setup_ros_transport(); // Handled by library

    // 3. Micro-ROS Init
    rcl_allocator_t allocator = rcl_get_default_allocator();
    rclc_support_init(&ros_ctx.support, 0, NULL, &allocator);

    rclc_node_init_default(&ros_ctx.node, "sprayer_node", "", &ros_ctx.support);

    // 4. Create Entities
    setup_publishers();
    setup_subscribers();
    setup_services();
    
    // 5. Executor
    rclc_executor_init(&ros_ctx.executor, &ros_ctx.support.context, 4, &allocator); // 4 handles
    rclc_executor_add_timer(&ros_ctx.executor, &ros_ctx.timer);
    rclc_executor_add_subscription(&ros_ctx.executor, &ros_ctx.pump_sub, &ros_ctx.pump_msg, &pump_callback, ON_NEW_DATA);
    rclc_executor_add_service(&ros_ctx.executor, &ros_ctx.valve_srv, &ros_ctx.valve_req, &ros_ctx.valve_res, &valve_service_callback);

    // 6. Main Loop
    while (1) {
        rclc_executor_spin_some(&ros_ctx.executor, RCL_MS_TO_NS(10));
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
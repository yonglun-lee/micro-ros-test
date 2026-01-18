/**
 * @file subscriber.cpp
 * @brief ROS Subscriber implementations.
 */

#include "ros_context.h"
#include "esp_log.h"

// Import global pump driver
extern class Pump pump_driver;

// Callback for Pump PWM
void pump_callback(const void * msgin) {
    const std_msgs__msg__Float32 * msg = (const std_msgs__msg__Float32 *)msgin;
    ESP_LOGI("ROS", "Pump Speed: %.2f", msg->data);
    pump_driver.set_effort(msg->data);
}

void setup_subscribers() {
    // Initialize subscription
    rcl_subscription_options_t opts = rcl_subscription_get_default_options();
    
    rclc_subscription_init_default(
        &ros_ctx.pump_sub,
        &ros_ctx.node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
        "cmd_pump",
        &opts
    );

    // Add to executor in main.cpp
}
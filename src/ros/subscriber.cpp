/**
 * @file subscriber.cpp
 * @brief Handles incoming ROS 2 messages.
 */

#include <Arduino.h>
#include "ros_context.h"
#include "pump.h"

extern Pump pump_driver;

void pump_callback(const void * msgin) {
    const std_msgs__msg__Float32 * msg = (const std_msgs__msg__Float32 *)msgin;
    log_i("ROS 2 Command: Set Pump Effort to %.2f", msg->data);
    pump_driver.set_effort(msg->data);
}

void setup_subscribers() {
    // FIX: Removed 'opts' and '&opts' to match the function signature
    // rclc_subscription_init_default(sub, node, type, topic)
    
    rcl_ret_t rc = rclc_subscription_init_default(
        &ros_ctx.pump_sub,
        &ros_ctx.node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
        "cmd_pump"
    );

    if (rc != RCL_RET_OK) {
        log_e("Failed to create pump subscriber");
    } else {
        log_d("Pump subscriber created");
    }
}
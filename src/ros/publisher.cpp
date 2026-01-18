/**
 * @file publisher.cpp
 * @brief Handles publishing of sensor data.
 */

#include "ros_context.h"
#include "flow_sensor.h"
#include <Arduino.h>

extern FlowSensor flow_driver;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
    if (timer != NULL) {
        float flow = flow_driver.read_flow_rate();
        // Only publish valid readings
        if(flow >= 0) {
            ros_ctx.flow_msg.data = flow;
            
            // FIX: Capture return value to resolve "warn_unused_result"
            rcl_ret_t rc = rcl_publish(&ros_ctx.flow_pub, &ros_ctx.flow_msg, NULL);
            
            if (rc != RCL_RET_OK) {
                log_e("Failed to publish flow rate");
            }
        }
    }
}

void setup_publishers() {
    rclc_publisher_init_default(
        &ros_ctx.flow_pub,
        &ros_ctx.node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
        "flow_rate"
    );

    rclc_timer_init_default(
        &ros_ctx.timer,
        &ros_ctx.support,
        RCL_MS_TO_NS(100), // 10Hz
        timer_callback
    );
}
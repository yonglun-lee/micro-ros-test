/**
 * @file publisher.cpp
 * @brief ROS Publisher implementations.
 */

#include "ros_context.h"

extern class FlowSensor flow_driver;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
    if (timer != NULL) {
        float flow = flow_driver.read_flow_rate();
        if(flow >= 0) {
            ros_ctx.flow_msg.data = flow;
            rcl_publish(&ros_ctx.flow_pub, &ros_ctx.flow_msg, NULL);
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

    // Timer setup
    rclc_timer_init_default(
        &ros_ctx.timer,
        &ros_ctx.support,
        RCL_MS_TO_NS(100), // 10Hz
        timer_callback
    );
}
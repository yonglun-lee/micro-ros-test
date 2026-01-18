/**
 * @file ros_context.h
 * @brief Context structure for micro-ROS entities.
 */

#pragma once

#include <Arduino.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

// Standard Message Types
#include <std_msgs/msg/float32.h>
#include <std_msgs/msg/bool.h>
#include <std_srvs/srv/set_bool.h>

struct RosContext {
    rcl_allocator_t allocator;
    rclc_support_t support;
    rcl_node_t node;
    rclc_executor_t executor;
    rcl_timer_t timer;
    
    // Publishers
    rcl_publisher_t flow_pub;
    std_msgs__msg__Float32 flow_msg;

    // Subscribers
    rcl_subscription_t pump_sub;
    std_msgs__msg__Float32 pump_msg;

    // Services
    rcl_service_t valve_srv;
    std_srvs__srv__SetBool_Request valve_req;
    std_srvs__srv__SetBool_Response valve_res;
};

// Global instance declaration
extern RosContext ros_ctx;
/**
 * @file service.cpp
 * @brief ROS Service implementations.
 */

#include "ros_context.h"
#include "esp_log.h"

extern class ValveManager valve_driver;

void valve_service_callback(const void * req, void * res) {
    std_srvs__srv__SetBool_Request * req_in = (std_srvs__srv__SetBool_Request *) req;
    std_srvs__srv__SetBool_Response * res_in = (std_srvs__srv__SetBool_Response *) res;

    // Logic: True = Open Valve 1, False = Close Valve 1 (Simplification)
    // For multiple valves, you might use a custom message or index logic.
    valve_driver.set_valve(1, req_in->data); 
    
    res_in->success = true;
    res_in->message.data = (char*)"Valve Toggled";
    res_in->message.size = 13;
}

void setup_services() {
    rclc_service_init_default(
        &ros_ctx.valve_srv,
        &ros_ctx.node,
        ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs, srv, SetBool),
        "set_valve"
    );
}
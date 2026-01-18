#include "ros_context.h"
#include "valves.h"
#include <Arduino.h>

extern ValveManager valve_driver;

void valve_service_callback(const void * req, void * res) {
    std_srvs__srv__SetBool_Request * req_in = (std_srvs__srv__SetBool_Request *) req;
    std_srvs__srv__SetBool_Response * res_in = (std_srvs__srv__SetBool_Response *) res;

    bool open_cmd = req_in->data;
    log_i("Service Request: Valve 1 -> %s", open_cmd ? "OPEN" : "CLOSE");
    
    valve_driver.set_valve(1, open_cmd); 
    
    res_in->success = true;
    // Note: Strings in micro-ROS must be handled carefully regarding memory
    // For simplicity, we assume the response buffer is large enough or use static string
    const char* msg = "Valve Toggled";
    // Usually you need to allocate string memory if it's dynamic, 
    // but SetBool uses string capacity defined in .rosidl
    // Assuming default init handling.
}

void setup_services() {
    rclc_service_init_default(
        &ros_ctx.valve_srv,
        &ros_ctx.node,
        ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs, srv, SetBool),
        "set_valve"
    );
}
#include "pump.h"
#include "pins.h"
#include "config.h"

// Note: Arduino ESP32 v3.0+ uses ledcAttach; older versions use ledcSetup/ledcAttachPin.
// Using v2.x compatible syntax for broader compatibility.

Pump::Pump() : is_enabled(false), current_duty(0.0f) {}

void Pump::init() {
    // Channel 0, Freq, 13-bit resolution
    ledcSetup(0, PUMP_PWM_FREQ_HZ, 13);
    ledcAttachPin(PIN_PUMP_PWM, 0);
    stop();
    log_d("Pump Initialized on Pin %d", PIN_PUMP_PWM);
}

void Pump::set_effort(float effort) {
    if (effort < 0.0f) effort = 0.0f;
    if (effort > 1.0f) effort = 1.0f;
    current_duty = effort;
    
    // 13-bit = 8191 max
    uint32_t duty = (uint32_t)(effort * 8191.0f); 
    ledcWrite(0, duty);
    log_v("Pump set to %.2f (Duty: %d)", effort, duty);
}

void Pump::stop() {
    set_effort(0.0f);
}
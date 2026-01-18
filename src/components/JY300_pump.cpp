/**
 * @file pump.cpp
 * @brief Driver for JY300 Water Pump (PWM).
 */

#include "driver/ledc.h"
#include "pins.h"
#include "config.h"
#include <math.h>

class Pump {
private:
    ledc_channel_config_t ledc_channel;
    ledc_timer_config_t ledc_timer;
    bool is_enabled;
    float current_duty; // 0.0 to 1.0

public:
    Pump() : is_enabled(false), current_duty(0.0f) {}

    void init() {
        // Configure Timer
        ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
        ledc_timer.timer_num = LEDC_TIMER_0;
        ledc_timer.duty_resolution = LEDC_TIMER_13_BIT;
        ledc_timer.freq_hz = PUMP_PWM_FREQ_HZ;
        ledc_timer.clk_cfg = LEDC_AUTO_CLK;
        ledc_timer_config(&ledc_timer);

        // Configure Channel
        ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
        ledc_channel.channel = LEDC_CHANNEL_0;
        ledc_channel.timer_sel = LEDC_TIMER_0;
        ledc_channel.intr_type = LEDC_INTR_DISABLE;
        ledc_channel.gpio_num = PIN_PUMP_PWM;
        ledc_channel.duty = 0;
        ledc_channel.hpoint = 0;
        ledc_channel_config(&ledc_channel);
    }

    /**
     * @brief Set pump effort
     * @param effort 0.0 to 1.0
     */
    void set_effort(float effort) {
        if (effort < 0.0f) effort = 0.0f;
        if (effort > 1.0f) effort = 1.0f;
        current_duty = effort;
        
        uint32_t duty = (uint32_t)(effort * 8191.0f); // 13-bit resolution
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    }

    void stop() {
        set_effort(0.0f);
    }
};

// Global instance
Pump pump_driver;
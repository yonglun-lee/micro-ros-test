/**
 * @file linear_actuator.cpp
 * @brief Driver for Linear Actuator with Potentiometer feedback.
 */

#include "driver/ledc.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "pins.h"
#include "config.h"

class LinearActuator {
private:
    ledc_channel_config_t ledc_channel;
    ledc_timer_config_t ledc_timer;
    
public:
    void init() {
        // 1. Setup Motor PWM (MD1)
        ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
        ledc_timer.timer_num = LEDC_TIMER_1;
        ledc_timer.duty_resolution = LEDC_TIMER_13_BIT;
        ledc_timer.freq_hz = ACTUATOR_PWM_FREQ_HZ;
        ledc_timer.clk_cfg = LEDC_AUTO_CLK;
        ledc_timer_config(&ledc_timer);

        ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
        ledc_channel.channel = LEDC_CHANNEL_1;
        ledc_channel.timer_sel = LEDC_TIMER_1;
        ledc_channel.gpio_num = PIN_ACTUATOR_PWM;
        ledc_channel.duty = 0;
        ledc_channel.hpoint = 0;
        ledc_channel_config(&ledc_channel);

        // 2. Setup Direction Pin
        gpio_set_direction(PIN_ACTUATOR_DIR, GPIO_MODE_OUTPUT);

        // 3. Setup ADC (Potentiometer)
        adc1_config_width(ADC_WIDTH_BIT_12);
        // Note: PIN_ACTUATOR_POT is GPIO34 which is ADC1_CHANNEL_6
        adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    }

    /**
     * @brief Set motor speed and direction
     * @param speed -100 to 100
     */
    void drive(int speed) {
        if (speed > 100) speed = 100;
        if (speed < -100) speed = -100;

        // Set Direction (Assume Positive = Extend)
        gpio_set_level(PIN_ACTUATOR_DIR, (speed >= 0) ? 1 : 0);

        uint32_t duty = (abs(speed) * 8191) / 100;
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    }

    /**
     * @brief Get current raw position
     * @return 0-4095
     */
    int get_position_raw() {
        return adc1_get_raw(ADC1_CHANNEL_6);
    }
};

LinearActuator actuator_driver;
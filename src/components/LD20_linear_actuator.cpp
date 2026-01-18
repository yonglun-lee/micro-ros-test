#include "linear_actuator.h"
#include "pins.h"
#include "config.h"

// Timer 1, Channel 1
#define ACTUATOR_PWM_CH 1 

void LinearActuator::init() {
    // 1. Setup Motor PWM
    ledcSetup(ACTUATOR_PWM_CH, ACTUATOR_PWM_FREQ_HZ, 13);
    ledcAttachPin(PIN_ACTUATOR_PWM, ACTUATOR_PWM_CH);

    // 2. Setup Direction Pin
    pinMode(PIN_ACTUATOR_DIR, OUTPUT);
    digitalWrite(PIN_ACTUATOR_DIR, LOW);

    // 3. Setup Potentiometer
    // Arduino handles ADC config automatically on analogRead
    pinMode(PIN_ACTUATOR_POT, INPUT); 
    
    log_d("Linear Actuator Initialized");
}

void LinearActuator::drive(int speed) {
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;

    // Direction Logic: Positive = Extend (HIGH), Negative = Retract (LOW)
    digitalWrite(PIN_ACTUATOR_DIR, (speed >= 0) ? HIGH : LOW);

    uint32_t duty = (abs(speed) * 8191) / 100;
    ledcWrite(ACTUATOR_PWM_CH, duty);
}

int LinearActuator::get_position_raw() {
    return analogRead(PIN_ACTUATOR_POT);
}
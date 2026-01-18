#include "flow_sensor.h"
#include "pins.h"
#include "config.h"

#define K24_ADDR 0x01
#define CMD_READ 0x03

void FlowSensor::init() {
    // Initialize Serial2 for RS485
    // RX=RO, TX=DI
    Serial2.begin(FLOW_SENSOR_BAUD, SERIAL_8N1, PIN_RS485_RO, PIN_RS485_DI);
    
    // DE/RE Control Pin
    pinMode(PIN_RS485_TX, OUTPUT);
    digitalWrite(PIN_RS485_TX, LOW); // Receive mode default
    
    log_d("Flow Sensor UART Initialized");
}

float FlowSensor::read_flow_rate() {
    uint8_t cmd[] = {K24_ADDR, CMD_READ, 0x00, 0x17, 0x00, 0x02, 0x00, 0x00}; 
    uint16_t crc = calculate_crc(cmd, 6);
    cmd[6] = crc & 0xFF;
    cmd[7] = (crc >> 8) & 0xFF;

    // Flush Input
    while(Serial2.available()) Serial2.read();

    // Enable TX
    digitalWrite(PIN_RS485_TX, HIGH);
    Serial2.write(cmd, 8);
    Serial2.flush(); // Wait for TX to finish
    // Disable TX (Enable RX)
    digitalWrite(PIN_RS485_TX, LOW);

    // Wait for response
    uint8_t data[10];
    size_t len = Serial2.readBytes(data, 9);
    
    if (len > 0 && data[1] == CMD_READ) {
        uint32_t val = (data[3] << 24) | (data[4] << 16) | (data[5] << 8) | data[6];
        float flow = (float)val / 100.0f; // Assuming scaling
        log_v("Flow Rate: %.2f L/min", flow);
        return flow;
    } else {
        log_e("Flow Sensor Read Failed or Timeout");
    }
    return -1.0f;
}

uint16_t FlowSensor::calculate_crc(uint8_t *buffer, uint16_t length) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= buffer[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) crc = (crc >> 1) ^ 0xA001;
            else crc >>= 1;
        }
    }
    return crc;
}
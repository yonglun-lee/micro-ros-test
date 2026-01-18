/**
 * @file flow_sensor.cpp
 * @brief RS485 Comm for K24 Flow Sensor.
 */

#include "driver/uart.h"
#include "pins.h"
#include "config.h"
#include <cstring>

#define K24_ADDR 0x01
#define CMD_READ 0x03

class FlowSensor {
public:
    void init() {
        uart_config_t uart_config = {
            .baud_rate = FLOW_SENSOR_BAUD,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_APB,
        };
        uart_driver_install(FLOW_SENSOR_UART, 256, 0, 0, NULL, 0);
        uart_param_config(FLOW_SENSOR_UART, &uart_config);
        
        // RS485 Half Duplex Mode
        uart_set_pin(FLOW_SENSOR_UART, PIN_RS485_DI, PIN_RS485_RO, PIN_RS485_TX, UART_PIN_NO_CHANGE);
        uart_set_mode(FLOW_SENSOR_UART, UART_MODE_RS485_HALF_DUPLEX);
    }

    /**
     * @brief Reads flow rate in L/min
     */
    float read_flow_rate() {
        // Modbus command: Addr(1) Func(1) StartHigh(1) StartLow(1) CntHigh(1) CntLow(1) CRC(2)
        // Reg 0x0017 is Flow Rate
        uint8_t cmd[] = {K24_ADDR, CMD_READ, 0x00, 0x17, 0x00, 0x02, 0x00, 0x00}; 
        // Note: CRC Calculation omitted for brevity, reusing the logic from your provided file
        // Hardcoded CRC for example if constant, but dynamic calc needed in production
        uint16_t crc = calculate_crc(cmd, 6);
        cmd[6] = crc & 0xFF;
        cmd[7] = (crc >> 8) & 0xFF;

        uart_write_bytes(FLOW_SENSOR_UART, (const char*)cmd, 8);
        
        uint8_t data[10];
        int len = uart_read_bytes(FLOW_SENSOR_UART, data, 9, pdMS_TO_TICKS(100));
        
        if (len > 0 && data[1] == CMD_READ) {
            // Parse float/int from bytes 3-6
            uint32_t val = (data[3] << 24) | (data[4] << 16) | (data[5] << 8) | data[6];
            // K24 sends int scaled by 100 usually
            return (float)val / 100.0f;
        }
        return -1.0f; // Error
    }

private:
    uint16_t calculate_crc(uint8_t *buffer, uint16_t length) {
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
};

FlowSensor flow_driver;
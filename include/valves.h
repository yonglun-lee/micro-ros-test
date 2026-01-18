/**
 * @file valves.h
 * @brief Header for CWX-15Q Valve Driver.
 */
#pragma once
#include <Arduino.h>

class ValveManager {
public:
    /**
     * @brief Initialize valve GPIO pins.
     */
    void init();

    /**
     * @brief Open or Close a specific valve.
     * @param id Valve ID (1, 2, or 3).
     * @param open True to open, False to close.
     */
    void set_valve(int id, bool open);

    /**
     * @brief Close all valves.
     */
    void close_all();
};
/** @file atm_sen_module.h
 * @brief Atmospheric sensors module interface.
 * 
 * This file handles the configuration of all atmospheric sensors,
 * through I2C, SPI and UART. It provides functions to initialize
 * and conduct readings from all the sensors, saving them to a 
 * dedicated structure.
 */

#ifndef ATM_SEN_MODULE_H
#define ATM_SEN_MODULE_H

#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>

// DATA STURTURES

/** @brief 
 * 
 */

typedef struct 
{
    char timestamp_str[20]; // Time since boot in milliseconds
    double pressure_pa;
    double altitude_m;
    float temperature_c;
    float humidity_pct;
    float methane_ppm;
    float ammonia_ppm;
    float oxygen_pct;
} sensor_readings_t;

extern void init_all_sensors(void);
extern void read_all(sensor_readings_t *gathered_data);

#endif
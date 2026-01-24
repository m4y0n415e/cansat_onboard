/** @file atm_sen_module.h
 * @brief Atmospheric sensors module interface.
 * 
 * This file handles the configuration of all atmospheric sensors,
 * through I2C, SPI and UART. It provides functions to initialize
 * and conduct readings from all the sensors, saving them to a 
 * dedicated 'sensor_readings_t' structure.
 */

#ifndef ATM_SEN_MODULE_H
#define ATM_SEN_MODULE_H

#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>

// DATA STURTURES

/** @brief Structure for keeping all data read from the atmospheric sensors.
 */

typedef struct 
{
    double pressure_pa; /// < Pressure reading in Pascals
    double altitude_m; /// < Altitude in meters
    float temperature_c; /// < Temperature in Celsius degrees
    float humidity_pct; /// < Humidity percentage
    float methane_ppm; /// < Methane in particles per million
    float ammonia_ppm; /// < Ammonia in particles per million
    float oxygen_pct; /// < Oxygen percentage
} sensor_readings_t;

// FUNCTIONS

/** @brief Initializes all sensors with their I2C, SPI or UART connection 
 * (depending on which is necessary). Within the function, for every sensor that
 * needs initialization a dedicated function is called.
 * This lays the grounds for starting continous readings.
 * @note This must be called once at system startup before the main loop.
 */

extern void init_all_sensors(void);

/** @brief Begins readings from all the atmospheric sensors.
 * Within the function, for every sensor a dedicated _read function is called.
 * The data read is then immediately saved in a dedicated structure.
 * 
 * @param[out] gathered_data Pointer to the an object of a 'sensor_readings_t' structure,
 *  to which data read from sensors is immediatly saved to.
 */
extern void read_all(sensor_readings_t *gathered_data);

#endif // ATM_SEN_MODULE_H
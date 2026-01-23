/** @file microsd_module.h
 *  @brief Micro SD Card reader interface.
 * 
 * This file handles the SD card reader SPI configuration for Pico using the 
 * ... library, implements a function for saving data 
 * collected from other sensors, and saving GPS coordinates processed by the GPS module.
 * 
 * @see ... for the SD driver documentation.
 */

#ifndef MICROSD_MODULE_H
#define MICROSD_MODULE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "atm_sen_module.h"
#include "time_manager.h"
#include "gps_module.h"

// FUNCTIONS

/** @brief  Initializes the Micro SD reader SPI connection and 
 * checks for successful mounting of the card.
 * 
 * @return true if the reader is correctly initialized and the microSD card is mounted.
 * @return false if the reader setup or the microSD card mount fails.
 */

extern bool sd_init();

/** @brief Saves all data collected from sensors onto the microSD card in a data_log.txt file.
 * 
 * It uses the functions available in the beforementioned library to save formatted strings with
 * values read from sensors kept in a sensor_readings_t file onto the microSD.
 * 
 * @param[in] data Pointer to a sensor_readings_t data struct which stores the data to
 * to be saved on the microSD card.\
 * @param[in] time Pointer to a time-keeping structure that plays a role in time management
 * for the timestamps in the data_log.txt file.
 */
extern void save_system_data(sensor_readings_t *data, current_time_t *time);

//extern void save_photo(current_time_t* state, uint8_t* image_buffer, uint32_t length);

/** @brief Saves all GPS data onto the microSD card in a gps_log.csv file.
 * 
 * It uses the functions available in the beforementioned library to save formatted strings with
 * values read from the GPS kept in a gps_data_t file onto the microSD.
 * 
 * @param[in] gps_data_t Pointer to a structure keeping data read from GPS and properly parsed.
 */
extern void save_gps_log(gps_data_t *gps);

#endif // MICROSD_MODULE_H
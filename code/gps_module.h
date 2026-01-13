/** @file gps_module.h
 * @brief  GPS module interface.
 *
 * This file handles the UART configuration for Pico for the GPS module,
 * defines the data structure for holding parsed GPS coordinates/time,
 * and provides functions to initialize and update the system.
 *
 * @see https://github.com/kosma/minmea for the NMEA parsing library documentation.
 */

#ifndef GPS_MODULE_H
#define GPS_MODULE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"

// CONFIGURATION MACROS

/** @brief UART Baud rate for the GPS module (Standard is usually 9600). */
#define GPS_BAUD_RATE 9600

/** @brief The hardware UART instance to use (uart0 or uart1). */
#define GPS_UART_ID uart0

/** @brief GPIO pin for UART transmission (Pico TX). */
#define GPS_TX_PIN 8

/** @brief GPIO pin for UART reception (Pico RX). */
#define GPS_RX_PIN 9

// DATA STRUCTURES

/** @brief The structure keeps the parsed GPS position and time information.
 * 
 * It is updated whenever a valid NMEA senstence ($GPRMC in this case) 
 * is succesfully parsed.
 */
typedef struct 
{
    float latitude; /// < Latitude in decimal degrees.
    float longitude; /// < Longitude in decimal degrees.
    float altitude; /// < Altitude above mean sea level in meters [m].
    uint8_t satellites; /// < Number of satellites currently in view.
    uint8_t hour; /// < UTC Hour
    uint8_t min; /// < UTC Minute
    uint8_t sec; /// < UTC Second
    bool fix; /// < True if a valid GPS fix is currently available.
} gps_data_t;

// FUNCTIONS

/**
 * @brief Initializes the GPS UART connection and GPIO pins.
 *
 * Sets up the specified GPS_UART_ID with the baud rate defined in
 * GPS_BAUD_RATE and configures the TX/RX pins.
 */

extern void gps_init(void);

/**
 * @brief Polls the UART for newly aquired GPS data and sends it to be parsed.
 *
 * This function should be called frequently (e.g., in the main loop).
 * It reads available characters from the UART buffer and calls a function to process
 * NMEA sentences.
 *
 * @return true if a valid packet was fully parsed and data was updated.
 * @return false if no new complete packet is available yet.
 */
extern bool gps_update(void);

/**
 * @brief Retrieves the latest parsed GPS data.
 *
 * Copies the most recent valid GPS data into the provided gos_data_t structure.
 *
 * @param[out] data Pointer to a gps_data_t struct where the data will be copied.
 */
extern void gps_get_data(gps_data_t *data);
#endif // GPS_MODULE_H

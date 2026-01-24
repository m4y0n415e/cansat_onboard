/** @file radio_module.h 
 * @brief High-level telemetry interface for nRF905 radio.
 * 
 * @details Abstracts low-level SPI drivers to provide a simple API for sending 
 * sensor data. Handles the formatting of float values into ASCII telemetry strings 
 * (e.g., "T:25.0 P:1013") and manages packet transmission.
 */

#ifndef RADIO_MODULE_H
#define RADIO_MODULE_H

#include "pico/stdlib.h"

/** @brief Initializes the radio hardware and driver.
 * 
 * @details
 * This function calls the underlying driver initialization routine. It sets up 
 * the SPI interface, configures GPIO pins for radio control (TX_EN, TRX_CE, PWR), 
 * and writes the default configuration registers (Frequency, Power, CRC).
 * 
 * @note Must be called once at system startup before attempting any transmissions.
 */
extern void radio_module_init(void);

/** @brief Formats sensor data and broadcasts it via radio.
 * 
 * @details
 * This function takes individual sensor readings, formats them into a standard 
 * ASCII telemetry string (e.g., "T:24.5 P:1001 A:150"), and broadcasts the 
 * packet via the nRF905 radio.
 * 
 * @param temp  Temperature (Celsius)
 * @param press Pressure (Pascals)
 * @param alt   Altitude (Meters)
 */
extern void radio_module_send_telemetry(float temp, float press, float alt);

#endif
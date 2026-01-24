/** @file time_manager.h
 * 
 * @brief Software RTC with GPS synchronization and FatFS integration.
 * 
 * @details Maintains system time using system ticks (`to_ms_since_boot`), independent 
 * of hardware RTC. 
 * It features:
 * - a sync with GPS: corrects drift and sets time/date via NMEA data.
 * - a FatFS backend: provides timestamps for SD card files.
 * - timezones: applies static offsets defined by `TIMEZONE_OFFSET`.
 * * Usage: Calling `time_manager_init()` at startup, `time_manager_update()` 
 * periodically in the loop, and `time_manager_sync()` when valid GPS data exists.
 * * @note Replaces the default `rtc.c` to remove hardware dependencies I had truble with.
 */

#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// DATA STRUCTURES
typedef struct 
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint16_t photo_count;
} current_time_t;

// FUNCTIONS

/** @brief Initializes the Software Real-Time Clock (RTC).
 * 
 * Sets the internal system time to a default start date (January 1, 2026).
 * It captures the current processor boot time (`to_ms_since_boot`) to establish
 * a baseline for the 1-second tick counter.
 * 
 * * @note This must be called once at system startup before the main loop.
 */
extern void time_manager_init(void);

/** @brief Ticks the internal clock forward.
 * 
 * This function checks the system's millisecond timer. If 1000ms have passed
 * since the last tick, it increments the internal `raw_seconds` counter
 * and updates the reference timer.
 * 
 * * @details This function is non-blocking and is designed to be called frequently 
 * inside the main `while(1)` loop.
 * 
 * * @return true If a full second has passed during this call (useful for triggering 1Hz events like LED blinks).
 * @return false If less than 1 second has passed since the last update.
 */
extern bool time_manager_update(void);

/** @brief Retrieves the current system time in a human-readable format.
 * 
 * Converts the internal epoch timestamp (`raw_seconds`) into a `current_time_t` structure
 * containing year, month, day, hour, minute, and second.
 * 
 * * @details If the time is exactly 00:00:00 (Midnight), this function automatically 
 * resets the `photo_count` field to 0.
 * 
 * * @return current_time_t* Pointer to the structure holding the current time.
 */
extern current_time_t* time_manager_get(void);

/** @brief Synchronizes the internal clock with an external source (e.g., GPS).
 * * Overwrites the internal time counter with new values provided by the GPS.
 * 
 * * @details 
 * - Automatically applies the defined `TIMEZONE_OFFSET` to the provided UTC hour.
 * - Uses `mktime()` to safe-handle hour overflows. For example, if the 
 * GPS says 23:00 UTC and your offset is +2, the date automatically moves on to the next day.
 * - Resets the millisecond counter (`last_second_ms`) to zero, ensuring 
 * the next software "tick" happens exactly 1 second after this sync occurs.
 * 
 * * @param[in] year  Full year (e.g., 2026).
 * @param[in] month Month number (1-12).
 * @param[in] day   Day of the month (1-31).
 * @param[in] hour  UTC Hour (0-23).
 * @param[in] min   Minute (0-59).
 * @param[in] sec   Second (0-59).
 */
extern void time_manager_sync(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);

#endif
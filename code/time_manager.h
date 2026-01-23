/** @file time_manager.h
 * @brief A module dedicated to 
 * 
 */

#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

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

extern void time_manager_init(void);

extern bool time_manager_update(void);

extern void time_manager_sync(uint8_t hour, uint8_t min, uint8_t sec);

extern current_time_t* time_manager_get(void);

#endif
#include "time_manager.h"
#include "pico/stdlib.h"
#include "ff.h"
#include "microsd_module.h"
#include <stdio.h>

#define TIMEZONE_OFFSET 1

// This is a standard counter that holds the total seconds elapsed since a specific starting point (January 1, 2026, as in the init function).
static time_t raw_seconds = 0;

static current_time_t system_time;

// This tracks the millisecond timer from the processor to detect when exactly 1 second has passed.
static uint32_t last_second_ms = 0; 

void time_manager_init(void)
{
    struct tm t = {0};
    t.tm_year = 2026 - 1900;
    t.tm_mon  = 0;
    t.tm_mday = 1;
    t.tm_hour = 0;
    t.tm_min  = 0;
    t.tm_sec  = 0;
    
    raw_seconds = mktime(&t);
    
    last_second_ms = to_ms_since_boot(get_absolute_time());
}

bool time_manager_update(void)
{
    uint32_t now = to_ms_since_boot(get_absolute_time());
    
    if (now - last_second_ms >= 1000) 
    {
        last_second_ms += 1000;
        
        raw_seconds++;
        
        return true; 
    }
    return false;
}

current_time_t *time_manager_get(void)
{
    struct tm *t_info;
    
    t_info = gmtime(&raw_seconds);
    
    system_time.year  = t_info->tm_year + 1900;
    system_time.month = t_info->tm_mon + 1;
    system_time.day   = t_info->tm_mday;
    system_time.hour  = t_info->tm_hour;
    system_time.min   = t_info->tm_min;
    system_time.sec   = t_info->tm_sec;
    
    if (system_time.hour == 0 && system_time.min == 0 && system_time.sec == 0) 
    {
        system_time.photo_count = 0;
    }

    return &system_time;
}

void time_manager_sync(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    struct tm t = {0};

    t.tm_year = year - 1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    t.tm_min  = min;
    t.tm_sec  = sec;

    t.tm_hour = hour + TIMEZONE_OFFSET; 

    raw_seconds = mktime(&t);
    last_second_ms = to_ms_since_boot(get_absolute_time());
}

DWORD get_fattime(void)
{
    current_time_t *t = time_manager_get();

    DWORD fattime = 0;

    // 32-bit format expected by FatFS
    // Year:  Bits 31-25 (Offset from 1980)
    fattime |= (DWORD)(t->year - 1980) << 25;
    
    // Month: Bits 24-21 (1-12)
    fattime |= (DWORD)(t->month) << 21;
    
    // Day:   Bits 20-16 (1-31)
    fattime |= (DWORD)(t->day) << 16;
    
    // Hour:  Bits 15-11 (0-23)
    fattime |= (DWORD)(t->hour) << 11;
    
    // Min:   Bits 10-5  (0-59)
    fattime |= (DWORD)(t->min) << 5;
    
    // Sec:   Bits 4-0   (0-29, in 2-second intervals)
    fattime |= (DWORD)(t->sec / 2);

    return fattime;
}
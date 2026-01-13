#include "time_manager.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define TIMEZONE_OFFSET 1

static time_t raw_seconds = 0;

static current_time_t system_time;

// Stopwatch for the 1-second tick
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

void time_manager_sync(uint8_t hour, uint8_t min, uint8_t sec)
{
    struct tm *t_info = gmtime(&raw_seconds);
    
    int h = hour + TIMEZONE_OFFSET;
    if (h >= 24) h -= 24;

    t_info->tm_hour = h;
    t_info->tm_min  = min;
    t_info->tm_sec  = sec;
    
    raw_seconds = mktime(t_info);

    last_second_ms = to_ms_since_boot(get_absolute_time());
}
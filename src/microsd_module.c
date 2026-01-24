#include "microsd_module.h"
#include "debug_mode.h"
#include "hardware/spi.h"
#include "ff.h"
#include "sd_card.h"
#include <stdio.h>

extern sd_card_t *sd_get_by_num(size_t num);

bool sd_init() 
{
    sd_card_t *pSD = sd_get_by_num(0);

    if (!pSD) 
    {
        LOG("[SD] ERROR: Could not get SD card object.\n");
        return false;
    }

    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    
    if (fr == FR_OK) 
    {
        LOG("[SD] Mount Success!\n");
        return true;
    } else 
    {
        LOG("[SD] ERROR: Mount failed with code: %d\n", fr);
        return false;
    }
}

void save_system_data(sensor_readings_t* data, current_time_t* time) 
{
    if (!sd_init()) return;

    FIL fil;
    FRESULT fr;

    fr = f_open(&fil, "data_log.txt", FA_WRITE | FA_OPEN_APPEND);

    if (fr == FR_OK) 
    {
        f_printf(&fil, "[%02d:%02d:%02d] [BMP280] Pressure: %.2f Pa | Altitude: %.2f m\n", 
                 time->hour, time->min, time->sec, 
                 data->pressure_pa, data->altitude_m);

        f_printf(&fil, "[%02d:%02d:%02d] [SHTC3] Temperature: %.2f C, Humidity: %.2f %%\n", 
                 time->hour, time->min, time->sec, 
                 data->temperature_c, data->humidity_pct);

        f_printf(&fil, "[%02d:%02d:%02d] [GASES] CH4: %.2f ppm, NH3: %.2f ppm, O2: %.2f %%\n", 
                 time->hour, time->min, time->sec, 
                 data->methane_ppm, data->ammonia_ppm, data->oxygen_pct);
        
        f_printf(&fil, "--------------------------------------------------\n");

        f_close(&fil);
        LOG("[SD] System data saved.\n");
    } 
    else 
    {
        LOG("[SD] Failed to open data_log.txt (Error: %d)\n", fr);
    }
}

void save_gps_log(gps_data_t* gps)
{
    // if(!gps->fix) return;

    if (!sd_init()) return;

    FIL fil;
    FRESULT fr;

    fr = f_open(&fil, "gps_log.csv", FA_WRITE | FA_OPEN_APPEND);

    if (fr == FR_OK) 
    {
        if (f_size(&fil) == 0) f_printf(&fil, "Latitude,Longitude,Altitude,Satellites\n"); // If file is empty - write to .csv header

        f_printf(&fil, "%.6f,%.6f,%.2f,%d,%d\n", 
                 gps->latitude, 
                 gps->longitude, 
                 gps->altitude, 
                 gps->satellites, 
                 gps->fix ? 1 : 0);

        f_close(&fil);
        LOG("[SD] GPS log saved.\n");
    } 
    else 
    {
        LOG("[SD] Failed to open gps_log.csv (Error: %d)\n", fr);
    }
}
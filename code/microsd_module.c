#include "microsd_module.h"
#include "debug_mode.h"
#include "hardware/spi.h"

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
    }
    else 
    {
        LOG("[SD] ERROR: Mount failed: %d\n", fr);
        return false;
    }
}

void save_system_data(sensor_readings_t* data, current_time_t* time) 
{
    if (!sd_init()) return;

    FIL fil;
    FRESULT fr = f_open(&fil, "data_log.txt", FA_WRITE | FA_OPEN_APPEND);

    LOG("[SD] Starting data save to data_log.txt.\n");
    if (fr == FR_OK) 
    {
        f_printf(&fil, "%s: [BMP280] Presssure: %.2f hPa\n", data->timestamp_str, data->pressure_pa);
        f_printf(&fil, "%s: [BMP280] Altitude: %.2f m\n",  data->timestamp_str, data->altitude_m);
        
        f_printf(&fil, "%s: [SHTC3] Temperature: %.2f °C\n", data->timestamp_str, data->temperature_c);
        f_printf(&fil, "%s: [SHTC3] Humidity: %.2f %%\n",  data->timestamp_str, data->humidity_pct);
        
        f_printf(&fil, "%s: CH4: %.2f ppm\n",    data->timestamp_str, data->methane_ppm);
        f_printf(&fil, "%s: NH3: %.2f ppm\n",    data->timestamp_str, data->ammonia_ppm);
        f_printf(&fil, "%s: O2: %.2f pct\n",     data->timestamp_str, data->oxygen_pct);
        
        f_printf(&fil, "\n");

        f_close(&fil);
    } else LOG("[SD] Failed to open data_log.txt (%d)\n", fr);
}

// void save_photo(current_time_t* state, uint8_t* image_buffer, uint32_t length) 
// {
//     if (!sd_init()) return;
    
//     f_mkdir("photos");
    
//     char filename[64];
//     sprintf(filename, "photos/%04d%02d%02d-%03d.jpg", 
//             state->year, state->month, state->day, state->photo_count);
            
//     FIL fil;
//     FRESULT fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
//     if (fr == FR_OK) {
//         UINT bw;
//         f_write(&fil, image_buffer, length, &bw);
//         f_close(&fil);
//         printf("Saved Photo: %s (%d bytes)\n", filename, bw);
//     } else {
//         printf("Photo Save Error: %d\n", fr);
//     }
// }

void save_gps_log(gps_data_t* gps)
{
    if (!sd_init()) return;

    FIL fil;
    FRESULT fr = f_open(&fil, "gps_log.csv", FA_WRITE | FA_OPEN_APPEND);

    LOG("[SD] Starting data save to gps_log.csv.\n");
    if (fr == FR_OK) 
    {   f_printf(&fil, "%.6f,%.6f,%.2f,%d,%d\n", 
                 gps->latitude, 
                 gps->longitude, 
                 gps->altitude, 
                 gps->satellites, 
                 gps->fix ? 1 : 0);
        f_close(&fil);
    } else LOG("[SD] Failed to open gps_log.csv (%d)\n", fr);
}
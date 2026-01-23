#include "microsd_module.h"
#include "debug_mode.h"
#include "hardware/spi.h"


bool sd_init() 
{
    
}

void save_system_data(sensor_readings_t* data, current_time_t* time) 
{
    
}

void save_photo(current_time_t* state, uint8_t* image_buffer, uint32_t length) 
{
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
}

void save_gps_log(gps_data_t* gps)
{

}
#include <stdio.h>
#include "pico/stdlib.h"
#include "debug_mode.h"
#include "atm_sen_module.h"
#include "microsd_module.h"
#include "time_manager.h"
#include "gps_module.h"
#include "radio_module.h"

int main(void)
{  
    stdio_init_all();

    sleep_ms(5000);
    LOG("[Main] System booting...\n");


    time_manager_init();
    LOG("[Main] Init GPS...\n");
    gps_init();
    LOG("[Main] GPS initialized, .\n");

    LOG("[Main] Init all sensors...\n");
    init_all_sensors();
    LOG("[Main] Sensors initialized\n");

    LOG("[Main] Init radio module...\n");
    radio_module_init();
    LOG("[Main] Radio communication initialized.\n");
    

    sensor_readings_t current_sensor_data = {};
    gps_data_t my_gps;


    LOG("[Main] Entering Loop:\n");
       
    while (1)
    {
        if (gps_update())
        {
            gps_get_data(&my_gps);
            if (my_gps.fix && my_gps.year > 0) 
            { 
                time_manager_sync(my_gps.year, my_gps.month, my_gps.day, my_gps.hour, my_gps.min, my_gps.sec);
            }
        }

        if(time_manager_update())
        {
            read_all(&current_sensor_data);

            LOG("Temp: %.2f C | Press: %.2f Pa | Alt: %.2f m | Hum: %.2f %% | O2: %.2f %% | CH4: %.2f ppm | NH3: %.2f\r\n ppm",
               current_sensor_data.temperature_c,
               current_sensor_data.pressure_pa,
               current_sensor_data.altitude_m,
               current_sensor_data.humidity_pct,
               current_sensor_data.oxygen_pct,
               current_sensor_data.methane_ppm,
               current_sensor_data.ammonia_ppm);

            sleep_ms(500);

            LOG("Logging data...\n");
            save_system_data(&current_sensor_data, time_manager_get());

            bool valid_fix = my_gps.fix && (my_gps.latitude != 0.0f);

            current_time_t* t = time_manager_get();
           
            LOG("[%02d:%02d:%02d] Temp: %.2f | GPS Fix: %s\n",
                t->hour, t->min, t->sec,
                current_sensor_data.temperature_c,
                valid_fix ? "YES" : "NO");


            LOG("%.6f,%.6f,%.2f m,%d,%d\n",
                        my_gps.latitude,
                        my_gps.longitude,
                        my_gps.altitude,
                        my_gps.satellites,
                        my_gps.fix ? 1 : 0);
        }
    }
}

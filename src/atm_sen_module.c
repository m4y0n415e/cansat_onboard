#include <math.h>
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "atm_sen_module.h"
#include "debug_mode.h"
#include "lib/bmp280/bmp280_i2c.h"
#include "lib/bmp280/bmp280_i2c_hal.h"
#include "dfrobot_oxygen.h"

#define I2C_PORT i2c0
#define SHTC3_ADDR 0x70
#define PIN_SDA 12
#define PIN_SCL 13
#define O2_ADDR  0x74

#define PIN_METHANE 26
#define PIN_AMMONIA 27

static float startup_pressure_pa = 0.0f;

static void bmp280_init()
{
    int err = BMP280_OK;
    uint8_t id = 0;

    LOG("[BMP280] Init...\n");

    bmp280_i2c_hal_init();

    err = bmp280_i2c_reset();
    if (err != BMP280_OK)
    {
        LOG("[BMP280] ERROR: Device reset failed\n");
    }

    err += bmp280_i2c_read_part_number(&id);
    if (err == BMP280_OK)
    {
        LOG("[BMP280] Part number: 0x%02X\n", id);
    }
    else
    {
        LOG("[BMP280] ERROR: Unable to read part number\n");
    }

    err += bmp280_i2c_set_calib();
    LOG("[BMP280] Calibration data: %s\n",
           err == BMP280_OK ? "OK" : "FAILED");

    err += bmp280_i2c_write_power_mode(POWERMODE_NORMAL);
    LOG("[BMP280] Normal mode: %s\n",
           err == BMP280_OK ? "OK" : "FAILED");

    err += bmp280_i2c_write_config_filter(FILTER_4);

    bmp280_ctrl_meas_t ctrl_meas = 
    {
        .osrs_press = OSRS_x4,
        .osrs_tmp   = OSRS_x1,
    };
    err += bmp280_i2c_write_osrs(ctrl_meas);

    if (err == BMP280_OK && id == 0x58) LOG("[BMP280] Initialization SUCCESS.\n");
    else LOG("[BMP280] ERROR: Initialization FAILED.\n");
}

void init_all_sensors()
{
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SDA);
    gpio_pull_up(PIN_SCL);

    adc_init();
    adc_gpio_init(PIN_METHANE);
    adc_gpio_init(PIN_AMMONIA);

    bmp280_init();
    oxygen_init();

    if (oxygen_init() == O2_OK) {
        LOG("[O2] Initialization SUCCESS.\n");
    } else {
        LOG("[O2] ERROR: Initialization FAILED.\n");
    }
}

static void bmp280_read(double *pressure, double *altitude)
{
    bmp280_data_t bmp280_dt;

    if (bmp280_i2c_read_data(&bmp280_dt) == BMP280_OK)
    {
        *pressure = (double)bmp280_dt.pressure / 256.0f;
        *altitude = 44330.0f * (1.0f - powf(*pressure / 101325.0f, 0.1903f));
    }
    else
    {
        *pressure = -1.0f; // error flag
        *altitude = -1.0f; // error flag
        LOG("[BMP280] ERROR: Read failed\n");
    }
}

static void shtc3_read(float *temp, float *hum)
{
    uint8_t cmd_wake[2]  = {0x35, 0x17};
    uint8_t cmd_meas[2]  = {0x78, 0x66};
    uint8_t cmd_sleep[2] = {0xB0, 0x98};
    uint8_t buffer[6];

    i2c_write_blocking(I2C_PORT, SHTC3_ADDR, cmd_wake, 2, false);
    sleep_us(250); // Small delay for wakeup

    i2c_write_blocking(I2C_PORT, SHTC3_ADDR, cmd_meas, 2, false);
    sleep_ms(20);  // Waiting for measurement to complete

    // Data - 6 bytes: Temp MSB, Temp LSB, CRC, Hum MSB, Hum LSB, CRC
    int ret = i2c_read_blocking(I2C_PORT, SHTC3_ADDR, buffer, 6, false);

    i2c_write_blocking(I2C_PORT, SHTC3_ADDR, cmd_sleep, 2, false);

    if (ret == PICO_ERROR_GENERIC) 
    {
        LOG("[SHTC3] ERROR: Read failed.\n");
        *temp = 0.0f;
        *hum = 0.0f;
        return;
    }

    // From documentation: 1. Combining bytes into integers
    // 2. Shifting the MSB 8 bits to the left and add the LSB
    uint16_t raw_temp = (buffer[0] << 8) | buffer[1];
    uint16_t raw_hum  = (buffer[3] << 8) | buffer[4];

    *temp = -45.0f + 175 * ((float)raw_temp / 65535.0f);
    *hum = 100.0f * ((float)raw_hum / 65535.0f);
}

static float mems_sensor_read(uint gpio_pin, float sensitivity_factor) 
{
    adc_select_input(gpio_pin - 26);

    uint16_t raw = adc_read();

    if (raw < 10) return -1.0f; // Error flag

    float voltage = (float)raw * 3.3f / 4095;

    float ppm = voltage * sensitivity_factor; 

    return ppm;
}

void read_all(sensor_readings_t* gathered_data) 
{
    shtc3_read(&gathered_data->temperature_c, &gathered_data->humidity_pct);

    gathered_data->methane_ppm = mems_sensor_read(PIN_METHANE, 100.0f);
    gathered_data->ammonia_ppm = mems_sensor_read(PIN_AMMONIA, 50.0f);
    
    bmp280_read(&gathered_data->pressure_pa,&gathered_data->altitude_m);
    
    float o2_val = 0.0f;
    if (oxygen_read(&o2_val) == O2_OK) gathered_data->oxygen_pct = o2_val;
    else 
    { 
        gathered_data->oxygen_pct = -1.0f; // Error Flag
        LOG("[O2] ERROR: Read failed.\n");
    }
}

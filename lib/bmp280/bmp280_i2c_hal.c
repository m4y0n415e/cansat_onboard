#include "bmp280_i2c_hal.h"

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define I2C_PORT i2c0
#define SDA_PIN  4
#define SCL_PIN  5

int16_t bmp280_i2c_hal_init(void)
{
    // i2c_init(I2C_PORT, 100 * 1000);

    // gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    // gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // gpio_pull_up(SDA_PIN);
    // gpio_pull_up(SCL_PIN);

    // sleep_ms(10);

    return BMP280_OK;
}

int16_t bmp280_i2c_hal_read(uint8_t address,
                            uint8_t *reg,
                            uint8_t *data,
                            uint16_t count)
{
    // Write register address
    int ret = i2c_write_blocking(
        I2C_PORT,
        address,
        reg,
        1,
        true   // repeated start
    );

    if (ret < 0)
        return BMP280_ERR;

    // Read data
    ret = i2c_read_blocking(
        I2C_PORT,
        address,
        data,
        count,
        false
    );

    return (ret < 0) ? BMP280_ERR : BMP280_OK;
}

int16_t bmp280_i2c_hal_write(uint8_t address,
                             uint8_t *data,
                             uint16_t count)
{
    int ret = i2c_write_blocking(
        I2C_PORT,
        address,
        data,
        count,
        false
    );

    return (ret < 0) ? BMP280_ERR : BMP280_OK;
}

void bmp280_i2c_hal_ms_delay(uint32_t ms)
{
    sleep_ms(ms);
}

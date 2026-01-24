/*
 * Driver for DFRobot SEN0465 Oxygen Sensor on RSP Pico 2
 * Author: m4y0n415e on GitHub
 */

#include "dfrobot_oxygen.h"

#define O2_ADDR        0x74
#define O2_I2C_PORT    i2c0 

static uint8_t calc_checksum(uint8_t* buf) 
{
    uint8_t sum = 0;
    for (int i = 1; i < 8; i++) sum += buf[i];
    return (~sum) + 1;
}

int8_t oxygen_read(float* concentration) 
{
    uint8_t tx_buf[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    tx_buf[8] = calc_checksum(tx_buf);

    int ret = i2c_write_blocking(O2_I2C_PORT, O2_ADDR, tx_buf, 9, false);
    if (ret == PICO_ERROR_GENERIC) return O2_ERR;

    sleep_ms(100);

    uint8_t rx_buf[9];
    ret = i2c_read_blocking(O2_I2C_PORT, O2_ADDR, rx_buf, 9, false);
    if (ret == PICO_ERROR_GENERIC) return O2_ERR;

    if (rx_buf[0] == 0xFF && rx_buf[8] == calc_checksum(rx_buf)) 
    {
        uint16_t raw = (rx_buf[2] << 8) | rx_buf[3];
        *concentration = (float)raw * 0.1f; 
        return O2_OK;
    }
    else return O2_ERR;
}

int8_t oxygen_init(void) 
{
    // "Pinging" the sensor by trying to read a value
    float val;
    if (oxygen_read(&val) == O2_OK) return O2_OK;
    else return O2_ERR;
}
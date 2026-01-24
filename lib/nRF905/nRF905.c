#include "nRF905.h"
#include <string.h>

// Commands
#define CMD_W_CONFIG      0x00
#define CMD_R_CONFIG      0x10
#define CMD_W_TX_PAYLOAD  0x20
#define CMD_R_RX_PAYLOAD  0x24
#define CMD_W_TX_ADDRESS  0x22
#define CMD_R_TX_ADDRESS  0x23

// Instruction to setup the registers (Standard 433MHz config)
// CH_NO=0x6C (433.2MHz), AUTO_RETRAN=0, RX_RED_PWR=0, PA_PWR=Max
static const uint8_t config_registers[] = {
    0x6C,       // CH_NO (433.2 MHz)
    0x00,       // Frequency Band (433) + CRC Config
    0x44,       // TX Address Width (4) + RX Address Width (4)
    0x20,       // RX Payload Width (32 bytes)
    0x20,       // TX Payload Width (32 bytes)
    0xCC,       // RX Address [0] (Address: 0xCCCCCCCC)
    0xCC,       // RX Address [1]
    0xCC,       // RX Address [2]
    0xCC,       // RX Address [3]
    0x58        // CRC Enable + 16Mhz Xtal + Max Power
};

// Helper: Write SPI Command + Data
static void nrf_write_config(uint8_t cmd, const uint8_t *data, uint8_t len) {
    gpio_put(PIN_CSN, 0);
    spi_write_blocking(NRF_SPI_PORT, &cmd, 1);
    spi_write_blocking(NRF_SPI_PORT, data, len);
    gpio_put(PIN_CSN, 1);
}

// Helper: Set TX Address
static void nrf_set_tx_addr() {
    // We send to 0xCCCCCCCC (Must match RX config above)
    uint8_t addr[4] = {0xCC, 0xCC, 0xCC, 0xCC};
    nrf_write_config(CMD_W_TX_ADDRESS, addr, 4);
}

void nrf905_init(void) {
    // 1. Init SPI
    spi_init(NRF_SPI_PORT, 1000 * 1000); // 1 MHz
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

    // 2. Init GPIOs
    gpio_init(PIN_CSN); gpio_set_dir(PIN_CSN, GPIO_OUT); gpio_put(PIN_CSN, 1);
    
    gpio_init(PIN_TX_EN);  gpio_set_dir(PIN_TX_EN, GPIO_OUT);
    gpio_init(PIN_TRX_CE); gpio_set_dir(PIN_TRX_CE, GPIO_OUT);
    gpio_init(PIN_PWR);    gpio_set_dir(PIN_PWR, GPIO_OUT);
    
    gpio_init(PIN_DR);     gpio_set_dir(PIN_DR, GPIO_IN);

    // 3. Power Up
    gpio_put(PIN_PWR, 1);
    gpio_put(PIN_TRX_CE, 0); // Standby
    gpio_put(PIN_TX_EN, 0);
    sleep_ms(3); // Start-up time

    // 4. Configure Registers
    nrf_write_config(CMD_W_CONFIG, config_registers, sizeof(config_registers));
    nrf_set_tx_addr();

    // 5. Default to RX Mode
    nrf905_wakeup();
}

void nrf905_tx(uint8_t *data, uint8_t len) {
    // 1. Standby
    gpio_put(PIN_TRX_CE, 0);
    
    // 2. Set TX Mode
    gpio_put(PIN_TX_EN, 1);
    
    // 3. Write Payload
    // If data is less than 32 bytes, pad with zeros
    uint8_t buffer[NRF905_PAYLOAD_SIZE] = {0};
    if (len > NRF905_PAYLOAD_SIZE) len = NRF905_PAYLOAD_SIZE;
    memcpy(buffer, data, len);
    
    nrf_write_config(CMD_W_TX_PAYLOAD, buffer, NRF905_PAYLOAD_SIZE);

    // 4. Start Transmission (Pulse TRX_CE)
    gpio_put(PIN_TRX_CE, 1);
    // sleep_us(15); // Min pulse usually required
    // Wait for transmission to finish (DR goes high when done in TX mode)
    // Safety timeout loop
    int timeout = 1000;
    while (!gpio_get(PIN_DR) && timeout--) { sleep_us(10); } 
    
    gpio_put(PIN_TRX_CE, 0);
    
    // 5. Return to RX Mode
    gpio_put(PIN_TX_EN, 0);
    gpio_put(PIN_TRX_CE, 1);
}

bool nrf905_data_ready(void) {
    return gpio_get(PIN_DR);
}

void nrf905_rx(uint8_t *buffer) {
    // Read payload command
    uint8_t cmd = CMD_R_RX_PAYLOAD;
    
    gpio_put(PIN_CSN, 0);
    spi_write_blocking(NRF_SPI_PORT, &cmd, 1);
    spi_read_blocking(NRF_SPI_PORT, 0, buffer, NRF905_PAYLOAD_SIZE);
    gpio_put(PIN_CSN, 1);
    
    // Reset DR is handled automatically by chip upon reading
}

void nrf905_sleep(void) {
    gpio_put(PIN_PWR, 0);
}

void nrf905_wakeup(void) {
    gpio_put(PIN_PWR, 1);
    gpio_put(PIN_TX_EN, 0); // RX Mode
    gpio_put(PIN_TRX_CE, 1); // Enable
    sleep_ms(3); // Oscillator startup
}
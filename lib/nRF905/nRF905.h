#ifndef NRF905_H
#define NRF905_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdint.h>
#include <stdbool.h>

// CONFIGURATION

// Frequency: 433MHz (Band 0, Channel 0x6C for 433.2 MHz)
#define NRF905_FREQ_433MHZ

#define NRF905_PAYLOAD_SIZE 32 

#define NRF_SPI_PORT spi1
#define PIN_CSN      9
#define PIN_SCK      10
#define PIN_MOSI     11
#define PIN_MISO     12

#define PIN_DR       13 // Data ready
#define PIN_TX_EN    20
#define PIN_TRX_CE   21
#define PIN_PWR      22

// FUNCTIONS

extern void nrf905_init(void);

extern void nrf905_tx(uint8_t *data, uint8_t len);

extern bool nrf905_data_ready(void);

extern void nrf905_rx(uint8_t *buffer);

extern void nrf905_sleep(void);

extern void nrf905_wakeup(void);

#endif
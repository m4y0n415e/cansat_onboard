#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "debug_mode.h"
#include "radio_module.h"
#include "lib/nrf905/nrf905.h"

void radio_module_init(void) 
{
    nrf905_init();
    printf("[nRF905]: Radio Module Initialized (433 MHz)\n");
}

void radio_module_send_telemetry(float temp, float press, float alt) 
{
    char buffer[NRF905_PAYLOAD_SIZE]; // 32 bytes max

    snprintf(buffer, sizeof(buffer), "T:%.1f P:%.0f A:%.1f", temp, press, alt);

    LOG("[nRF905 RADIO TX] %s\n", buffer);

    // Casting the char* to uint8_t* because the driver expects raw bytes
    nrf905_tx((uint8_t*)buffer, strlen(buffer));
}
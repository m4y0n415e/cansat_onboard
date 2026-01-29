#ifndef DFROBOT_OXYGEN_H
#define DFROBOT_OXYGEN_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/error.h"
#include "hardware/i2c.h"

// Standard return codes -  OK and ERROR
#define O2_OK   0
#define O2_ERR -1

extern int8_t oxygen_init(void);

extern int8_t oxygen_read(float* concentration);

#endif
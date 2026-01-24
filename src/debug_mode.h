/** @file debug_mode.h
 * @brief Conditional debug logging and error handling macros.
 * 
 * * @details Provides the `LOG` macro which directs output to the standard 
 * I/O (USB/UART) only when `DEBUG_MODE` is defined. This allows for logging 
 * during development that can be completely compiled out for the final 
 * flight release.
 */

#ifndef DEBUG_MODE_H
#define DEBUG_MODE_H

#include <stdio.h>

#define DEBUG_MODE

#ifdef DEBUG_MODE
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...) ((void)0)
#endif

#endif // DEBUG_MODE_H
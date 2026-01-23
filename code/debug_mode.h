/** @file debug_mode.h
 * @brief 
 * 
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

#endif
/** @file gps_module.c
 *  @brief Implementation of GPS NMEA parsing and UART handling.
 * 
 * @see gps_module.h for the public API and data structures.
 */

#include <string.h>
#include "gps_module.h"
#include "time_manager.h"
#include "debug_mode.h"
#include "minmea.h"
#include "hardware/uart.h"

#define NMEA_BUFFER_LEN 85

static char line_buffer[NMEA_BUFFER_LEN];
static int buffer_pos = 0;
static gps_data_t last_data = {0};

void gps_init(void)
{
    uart_init(GPS_UART_ID, GPS_BAUD_RATE);
    gpio_set_function(GPS_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GPS_RX_PIN, GPIO_FUNC_UART);
}

/** @brief Reads a line of data received from the GPS module and parses it.   
 * 
 * It checks the sentence ID using the `minmea` library and parses specific frames:
 * - **RMC (Recommended Minimum):** Extracts Latitude, Longitude, UTC Time, and Fix validity.
 * - **GGA (Global Positioning System Fix Data):** Extracts Altitude and Satellite count.
 * 
 * If the parsing is successful and the data is valid, the static `last_data`
 * structure is updated with the new values.
 *
 * @param[in] line  A null-terminated string containing the raw NMEA sentence
 * (e.g., "$GPRMC,123519,A,4807.038,N...").
 *
 * @note This function modifies the internal static variable `last_data`.
 * @warning This function assumes `line` is a valid C-string.
*/
static void read_and_process(char *line)
{
    if (minmea_sentence_id(line, false) == MINMEA_SENTENCE_RMC)
    {
        struct minmea_sentence_rmc frame;
        
        if (minmea_parse_rmc(&frame, line))
        {
            if (frame.valid)
            {
                last_data.fix = true;
                last_data.latitude = minmea_tocoord(&frame.latitude);
                last_data.longitude = minmea_tocoord(&frame.longitude);

                last_data.hour = frame.time.hours;
                last_data.min = frame.time.minutes;
                last_data.sec = frame.time.seconds;

                last_data.day = frame.date.day;
                last_data.month = frame.date.month;
                last_data.year = frame.date.year + 2000;
            }
            else last_data.fix = false;
        }
    } 
    else if (minmea_sentence_id(line, false) == MINMEA_SENTENCE_GGA)
    {
        struct minmea_sentence_gga frame;
        if (minmea_parse_gga(&frame, line)) 
        {
            // Checking fix quality (0 = Invalid, 1 = GPS Fix, 2 = DGPS Fix)
            if (frame.fix_quality > 0) 
            {
                last_data.altitude = minmea_tofloat(&frame.altitude);
                last_data.satellites = frame.satellites_tracked;
            }
        }
    }
}

bool gps_update(void)
{
    bool new_data = false;
    
    while (uart_is_readable(GPS_UART_ID)) 
    {
        char c = uart_getc(GPS_UART_ID);

        if (c == '\n' || c == '\r') 
        {
            if (buffer_pos > 0)
            {
                line_buffer[buffer_pos] = '\0';
                read_and_process(line_buffer);
                new_data = true; // update flag - true
            }
            buffer_pos = 0; // cleaning the buffer for the next read sentence
        } else 
        {
            if (buffer_pos < NMEA_BUFFER_LEN - 1) line_buffer[buffer_pos++] = c;
            else buffer_pos = 0;
        }
    }
    return new_data;
}

void gps_get_data(gps_data_t *data) 
{ 
    *data = last_data;
}
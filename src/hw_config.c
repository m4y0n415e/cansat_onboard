#include "ff.h"
#include "diskio.h"   
#include "sd_card.h"  
#include "hardware/spi.h"
#include <strings.h>

static spi_t spis[] = 
{  
    {
        .hw_inst = spi0,  
        .miso_gpio = 16, 
        .mosi_gpio = 19, 
        .sck_gpio  = 18, 
        .baud_rate = 1000 * 1000, 
    }
};

static sd_card_t sd_cards[] = 
{  
    {
        .pcName = "0:",           
        .spi = &spis[0],          
        .ss_gpio = 17,            
        .use_card_detect = false, 
        .card_detect_gpio = -1,   
        .card_detected_true = -1, 
        .m_Status = STA_NOINIT    
    }
};

size_t sd_get_num() { return count_of(sd_cards); }

sd_card_t *sd_get_by_num(size_t num) {
    if (num <= sd_get_num()) return &sd_cards[num];
    else return NULL;
}

size_t spi_get_num() { return count_of(spis); }

spi_t *spi_get_by_num(size_t num) {
    if (num <= spi_get_num()) return &spis[num];
    else return NULL;
}
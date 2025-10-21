/******
Demo for ssd1306 i2c driver for  Raspberry Pi
******/
#include <stdio.h>
#include <stdlib.h>
#include "ssd1306_i2c.h"
#include "config.h"
#include "time.h"
#include <unistd.h>

#define CONFIG_FILE "display.conf"

int main(void)
{
    unsigned char symbol=0;

    /* Load configuration from file */
    printf("Loading configuration from %s...\n", CONFIG_FILE);
    if (load_config(CONFIG_FILE, &display_config) == 0) {
        printf("Using default configuration\n");
    } else {
        printf("Configuration loaded successfully\n");
    }

    /* Print configuration for debugging */
    print_config(&display_config);

    /* Initialize display */
    ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);      //LCD Screen initialization
    if(i2cd<0)
    {
        printf("I2C device failed to open\r\n");
        return 1;
    }
    usleep(150*1000);                                                  //Short delay Ensure the normal response of the lower function
    FirstGetIpAddress();                        //Get IP address

    /* Main display loop */
    while(1)
    {
        LCD_Display(symbol);
        sleep(1);
        sleep(1);
        sleep(1);
        symbol++;
        if(symbol==3)
        {
          symbol=0;
        }
    }
    return 0;
}

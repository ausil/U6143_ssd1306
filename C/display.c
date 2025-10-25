/******
Demo for ssd1306 i2c driver for  Raspberry Pi
******/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "ssd1306_i2c.h"
#include "config.h"
#include "time.h"
#include <unistd.h>

#define SYSTEM_CONFIG_FILE "/etc/uctronics-display.conf"
#define LOCAL_CONFIG_FILE "display.conf"

/* Print usage information */
void print_usage(const char *prog_name)
{
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("\n");
    printf("SSD1306 OLED Display Driver for UCTRONICS displays\n");
    printf("\n");
    printf("Options:\n");
    printf("  -c, --config FILE    Use specified configuration file\n");
    printf("  -h, --help           Show this help message\n");
    printf("\n");
    printf("Configuration file search order (if -c not specified):\n");
    printf("  1. %s\n", SYSTEM_CONFIG_FILE);
    printf("  2. %s (in same directory as executable)\n", LOCAL_CONFIG_FILE);
    printf("  3. Built-in defaults\n");
    printf("\n");
}

/* Try to load config file, return 1 if successful, 0 otherwise */
int try_load_config(const char *config_path)
{
    if (access(config_path, F_OK) == 0) {
        printf("Trying config file: %s\n", config_path);
        if (load_config(config_path, &display_config)) {
            printf("Configuration loaded successfully from: %s\n", config_path);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    unsigned char symbol=0;
    const char *config_file = NULL;
    int config_loaded = 0;

    /* Parse command-line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) {
            if (i + 1 < argc) {
                config_file = argv[++i];
            } else {
                fprintf(stderr, "Error: -c/--config requires a file path\n");
                print_usage(argv[0]);
                return 1;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    /* Load configuration */
    if (config_file != NULL) {
        /* User specified config file via command line */
        printf("Loading configuration from %s...\n", config_file);
        if (load_config(config_file, &display_config)) {
            printf("Configuration loaded successfully\n");
            config_loaded = 1;
        } else {
            fprintf(stderr, "Error: Could not load specified config file: %s\n", config_file);
            return 1;
        }
    } else {
        /* Try default locations in order */
        /* 1. System-wide config */
        if (try_load_config(SYSTEM_CONFIG_FILE)) {
            config_loaded = 1;
        }
        /* 2. Local config in executable directory */
        else {
            char exe_path[1024];
            char local_config_path[1024];
            ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
            if (len != -1) {
                exe_path[len] = '\0';
                char *exe_dir = dirname(exe_path);
                snprintf(local_config_path, sizeof(local_config_path), "%s/%s", exe_dir, LOCAL_CONFIG_FILE);
                if (try_load_config(local_config_path)) {
                    config_loaded = 1;
                }
            }
        }
    }

    if (!config_loaded) {
        printf("No configuration file found, using built-in defaults\n");
        set_default_config(&display_config);
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

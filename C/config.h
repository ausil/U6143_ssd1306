/*********************************************************************
* Configuration file parser for SSD1306 display
* Supports INI-style configuration files
*********************************************************************/
#ifndef CONFIG_H_
#define CONFIG_H_

/* Temperature display types */
#define CELSIUS       0
#define FAHRENHEIT    1

/* IP display switch */
#define IP_DISPLAY_OPEN     0
#define IP_DISPLAY_CLOSE    1

/* Configuration structure */
typedef struct {
    char i2c_device[64];            /* I2C device path, e.g., "/dev/i2c-3" */
    int temperature_type;           /* CELSIUS or FAHRENHEIT */
    int ip_switch;                  /* IP_DISPLAY_OPEN or IP_DISPLAY_CLOSE */
    char custom_display[32];        /* Custom text when IP display is closed */
    char network_interface[16];     /* Network interface name, e.g., "end0" or "wlan0" */
} DisplayConfig;

/* Default configuration values */
#define DEFAULT_I2C_DEVICE      "/dev/i2c-3"
#define DEFAULT_TEMPERATURE     FAHRENHEIT
#define DEFAULT_IP_SWITCH       IP_DISPLAY_OPEN
#define DEFAULT_CUSTOM_TEXT     "UCTRONICS"
#define DEFAULT_NET_INTERFACE   "end0"

/* Function prototypes */
int load_config(const char *config_file, DisplayConfig *config);
void set_default_config(DisplayConfig *config);
void print_config(const DisplayConfig *config);

#endif /* CONFIG_H_ */

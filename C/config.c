#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

/* Helper function to trim whitespace from both ends of a string */
static char* trim_whitespace(char *str)
{
    char *end;

    /* Trim leading space */
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  /* All spaces? */
        return str;

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    /* Write new null terminator */
    end[1] = '\0';

    return str;
}

/* Helper function to remove quotes from a string */
static void remove_quotes(char *str)
{
    size_t len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len-1] == '"') {
        memmove(str, str+1, len-2);
        str[len-2] = '\0';
    }
}

/* Set default configuration values */
void set_default_config(DisplayConfig *config)
{
    strncpy(config->i2c_device, DEFAULT_I2C_DEVICE, sizeof(config->i2c_device) - 1);
    config->i2c_device[sizeof(config->i2c_device) - 1] = '\0';

    config->temperature_type = DEFAULT_TEMPERATURE;
    config->ip_switch = DEFAULT_IP_SWITCH;

    strncpy(config->custom_display, DEFAULT_CUSTOM_TEXT, sizeof(config->custom_display) - 1);
    config->custom_display[sizeof(config->custom_display) - 1] = '\0';

    strncpy(config->network_interface, DEFAULT_NET_INTERFACE, sizeof(config->network_interface) - 1);
    config->network_interface[sizeof(config->network_interface) - 1] = '\0';
}

/* Parse configuration file */
int load_config(const char *config_file, DisplayConfig *config)
{
    FILE *fp;
    char line[256];
    char *key, *value;

    /* Set defaults first */
    set_default_config(config);

    /* Try to open config file */
    fp = fopen(config_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Warning: Could not open config file '%s', using defaults\n", config_file);
        return 0;  /* Not a fatal error, we have defaults */
    }

    /* Parse line by line */
    while (fgets(line, sizeof(line), fp)) {
        char *line_ptr = trim_whitespace(line);

        /* Skip empty lines and comments */
        if (line_ptr[0] == '\0' || line_ptr[0] == '#' || line_ptr[0] == ';') {
            continue;
        }

        /* Skip section headers [section] */
        if (line_ptr[0] == '[') {
            continue;
        }

        /* Find the '=' separator */
        char *equals = strchr(line_ptr, '=');
        if (equals == NULL) {
            continue;  /* Skip malformed lines */
        }

        /* Split into key and value */
        *equals = '\0';
        key = trim_whitespace(line_ptr);
        value = trim_whitespace(equals + 1);

        /* Remove quotes from value if present */
        remove_quotes(value);

        /* Parse configuration options */
        if (strcmp(key, "i2c_device") == 0) {
            strncpy(config->i2c_device, value, sizeof(config->i2c_device) - 1);
            config->i2c_device[sizeof(config->i2c_device) - 1] = '\0';
        }
        else if (strcmp(key, "temperature_type") == 0) {
            if (strcasecmp(value, "celsius") == 0 || strcmp(value, "0") == 0) {
                config->temperature_type = CELSIUS;
            } else if (strcasecmp(value, "fahrenheit") == 0 || strcmp(value, "1") == 0) {
                config->temperature_type = FAHRENHEIT;
            }
        }
        else if (strcmp(key, "ip_switch") == 0) {
            if (strcasecmp(value, "open") == 0 || strcmp(value, "0") == 0) {
                config->ip_switch = IP_DISPLAY_OPEN;
            } else if (strcasecmp(value, "close") == 0 || strcmp(value, "1") == 0) {
                config->ip_switch = IP_DISPLAY_CLOSE;
            }
        }
        else if (strcmp(key, "custom_display") == 0) {
            strncpy(config->custom_display, value, sizeof(config->custom_display) - 1);
            config->custom_display[sizeof(config->custom_display) - 1] = '\0';
        }
        else if (strcmp(key, "network_interface") == 0) {
            strncpy(config->network_interface, value, sizeof(config->network_interface) - 1);
            config->network_interface[sizeof(config->network_interface) - 1] = '\0';
        }
    }

    fclose(fp);
    return 1;  /* Success */
}

/* Print configuration for debugging */
void print_config(const DisplayConfig *config)
{
    printf("Configuration:\n");
    printf("  I2C Device: %s\n", config->i2c_device);
    printf("  Temperature Type: %s\n",
           config->temperature_type == CELSIUS ? "Celsius" : "Fahrenheit");
    printf("  Network Interface: %s\n", config->network_interface);
    printf("  IP Display: %s\n",
           config->ip_switch == IP_DISPLAY_OPEN ? "Open" : "Closed");
    printf("  Custom Display: %s\n", config->custom_display);
}

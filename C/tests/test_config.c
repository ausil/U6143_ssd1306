/*
 * Tests for configuration parser
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_framework.h"
#include "../config.h"

void test_default_config(void)
{
    DisplayConfig config;
    printf("\nTest: Default Configuration\n");

    set_default_config(&config);

    ASSERT_STR_EQUAL("/dev/i2c-3", config.i2c_device,
        "Default I2C device should be /dev/i2c-3");
    ASSERT_EQUAL(FAHRENHEIT, config.temperature_type,
        "Default temperature should be FAHRENHEIT");
    ASSERT_EQUAL(IP_DISPLAY_OPEN, config.ip_switch,
        "Default IP switch should be OPEN");
    ASSERT_STR_EQUAL("UCTRONICS", config.custom_display,
        "Default custom display should be UCTRONICS");
    ASSERT_STR_EQUAL("end0", config.network_interface,
        "Default network interface should be end0");
}

void test_valid_config_file(void)
{
    DisplayConfig config;
    printf("\nTest: Valid Configuration File\n");

    int result = load_config("tests/test_valid.conf", &config);

    ASSERT_TRUE(result == 1, "Should successfully load valid config file");
    ASSERT_STR_EQUAL("/dev/i2c-7", config.i2c_device,
        "I2C device should be /dev/i2c-7");
    ASSERT_EQUAL(CELSIUS, config.temperature_type,
        "Temperature type should be CELSIUS");
    ASSERT_STR_EQUAL("wlan0", config.network_interface,
        "Network interface should be wlan0");
    ASSERT_EQUAL(IP_DISPLAY_CLOSE, config.ip_switch,
        "IP switch should be CLOSE");
    ASSERT_STR_EQUAL("TEST123", config.custom_display,
        "Custom display should be TEST123");
}

void test_partial_config_file(void)
{
    DisplayConfig config;
    printf("\nTest: Partial Configuration File (uses defaults for missing values)\n");

    int result = load_config("tests/test_partial.conf", &config);

    ASSERT_TRUE(result == 1, "Should successfully load partial config file");
    ASSERT_STR_EQUAL("/dev/i2c-5", config.i2c_device,
        "I2C device should be /dev/i2c-5 from config");
    ASSERT_EQUAL(FAHRENHEIT, config.temperature_type,
        "Temperature type should be FAHRENHEIT from config");
    // These should be defaults
    ASSERT_STR_EQUAL("end0", config.network_interface,
        "Network interface should be default (end0)");
    ASSERT_EQUAL(IP_DISPLAY_OPEN, config.ip_switch,
        "IP switch should be default (OPEN)");
    ASSERT_STR_EQUAL("UCTRONICS", config.custom_display,
        "Custom display should be default (UCTRONICS)");
}

void test_nonexistent_config_file(void)
{
    DisplayConfig config;
    printf("\nTest: Non-existent Configuration File (should use defaults)\n");

    int result = load_config("tests/nonexistent.conf", &config);

    ASSERT_FALSE(result == 1, "Should fail to load non-existent config file");
    // Config should have defaults
    ASSERT_STR_EQUAL("/dev/i2c-3", config.i2c_device,
        "Should use default I2C device");
}

void test_temperature_type_values(void)
{
    printf("\nTest: Temperature Type Values\n");

    ASSERT_EQUAL(0, CELSIUS, "CELSIUS constant should be 0");
    ASSERT_EQUAL(1, FAHRENHEIT, "FAHRENHEIT constant should be 1");
}

void test_ip_switch_values(void)
{
    printf("\nTest: IP Switch Values\n");

    ASSERT_EQUAL(0, IP_DISPLAY_OPEN, "IP_DISPLAY_OPEN constant should be 0");
    ASSERT_EQUAL(1, IP_DISPLAY_CLOSE, "IP_DISPLAY_CLOSE constant should be 1");
}

int main(void)
{
    TEST_SUITE("Configuration Parser Tests");

    test_default_config();
    test_valid_config_file();
    test_partial_config_file();
    test_nonexistent_config_file();
    test_temperature_type_values();
    test_ip_switch_values();

    TEST_RESULTS();
}

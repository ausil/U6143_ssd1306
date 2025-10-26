# Tests for UCTRONICS Display Driver

This directory contains automated tests for the display driver.

## Running Tests

### Run all tests:
```bash
make test
```

### Clean test artifacts:
```bash
make test-clean
```

## Test Suites

### Configuration Parser Tests (`test_config.c`)

Tests the INI-style configuration file parser:

- **Default Configuration** - Verifies default values are set correctly
- **Valid Config File** - Tests parsing a complete configuration file
- **Partial Config File** - Tests that missing values use defaults
- **Non-existent File** - Tests graceful handling of missing config files
- **Temperature Type Constants** - Verifies CELSIUS/FAHRENHEIT values
- **IP Switch Constants** - Verifies IP_DISPLAY_OPEN/CLOSE values

**Test Files:**
- `test_valid.conf` - Complete configuration with all options
- `test_partial.conf` - Partial configuration (tests default fallback)

## Test Framework

The tests use a simple custom framework defined in `test_framework.h`:

### Available Assertions:
- `ASSERT_TRUE(condition, message)` - Assert condition is true
- `ASSERT_FALSE(condition, message)` - Assert condition is false
- `ASSERT_EQUAL(expected, actual, message)` - Assert numeric equality
- `ASSERT_STR_EQUAL(expected, actual, message)` - Assert string equality

### Test Output:
- Green ✓ for passing tests
- Red ✗ for failing tests
- Summary with total/passed/failed counts

## Adding New Tests

1. Create a new test file in `tests/`:
   ```c
   #include "test_framework.h"
   #include "../your_module.h"

   void test_your_feature(void) {
       printf("\nTest: Your Feature\n");
       ASSERT_TRUE(1 == 1, "Should be true");
   }

   int main(void) {
       TEST_SUITE("Your Test Suite");
       test_your_feature();
       TEST_RESULTS();
   }
   ```

2. Add build rule to Makefile:
   ```makefile
   tests/test_your_module: tests/test_your_module.c your_module.o
       $(CXX) $(CFLAGS) tests/test_your_module.c your_module.o -o tests/test_your_module
   ```

3. Add to test target dependencies:
   ```makefile
   test: tests/test_config tests/test_your_module
       @./tests/test_config
       @./tests/test_your_module
   ```

## Test Coverage

Currently tested:
- ✓ Configuration parser (all functions)
- ✓ Default configuration values
- ✓ Configuration file loading
- ✓ Error handling for missing files

Not yet tested (hardware-dependent):
- Display initialization
- I2C communication
- Display rendering
- System information gathering

## Future Improvements

- Add mock I2C interface for hardware function testing
- Add integration tests with mock hardware
- Add performance/benchmark tests
- Add memory leak detection (valgrind integration)
- Add code coverage reporting (gcov)

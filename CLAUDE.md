# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a driver library for UCTRONICS SSD1306 OLED displays (128x32 I2C) on Raspberry Pi and compatible single-board computers. The display shows system information including CPU temperature, load, memory usage, and SD card storage.

## Build and Run

### C Implementation (Primary)

Build the display program:
```bash
cd C
make clean
make
```

Run manually:
```bash
cd C
./display              # Uses default config file locations
./display -h           # Show help and config file search order
./display -c custom.conf  # Use custom config file
```

Install as systemd service (runs on boot):
```bash
sudo ./setup_display_service.sh
```

Service management:
```bash
sudo systemctl status uctronics-display.service
sudo systemctl stop uctronics-display.service
sudo systemctl start uctronics-display.service
sudo journalctl -u uctronics-display.service
```

### Python Implementation (Legacy - for older displays without MCU)

```bash
cd python
sudo python3 ssd1306_stats.py
```

## Configuration

The display is configured via an INI-style configuration file. The program searches for configuration in this order:

1. Command-line specified: `./display -c /path/to/config.conf`
2. System-wide: `/etc/uctronics-display.conf`
3. Local to executable: `display.conf` (in same directory as binary)
4. Built-in defaults

### Configuration File Format

```ini
# I2C device path
i2c_device = /dev/i2c-3

# Temperature display type: celsius, fahrenheit
temperature_type = fahrenheit

# Network interface name - the actual interface to query for IP
network_interface = end0

# IP display: open (show IP), close (show custom text)
ip_switch = open

# Custom text (shown when ip_switch = close)
custom_display = UCTRONICS
```

### Platform-Specific Settings

**I2C Device:**
- Rock 3C: `/dev/i2c-3`
- Older Raspberry Pi: `/dev/i2c-1`
- Verify available: `ls /dev/i2c-*`

**Network Interface Names:**
- Rock 3C/Fedora: `end0` (Ethernet), `wlan0` (WiFi)
- Traditional: `eth0` (Ethernet), `wlan0` (WiFi)
- Check with: `ip addr show`

**Temperature Units:**
- `celsius` or `fahrenheit`

**IP Display:**
- `open` = display IP address on screen
- `close` = display custom text instead

## Architecture

### Core Components

**`C/config.c` / `C/config.h`** - Configuration system
- INI-style config file parser
- `load_config()` reads `display.conf` and populates `DisplayConfig` structure
- `set_default_config()` provides fallback defaults
- Global `display_config` variable accessed throughout the codebase

**`C/ssd1306_i2c.c`** - Main driver implementation
- Low-level I2C communication via configurable `/dev/i2c-*` device
- Display initialization and control
- System information gathering (CPU temp from `/sys/class/thermal/thermal_zone0/temp`, memory from `/proc/meminfo`, disk usage)
- Network interface querying (ioctl SIOCGIFADDR) using configured interface name

**`C/display.c`** - Application entry point
- Loads configuration from `display.conf` at startup
- Initializes display via `ssd1306_begin()`
- Cycles through 3 display modes every 3 seconds:
  - Mode 0: Temperature + CPU load + IP/custom text
  - Mode 1: RAM usage (free/total)
  - Mode 2: SD card usage (used/total)

**`C/ssd1306_i2c.h`** - Public API and type definitions

**`C/bmp.h`** - Bitmap graphics data (backgrounds for each display mode)

**`C/oled_fonts.h`** - Font data (8x16 and 6x8 character sets)

### Display Rendering

The SSD1306 is a 128x32 monochrome OLED organized as 4 pages (rows) of 128 bytes:
- `OLED_Set_Pos(x, y)` sets cursor position
- `OLED_WR_Byte(data, OLED_DATA)` writes pixel data
- `OLED_ShowChar()` / `OLED_ShowString()` render text using font arrays
- `OLED_DrawBMP()` / `OLED_DrawPartBMP()` render bitmap backgrounds
- `OLED_Clear()` / `OLED_ClearLint()` clear full screen or specific rows

### System Integration

**`setup_display_service.sh`** - Automated installer
- Detects user home directory from `$SUDO_USER`
- Compiles C program via Makefile
- Creates systemd service file at `/etc/systemd/system/uctronics-display.service`
- Enables and starts service
- Service runs as root (required for I2C hardware access)

## Platform Notes

**Configuration vs. Code:**
- All platform-specific settings are now in `C/display.conf`
- No need to modify C code for different platforms
- Simply edit `display.conf` before running or installing the service

**Supporting New Platforms:**
1. Identify I2C bus: `ls /dev/i2c-*` → update `i2c_device` in config
2. Identify network interface: `ip addr show` → update `network_interface` in config
3. Ensure I2C permissions: user must be in `i2c` group or run as root

**Historical Hardcoded Values (now configurable):**
- Rock 3C defaults: `i2c-3`, `end0` (Fedora network naming)
- Traditional Raspberry Pi: `i2c-1`, `eth0`

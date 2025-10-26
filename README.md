# U6143_ssd1306

Driver for UCTRONICS SSD1306 OLED displays (128x32 I2C). Displays system information including CPU temperature, load, memory usage, disk storage, and IP address.

Supports Raspberry Pi, Rock 3C, and other compatible single-board computers.

## Features

- **Multiple Implementations:**
  - **C implementation** (recommended) - Modern driver with systemd integration
  - **Python implementation** - For older displays without MCU controller

- **C Driver Features:**
  - Graceful shutdown with signal handling
  - Comprehensive error handling and reporting
  - INI-style configuration file
  - Systemd service integration
  - Auto-restart on failure
  - Platform-independent design

## Quick Start

### Enable I2C Interface

**Raspberry Pi:**
```bash
sudo raspi-config
```
Choose **Interface Options** → Enable **I2C**

**Rock 3C / Other SBCs:**
Ensure I2C is enabled in your board's configuration.

### Installation

```bash
# Clone repository
git clone https://github.com/UCTRONICS/U6143_ssd1306.git
cd U6143_ssd1306/C

# Build and install
make
sudo make install

# Enable auto-start on boot
sudo systemctl enable --now uctronics-display.service
```

That's it! The display should now be running and will start automatically on boot.

## Detailed Documentation

For detailed installation, configuration, and troubleshooting instructions, see:

- **[C Implementation README](C/README.md)** - Full documentation for the C driver

## Configuration

The C driver uses a configuration file instead of editing source code.

Edit `/etc/uctronics-display.conf` (or `C/display.conf` before installation):

```ini
# I2C device path
i2c_device = /dev/i2c-3

# Temperature display: celsius or fahrenheit
temperature_type = fahrenheit

# Network interface: eth0, end0, wlan0, etc.
network_interface = end0

# IP display: open (show IP) or close (show custom text)
ip_switch = open

# Custom text (when ip_switch = close)
custom_display = UCTRONICS
```

### Platform-Specific Defaults

**Rock 3C (Fedora):**
- I2C: `/dev/i2c-3`
- Network: `end0` (Ethernet)

**Raspberry Pi:**
- I2C: `/dev/i2c-1`
- Network: `eth0` (Ethernet)

Check your I2C device: `ls /dev/i2c-*`
Check your network interface: `ip addr show`

## Usage

### Service Management

```bash
# Check status
systemctl status uctronics-display.service

# Start/stop
sudo systemctl start uctronics-display.service
sudo systemctl stop uctronics-display.service

# Enable/disable auto-start
sudo systemctl enable uctronics-display.service
sudo systemctl disable uctronics-display.service

# View logs
journalctl -u uctronics-display.service
```

### Manual Execution

```bash
# Run with system config
uctronics-display

# Run with custom config
uctronics-display -c /path/to/config.conf

# Show help
uctronics-display -h
```

Press **Ctrl+C** to exit gracefully.

## Uninstallation

```bash
cd U6143_ssd1306/C
sudo make uninstall
```

This removes the binary, configuration file, and systemd service.

## Python Implementation (Legacy)

For older 0.91 inch LCD displays **without MCU controller**, use the Python implementation:

### Install Dependencies

```bash
sudo apt-get install python3-pip python3-pil
sudo pip3 install adafruit-circuitpython-ssd1306
```

### Run

```bash
cd U6143_ssd1306/python
sudo python3 ssd1306_stats.py
```

## Display Modes

The display cycles through 3 modes every 3 seconds:

1. **Mode 0:** CPU temperature, load, IP address (or custom text)
2. **Mode 1:** RAM usage (free / total)
3. **Mode 2:** SD card usage (used / total)

## Makefile Targets

| Target | Description |
|--------|-------------|
| `make` | Build the binary |
| `make install` | Install binary, config, and systemd service |
| `make uninstall` | Remove everything |
| `make enable-service` | Enable and start the service |
| `make disable-service` | Stop and disable the service |
| `make test` | Run automated tests |
| `make release` | Create release tarball (e.g., uctronics-display-1.0.0.tar.gz) |

## Troubleshooting

### Display Not Working

Check I2C device exists:
```bash
ls -l /dev/i2c-*
```

Test I2C connection:
```bash
sudo i2cdetect -y 3  # Replace 3 with your I2C bus number
```
Device should appear at address `0x3c`.

### Service Not Starting

```bash
systemctl status uctronics-display.service
journalctl -u uctronics-display.service -n 50
```

### IP Shows 0.0.0.0

Update network interface in `/etc/uctronics-display.conf`:
```bash
# Check interface name
ip addr show

# Edit config
sudo nano /etc/uctronics-display.conf

# Restart service
sudo systemctl restart uctronics-display.service
```

## Version

Current version: **1.0.0**

## Changes from Previous Versions

### v1.0.0 (Current)

- **Removed:** `setup_display_service.sh` script
- **New:** Makefile-based installation system
- **New:** INI-style configuration file (`display.conf`)
- **New:** Graceful shutdown with signal handling
- **New:** Comprehensive error handling
- **New:** Version flag (`--version` / `-v`)
- **New:** Systemd preset file for automatic service enablement
- **Changed:** Binary renamed from `display` to `uctronics-display`
- **Changed:** Configuration via file instead of editing header files
- **Improved:** Systemd service integration

## Contributing

Issues and pull requests welcome at: https://github.com/UCTRONICS/U6143_ssd1306

## License

See repository for license information.

## Support

For technical support and questions:
- GitHub Issues: https://github.com/UCTRONICS/U6143_ssd1306/issues
- Detailed C driver docs: [C/README.md](C/README.md)

# UCTRONICS SSD1306 OLED Display Driver

C driver for UCTRONICS SSD1306 OLED displays (128x32 I2C) on Raspberry Pi and compatible single-board computers. Displays system information including CPU temperature, load, memory usage, disk storage, and IP address.

## Hardware Setup

### Enable I2C Interface

**Raspberry Pi:**
```bash
sudo raspi-config
```
Choose **Interface Options** → Enable **I2C**

**Rock 3C / Other SBCs:**
Ensure I2C is enabled in your board's configuration.

### Identify I2C Device

Check which I2C bus your display is connected to:
```bash
ls /dev/i2c-*
```
Common values:
- `/dev/i2c-1` - Raspberry Pi
- `/dev/i2c-3` - Rock 3C

## Installation

### 1. Download the Library

```bash
git clone https://github.com/UCTRONICS/U6143_ssd1306.git
cd U6143_ssd1306/C
```

### 2. Configure (Optional)

Edit `display.conf` to match your hardware:
```ini
# I2C device path
i2c_device = /dev/i2c-3

# Temperature display: celsius or fahrenheit
temperature_type = fahrenheit

# Network interface: check with 'ip addr show'
network_interface = end0

# IP display: open (show IP) or close (show custom text)
ip_switch = open

# Custom text (when ip_switch = close)
custom_display = UCTRONICS
```

### 3. Build and Install

```bash
make
sudo make install
```

This installs:
- Binary → `/usr/bin/uctronics-display`
- Config → `/etc/uctronics-display.conf`
- Service → `/usr/lib/systemd/system/uctronics-display.service`
- Preset → `/usr/lib/systemd/system-preset/90-uctronics-display.preset`

### 4. Enable Auto-Start on Boot

The systemd preset file automatically enables the service on installation. To manually control:

```bash
# Enable and start
sudo systemctl enable --now uctronics-display.service

# Or use systemd presets
sudo systemctl preset uctronics-display.service
```

## Usage

### Run Manually

```bash
# Run with system config
uctronics-display

# Run with custom config
uctronics-display -c /path/to/custom.conf

# Show help
uctronics-display -h
```

Press **Ctrl+C** to exit gracefully.

### Systemd Service Management

```bash
# Check status
systemctl status uctronics-display.service

# Start/stop service
sudo systemctl start uctronics-display.service
sudo systemctl stop uctronics-display.service

# Enable/disable auto-start on boot
sudo systemctl enable uctronics-display.service
sudo systemctl disable uctronics-display.service

# View logs
journalctl -u uctronics-display.service
```

### Quick Service Control (Makefile)

```bash
# Enable and start service
sudo make enable-service

# Stop and disable service
sudo make disable-service
```

## Configuration

### Configuration File Search Order

When `-c` option is not specified:

1. `/etc/uctronics-display.conf` (system-wide)
2. Built-in defaults

### Configuration Options

| Option | Values | Description |
|--------|--------|-------------|
| `i2c_device` | `/dev/i2c-*` | I2C device path |
| `temperature_type` | `celsius`, `fahrenheit` | Temperature units |
| `network_interface` | `eth0`, `end0`, `wlan0`, etc. | Network interface to query |
| `ip_switch` | `open`, `close` | Show IP address or custom text |
| `custom_display` | Any text (max 31 chars) | Custom text when IP display is closed |

### Platform-Specific Settings

**Rock 3C (Fedora):**
- I2C: `/dev/i2c-3`
- Network: `end0` (Ethernet), `wlan0` (WiFi)

**Raspberry Pi:**
- I2C: `/dev/i2c-1`
- Network: `eth0` (Ethernet), `wlan0` (WiFi)

Check your network interface:
```bash
ip addr show
```

## Display Modes

The display cycles through 3 modes every 3 seconds:

1. **Mode 0:** CPU temperature, load, IP address (or custom text)
2. **Mode 1:** RAM usage (free / total)
3. **Mode 2:** SD card usage (used / total)

## Uninstallation

```bash
cd U6143_ssd1306/C
sudo make uninstall
```

This will:
- Stop and disable the service
- Remove the service and preset files
- Remove the binary and configuration

## Building from Source

### Build

```bash
make clean
make
```

### Install to Custom Location

```bash
sudo make install PREFIX=/usr/local
```

### Create Installation Package

```bash
make install DESTDIR=/tmp/package-root
```

## Makefile Targets

| Target | Description |
|--------|-------------|
| `make` | Build the binary |
| `make clean` | Remove build artifacts |
| `make install` | Install binary, config, and service file |
| `make uninstall` | Uninstall everything |
| `make install-service` | Install service file only |
| `make uninstall-service` | Remove service only |
| `make enable-service` | Enable and start the service |
| `make disable-service` | Stop and disable the service |
| `make test` | Run automated tests |
| `make test-clean` | Remove test artifacts |
| `make release` | Create release tarball (e.g., uctronics-display-1.0.0.tar.gz) |

## Testing

The project includes automated tests for configuration parsing and other non-hardware components.

### Run Tests

```bash
make test
```

All tests should pass:
```
Running test suite: Configuration Parser Tests
...
✓ All tests passed!
```

See [tests/README.md](tests/README.md) for detailed testing documentation.

## Features

- **Graceful Shutdown:** Responds to SIGINT/SIGTERM signals, clears display on exit
- **Error Handling:** Comprehensive error reporting with errno messages
- **Flexible Configuration:** INI-style config file with multiple search locations
- **Auto-Restart:** Systemd service automatically restarts on failure
- **Platform Independent:** Works on Raspberry Pi, Rock 3C, and other SBCs
- **Tested:** Automated tests for configuration parser
- **Logging:** Automatic logging to syslog when running as service, console output otherwise

## Troubleshooting

### Display Not Working

**Check I2C device:**
```bash
ls -l /dev/i2c-*
```

**Test I2C connection:**
```bash
sudo i2cdetect -y 3  # Replace 3 with your I2C bus number
```
You should see device at address `0x3c`.

**Check permissions:**
```bash
# Add user to i2c group (then log out/in)
sudo usermod -a -G i2c $USER

# Or run as root
sudo uctronics-display
```

### Service Not Starting

**Check service status:**
```bash
systemctl status uctronics-display.service
```

**View detailed logs:**
```bash
# View last 50 lines
journalctl -u uctronics-display.service -n 50

# Follow logs in real-time
journalctl -u uctronics-display.service -f

# View all logs since boot
journalctl -u uctronics-display.service -b
```

**Check config file:**
```bash
cat /etc/uctronics-display.conf
```

### IP Address Shows 0.0.0.0

- Verify network interface name: `ip addr show`
- Update `network_interface` in `/etc/uctronics-display.conf`
- Restart service: `sudo systemctl restart uctronics-display.service`

## Changes from Previous Versions

- **Removed:** `setup_display_service.sh` script (replaced with Makefile targets)
- **Binary Renamed:** `display` → `uctronics-display`
- **Simplified Config Search:** No longer searches for `display.conf` in executable directory
- **Integrated Service Installation:** Systemd service now installed automatically with `make install`
- **Signal Handling:** Added graceful shutdown on Ctrl+C or SIGTERM
- **Improved Error Handling:** Better error messages with system error descriptions

## License

See main repository for license information.

## Support

For issues and questions, visit: https://github.com/UCTRONICS/U6143_ssd1306

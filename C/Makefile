VERSION		= 1.0.0
Target		= uctronics-display
CXX         	= gcc
CFLAGS		= -Wall -g
ODIR		= obj
SRC		=$(wildcard *.c)
DIR		=$(notdir $(SRC))
OBJ 		= $(patsubst %.c, %.o, $(DIR))

# Installation paths
PREFIX		?= /usr
DESTDIR		?=
BINDIR		= $(PREFIX)/bin
SYSCONFDIR	= /etc
SYSTEMDDIR	= $(PREFIX)/lib/systemd/system
PRESETDIR	= $(PREFIX)/lib/systemd/system-preset

# Service files
SERVICEFILE	= uctronics-display.service
PRESETFILE	= 90-uctronics-display.preset

$(Target): $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@ -rdynamic

%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean install uninstall install-service uninstall-service enable-service disable-service test test-clean release
clean: test-clean
	rm -rf *.o
	rm -rf $(Target)

install: $(Target) install-service
	install -D -m 0755 $(Target) $(DESTDIR)$(BINDIR)/uctronics-display
	install -D -m 0644 display.conf $(DESTDIR)$(SYSCONFDIR)/uctronics-display.conf
	@echo ""
	@echo "Installation complete!"
	@echo "To enable and start the service, run:"
	@echo "  sudo systemctl enable --now uctronics-display.service"
	@echo ""

install-service:
	install -D -m 0644 $(SERVICEFILE) $(DESTDIR)$(SYSTEMDDIR)/$(SERVICEFILE)
	install -D -m 0644 $(PRESETFILE) $(DESTDIR)$(PRESETDIR)/$(PRESETFILE)

uninstall: uninstall-service
	rm -f $(DESTDIR)$(BINDIR)/uctronics-display
	rm -f $(DESTDIR)$(SYSCONFDIR)/uctronics-display.conf
	@echo "Uninstallation complete."

uninstall-service:
	systemctl stop $(SERVICEFILE) 2>/dev/null || true
	systemctl disable $(SERVICEFILE) 2>/dev/null || true
	rm -f $(DESTDIR)$(SYSTEMDDIR)/$(SERVICEFILE)
	rm -f $(DESTDIR)$(PRESETDIR)/$(PRESETFILE)
	systemctl daemon-reload 2>/dev/null || true

enable-service:
	systemctl daemon-reload
	systemctl enable $(SERVICEFILE)
	systemctl start $(SERVICEFILE)
	@echo "Service enabled and started. Check status with: systemctl status $(SERVICEFILE)"

disable-service:
	systemctl stop $(SERVICEFILE)
	systemctl disable $(SERVICEFILE)
	@echo "Service stopped and disabled."

# Test targets
test: tests/test_config
	@echo "Running tests..."
	@./tests/test_config

tests/test_config: tests/test_config.c config.o
	@mkdir -p tests
	$(CXX) $(CFLAGS) tests/test_config.c config.o -o tests/test_config

test-clean:
	rm -f tests/test_config
	rm -f tests/*.o

# Create release tarball
RELEASE_NAME = $(Target)-$(VERSION)
RELEASE_DIR = /tmp/$(RELEASE_NAME)

release: clean
	@echo "Creating release tarball: $(RELEASE_NAME).tar.gz"
	@rm -rf $(RELEASE_DIR)
	@mkdir -p $(RELEASE_DIR)
	@mkdir -p $(RELEASE_DIR)/tests
	@cp *.c *.h Makefile $(RELEASE_DIR)/
	@cp display.conf $(SERVICEFILE) $(PRESETFILE) README.md $(RELEASE_DIR)/
	@cp tests/*.c tests/*.h tests/*.conf tests/README.md $(RELEASE_DIR)/tests/
	@tar -czf $(RELEASE_NAME).tar.gz -C /tmp $(RELEASE_NAME)
	@rm -rf $(RELEASE_DIR)
	@echo "Release tarball created: $(RELEASE_NAME).tar.gz"
	@echo "Contents:"
	@tar -tzf $(RELEASE_NAME).tar.gz | head -20

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

$(Target): $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@ -rdynamic

%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean install uninstall
clean:
	rm -rf *.o
	rm -rf $(Target)

install: $(Target)
	install -D -m 0755 $(Target) $(DESTDIR)$(BINDIR)/uctronics-display
	install -D -m 0644 display.conf $(DESTDIR)$(SYSCONFDIR)/uctronics-display.conf

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/uctronics-display
	rm -f $(DESTDIR)$(SYSCONFDIR)/uctronics-display.conf

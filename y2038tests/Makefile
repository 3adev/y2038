# Makefile for custom apps to include in the image
# Pseudo targets:
#
# - clean: remove all build aterfacts
#
# - all: build the init executable
#
# - run: try and cross-run init on host through QEMU
#
# Default is 'all'

.PHONY: all install clean

CFLAGS += --sysroot=$(SYSROOT) -isystem =/usr/include -g
LDCFLAGS += --sysroot=$(SYSROOT) -isystem =/usr/include -L=/lib -L=/usr/lib -g

all: test_clock_gettime_settime test_clock_gettime_settime64

test_clock_gettime_settime: test_clock_gettime_settime.o
	$(CC) $(LDCFLAGS) -o test_clock_gettime_settime test_clock_gettime_settime.o

test_clock_gettime_settime.o: test_clock_gettime_settime.c
	$(CC) $(CFLAGS) -c -o test_clock_gettime_settime.o test_clock_gettime_settime.c

test_clock_gettime_settime64: test_clock_gettime_settime64.o
	$(CC) $(LDCFLAGS) -o test_clock_gettime_settime64 test_clock_gettime_settime64.o
	
test_clock_gettime_settime64.o: test_clock_gettime_settime.c
	$(CC) $(CFLAGS) -c -o test_clock_gettime_settime64.o -D_TIME_BITS=64 test_clock_gettime_settime.c

install: all
	install -m 755 test_clock_gettime_settime $(INSTALL_DIR)
	install -m 755 test_clock_gettime_settime64 $(INSTALL_DIR)

clean:
	rm -f test_clock_gettime_settime test_clock_gettime_settime64
	rm -f test_clock_gettime_settime.o test_clock_gettime_settime64.o

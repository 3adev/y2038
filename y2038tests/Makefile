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

CFLAGS += -g -I $(SYSROOT)/usr/include
LDFLAGS += -g

all: test_clock_gettime_settime test_clock_gettime_settime64

test_clock_gettime_settime: test_clock_gettime_settime.o

# use static lib for now as dynamic lib does not work
test_clock_gettime_settime64: test_clock_gettime_settime64.o $(SYSROOT)/usr/lib/libc.a

test_clock_gettime_settime.o: test_clock_gettime_settime.c

test_clock_gettime_settime64.o: test_clock_gettime_settime.c
	$(CC) -c -o test_clock_gettime_settime64.o $(CFLAGS) -D_TIME_BITS=64 test_clock_gettime_settime.c

install: all
	install -m 755 test_clock_gettime_settime $(INSTALL_DIR)
	install -m 755 test_clock_gettime_settime64 $(INSTALL_DIR)

clean:
	rm -f test_clock_gettime_settime test_clock_gettime_settime64
	rm -f test_clock_gettime_settime.o test_clock_gettime_settime64.o

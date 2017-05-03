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

all: test_y2038 test_n2038

test_y2038: $(patsubst %.c,%.64.o,$(wildcard *.c)) 
	@echo "LD64  $^"
	@$(CC) $(LDCFLAGS) -o test_y2038 $^ -lrt

test_n2038: $(patsubst %.c,%.32.o,$(wildcard *.c))
	@echo "LD32  $^"
	@$(CC) $(LDCFLAGS) -o test_n2038 $^ -lrt

%.64.o: %.c %.64.d
	@echo "CC64  $<"
	@$(CC) $(LDCFLAGS) -c -o $@ -D_TIME_BITS=64 $<
	
%.64.d: %.c
	@$(CC) $(LDCFLAGS) -M -o $@ -D_TIME_BITS=64 $<
	
%.32.o: %.c %.32.d
	@echo "CC32  $<"
	@$(CC) $(LDCFLAGS) -c -o $@ $<

%.32.d: %.c
	@$(CC) $(LDCFLAGS) -M -o $@ $<
	
install: all
	install -m 755 test_n2038 $(INSTALL_DIR)
	install -m 755 test_y2038 $(INSTALL_DIR)

clean:
	rm -f test_[ny]2038 *.o *.d

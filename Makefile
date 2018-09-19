#---------------------------------------------------------------------------
# DEFINITIONS
#
# Only this part of the makefile need to be changed, and you do not
# actually need to change the file: all you need is to specify values
# for environment or make variables listed below, e.g
#
#   KERNEL=linux/arch/x86/boot/uImage <other vars> make <options> <target>
#
# or 
#
#   make <options> KERNEL=linux/arch/x86/boot/uImage <other vars> <target>
#---------------------------------------------------------------------------

# Main directories. Make them available to sub-Makefiles if needed

export KERNEL_SOURCE_DIR       := $(PWD)/linux/src
export KERNEL_BUILD_DIR_Y2038  := $(PWD)/linux/build-y2038
export KERNEL_BUILD_DIR_N2038  := $(PWD)/linux/build-n2038
export GLIBC_SOURCE_DIR        := $(PWD)/glibc/src
export GLIBC_BUILD_DIR         := $(PWD)/glibc/build
export ROOTFS_DIR_Y2038        := $(PWD)/rootfs-y2038
export ROOTFS_DIR_N2038        := $(PWD)/rootfs-n2038
export BUSYBOX_DIR             := $(PWD)/busybox
export Y2038TESTS_DIR          := $(PWD)/y2038tests
export KERNEL_HDR_DIR_Y2038    := $(ROOTFS_DIR_Y2038)/usr
export KERNEL_HDR_DIR_N2038    := $(ROOTFS_DIR_N2038)/usr

# This one is used in GLIBC rules below

TARGET_ARCH = arm-linux-gnueabi

# Cross compilation helpers

export ARCH=arm
export CROSS_COMPILE=$(TARGET_ARCH)-

export CC=$(CROSS_COMPILE)gcc
export CXX=arm-linux-gnueabi-g++
export LD=$(CROSS_COMPILE)ld

# KERNEL

KERNEL_GIT = git://git.kernel.org/pub/scm/linux/kernel/git/arnd/playground.git
KERNEL_COMMIT = y2038-4.8
KERNEL_DEVICE_TREE  = vexpress-v2p-ca15-tc1
KERNEL_COMMAND_LINE = 'root=/dev/ram  rw quiet console=ttyAMA0'

# GLIBC

GLIBC_GIT = git://sourceware.org/git/glibc.git
GLIBC_COMMIT = aaribaud/y2038
GLIBC_HOST = $(TARGET_ARCH)

# BUSYBOX

BUSYBOX_GIT = git://busybox.net/busybox.git
BUSYBOX_COMMIT = 1_26_1

# QEMU

export QEMU_ARCH=arm
QEMU_ARGS = -M vexpress-a15 -m 2048

#---------------------------------------------------------------------------
# GLOBAL TARGETS
#---------------------------------------------------------------------------

# pseudo targets to (re)make each component.
.PHONY: all clean clean-all qemu

# default target creates initramfs but does not run it.
all: initrd-y2038.gz

# clean only cleans the rootfs and initramfs 
clean:
	git clean -xfd rootfs-n2038
	git clean -xfd rootfs-y2038
	rm -f initramfs-n2038
	rm -f initramfs-y2038
	rm -f initrd-y2038.gz
	rm -f initrd-n2038.gz

# clean-all cleans and asks for subprojects to clean
# NOTE: The GLIBC clean target is *very slow*.
#       It's just faster to simply delete the build dir.
clean-all: clean
	$(MAKE) -C $(KERNEL_SOURCE_DIR) O=$(KERNEL_BUILD_DIR_Y2038) clean
	$(MAKE) -C $(KERNEL_SOURCE_DIR) O=$(KERNEL_BUILD_DIR_N2038) clean
	rm -rf $(GLIBC_BUILD_DIR)
	$(MAKE) -C $(BUSYBOX_DIR) clean
	$(MAKE) -C $(Y2038TESTS_DIR) clean

#---------------------------------------------------------------------------
# KERNEL
#---------------------------------------------------------------------------

# Target (sub)component files

KERNEL_IMAGE_Y2038 = $(KERNEL_BUILD_DIR_Y2038)/arch/$(ARCH)/boot/zImage
KERNEL_DTB_Y2038   = $(KERNEL_BUILD_DIR_Y2038)/arch/$(ARCH)/boot/dts/$(KERNEL_DEVICE_TREE).dtb
KERNEL_HDR_Y2038   = $(KERNEL_HDR_DIR_Y2038)/include/linux/version.h

KERNEL_IMAGE_N2038 = $(KERNEL_BUILD_DIR_N2038)/arch/$(ARCH)/boot/zImage
KERNEL_DTB_N2038   = $(KERNEL_BUILD_DIR_N2038)/arch/$(ARCH)/boot/dts/$(KERNEL_DEVICE_TREE).dtb
KERNEL_HDR_N2038   = $(KERNEL_HDR_DIR_N2038)/include/linux/version.h

# How to fetch the component
$(KERNEL_SOURCE_DIR):
	mkdir -p $(KERNEL_SOURCE_DIR)
	git clone $(KERNEL_GIT) $(KERNEL_SOURCE_DIR)
	cd $(KERNEL_SOURCE_DIR) && git checkout $(KERNEL_COMMIT)

# How to configure the component

$(KERNEL_BUILD_DIR_Y2038)/.config: configs/kernel-$(ARCH)-defconfig-y2038 | $(KERNEL_SOURCE_DIR)
	mkdir -p $(KERNEL_BUILD_DIR_Y2038)
	cp -f configs/kernel-$(ARCH)-defconfig-y2038 $(KERNEL_BUILD_DIR_Y2038)/.config
	$(MAKE) -C $(KERNEL_SOURCE_DIR) O=$(KERNEL_BUILD_DIR_Y2038) olddefconfig

$(KERNEL_BUILD_DIR_N2038)/.config: configs/kernel-$(ARCH)-defconfig-n2038 | $(KERNEL_SOURCE_DIR)
	mkdir -p $(KERNEL_BUILD_DIR_N2038)
	cp -f configs/kernel-$(ARCH)-defconfig-n2038 $(KERNEL_BUILD_DIR_N2038)/.config
	$(MAKE) -C $(KERNEL_SOURCE_DIR) O=$(KERNEL_BUILD_DIR_N2038) olddefconfig

# How to build the image for the image

$(KERNEL_IMAGE_Y2038): $(KERNEL_BUILD_DIR_Y2038)/.config
	$(MAKE) -j -C $(KERNEL_SOURCE_DIR) O=$(KERNEL_BUILD_DIR_Y2038) zImage

$(KERNEL_IMAGE_N2038): $(KERNEL_BUILD_DIR_N2038)/.config
	$(MAKE) -j -C $(KERNEL_SOURCE_DIR) O=$(KERNEL_BUILD_DIR_N2038) zImage

# How to build the device tree for the image

ifneq ($(KERNEL_DTB_Y2038),)
$(KERNEL_DTB_Y2038): $(KERNEL_BUILD_DIR_Y2038)/.config
	$(MAKE) -C $(KERNEL_SOURCE_DIR) O=$(KERNEL_BUILD_DIR_Y2038) dtbs
endif

ifneq ($(KERNEL_DTB_N2038),)
$(KERNEL_DTB_N2038): $(KERNEL_BUILD_DIR_N2038)/.config
	$(MAKE) -C $(KERNEL_SOURCE_DIR) O=$(KERNEL_BUILD_DIR_N2038) dtbs
endif

# pseudo targets to build all only the kernel, only the dtbs, all of
# linux
.PHONY: kernel-y2038 dtb-y2038 linux-y2038
.PHONY: kernel-n2038 dtb-n2038 linux-n2038
.PHONY: kernel dtb linux

# Helper to build the kernel

kernel-y2038: $(KERNEL_IMAGE_Y2038)
dtb-y2038: $(KERNEL_DTB_Y2038)
linux-y2038: kernel-y2038 dtb-y2038

kernel-n2038: $(KERNEL_IMAGE_N2038)
dtb-n2038: $(KERNEL_DTB_N2038)
linux-n2038: kernel-n2038 dtb-n2038

linux: linux-y2038 linux-n2038

#---------------------------------------------------------------------------
# GLIBC
#
# GLIBC must be configured with --prefix=/usr because this path (and all
# those which derive from it) is where things will be at RUN time, and
# installed with DESTDIR=$(ROOTFS_DIR_Y2038) because that is where GLIBC files
# must be copied before it is turned into an initramfs.
# Also, kernel headers should be those of the kernel being built, hence
# the --with-headers configure option.
#---------------------------------------------------------------------------

# Target files to build (representative, not the only ones)
GLIBC_LIBS = $(GLIBC_BUILD_DIR)/libc.a $(GLIBC_BUILD_DIR)/libc.so

# How to fetch the component
$(GLIBC_SOURCE_DIR)/configure:
	mkdir -p $(GLIBC_SOURCE_DIR)
	git clone $(GLIBC_GIT) $(GLIBC_SOURCE_DIR)
	cd $(GLIBC_SOURCE_DIR) && git checkout -f $(GLIBC_COMMIT)

# How to configure the component
$(GLIBC_BUILD_DIR)/Makefile: $(GLIBC_SOURCE_DIR)/configure  $(KERNEL_HDR_Y2038)
	mkdir -p $(GLIBC_BUILD_DIR)
	cd $(GLIBC_BUILD_DIR) && $(GLIBC_SOURCE_DIR)/configure --prefix=/usr --host=$(GLIBC_HOST) --with-headers=$(KERNEL_HDR_DIR_Y2038)/include

# How to build the component
$(GLIBC_LIBS): $(GLIBC_BUILD_DIR)/Makefile
	$(MAKE) -C $(GLIBC_BUILD_DIR)

.PHONY: glibc-configure glibc glibc-install

glibc-configure: $(GLIBC_BUILD_DIR)/Makefile

glibc: $(GLIBC_LIBS)

#---------------------------------------------------------------------------
# BUSYBOX
#---------------------------------------------------------------------------

BUSYBOX_EXE = $(BUSYBOX_DIR)/busybox

$(BUSYBOX_DIR):
	mkdir -p $(BUSYBOX_DIR)
	git clone $(BUSYBOX_GIT) $(BUSYBOX_DIR)
	cd $(BUSYBOX_DIR) && git checkout -f $(BUSYBOX_COMMIT)

$(BUSYBOX_DIR)/.config: configs/busybox-$(ARCH)-defconfig | $(BUSYBOX_DIR)
	cp -f configs/busybox-$(ARCH)-defconfig $(BUSYBOX_DIR)/.config
	sed -i -e "s!CONFIG_SYSROOT=\"\"!CONFIG_SYSROOT=\"$(ROOTFS_DIR_Y2038)\"!" $(BUSYBOX_DIR)/.config

$(BUSYBOX_EXE): $(BUSYBOX_DIR)/.config
	$(MAKE) -C $(BUSYBOX_DIR) busybox

.PHONY: busybox-install

busybox: $(BUSYBOX_EXE)

#---------------------------------------------------------------------------
# Y2038 TESTS
#---------------------------------------------------------------------------

.PHONY: y2038tests

y2038tests: $(ROOTFS_LIBS_Y2038)
	$(MAKE) -C $(Y2038TESTS_DIR) SYSROOT=$(ROOTFS_DIR_Y2038) all

#---------------------------------------------------------------------------
# ROOTFS
#---------------------------------------------------------------------------

# how to install the kernel headers in the rootfs

$(KERNEL_HDR_Y2038): $(KERNEL_IMAGE_Y2038)
	mkdir -p $(KERNEL_HDR_DIR_Y2038) && $(MAKE) -C $(KERNEL_SOURCE_DIR) O=$(KERNEL_BUILD_DIR_Y2038) INSTALL_HDR_PATH=$(KERNEL_HDR_DIR_Y2038) headers_install

# how to install GLIBC in the rootfs

ROOTFS_LIBS_Y2038 = $(ROOTFS_DIR_Y2038)/usr/lib/libc.a $(ROOTFS_DIR_Y2038)/usr/lib/libc.so

$(ROOTFS_LIBS_Y2038): $(GLIBC_LIBS)
	$(MAKE) -C $(GLIBC_BUILD_DIR) DESTDIR=$(ROOTFS_DIR_Y2038) install

ROOTFS_LIBS_N2038 = $(ROOTFS_DIR_N2038)/usr/lib/libc.a $(ROOTFS_DIR_N2038)/usr/lib/libc.so

$(ROOTFS_LIBS_N2038): $(GLIBC_LIBS)
	$(MAKE) -C $(GLIBC_BUILD_DIR) DESTDIR=$(ROOTFS_DIR_N2038) install

# how to install busybox (and set it up as init) in the rootfs

ROOTFS_INIT_Y2038 = $(ROOTFS_DIR_Y2038)/init
ROOTFS_INIT_N2038 = $(ROOTFS_DIR_N2038)/init

$(ROOTFS_INIT_Y2038): $(BUSYBOX_EXE)
	$(MAKE) -C $(BUSYBOX_DIR) CONFIG_PREFIX=$(ROOTFS_DIR_Y2038) install
	cp -f $(BUSYBOX_DIR)/busybox $(ROOTFS_INIT_Y2038)


$(ROOTFS_INIT_N2038): $(BUSYBOX_EXE)
	$(MAKE) -C $(BUSYBOX_DIR) CONFIG_PREFIX=$(ROOTFS_DIR_N2038) install
	cp -f $(BUSYBOX_DIR)/busybox $(ROOTFS_INIT_N2038)

# how to install busybox

rootfs-y2038: $(ROOTFS_LIBS_Y2038) $(ROOTFS_INIT_Y2038) y2038tests-install-y2038
	echo /lib >> $(ROOTFS_DIR_Y2038)/etc/ld.so.conf
	echo /usr/lib >> $(ROOTFS_DIR_Y2038)/etc/ld.so.conf

rootfs-n2038: $(ROOTFS_LIBS_N2038) $(ROOTFS_INIT_N2038) y2038tests-install-n2038
	echo /lib >> $(ROOTFS_DIR_N2038)/etc/ld.so.conf
	echo /usr/lib >> $(ROOTFS_DIR_N2038)/etc/ld.so.conf

# pseudo targets to (re)build parts or all the whole rootfs
.PHONY: linux-install-y2038 glibc-install-y2038 busybox-install-y2038 y2038tests-install-y2038 rootfs-y2038
.PHONY: linux-install-n2038 glibc-install-n2038 busybox-install-n2038 y2038tests-install-n2038 rootfs-n2038

linux-install-y2038: linux-y2038 $(KERNEL_HDR_Y2038)

glibc-install-y2038: $(ROOTFS_LIBS_Y2038)

busybox-install-y2038: $(ROOTFS_INIT_Y2038)

y2038tests-install-y2038: y2038tests
	$(MAKE) -C $(Y2038TESTS_DIR) install INSTALL_DIR=$(ROOTFS_DIR_Y2038)/usr/bin SYSROOT=$(ROOTFS_DIR_Y2038)

linux-install-n2038: linux-n2038 $(KERNEL_HDR_N2038)

glibc-install-n2038: $(ROOTFS_LIBS_N2038)

busybox-install-n2038: $(ROOTFS_INIT_N2038)

y2038tests-install-n2038: y2038tests
	$(MAKE) -C $(Y2038TESTS_DIR) install INSTALL_DIR=$(ROOTFS_DIR_N2038)/usr/bin SYSROOT=$(ROOTFS_DIR_N2038)

#---------------------------------------------------------------------------
# INITRAMFS
#---------------------------------------------------------------------------

initramfs-y2038: rootfs-y2038
	(cd $(ROOTFS_DIR_Y2038) && find | cpio -o --format=newc) > initramfs-y2038

initramfs-n2038: rootfs-n2038
	(cd $(ROOTFS_DIR_N2038) && find | cpio -o --format=newc) > initramfs-n2038

initrd-y2038.gz: initramfs-y2038
	gzip < initramfs-y2038 > initrd-y2038.gz

initrd-n2038.gz: initramfs-n2038
	gzip < initramfs-n2038 > initrd-n2038.gz

#---------------------------------------------------------------------------
# QEMU
# Declared last here as it depends on previous targets defined above
#---------------------------------------------------------------------------

ifneq ($(KERNEL_DTB_Y2038),)
QEMU_DTB_Y2038 = -dtb $(KERNEL_DTB_Y2038)
endif

ifneq ($(KERNEL_DTB_N2038),)
QEMU_DTB_N2038 = -dtb $(KERNEL_DTB_N2038)
endif

# run the image under QEMU
qemu-y2038: $(KERNEL_IMAGE_Y2038) $(KERNEL_DTB_Y2038) initrd-y2038.gz
	@echo "*******************************************"
	@echo "***                                     ***"
	@echo "***   NOW RUNNING Y2038 IMAGE IN QEMU   ***"
	@echo "***                                     ***"
	@echo "***   To exit, hit Ctrl-A, then X       ***"
	@echo "***                                     ***"
	@echo "*******************************************"
	qemu-system-$(QEMU_ARCH) -nographic $(QEMU_ARGS) -kernel $(KERNEL_IMAGE_Y2038) $(QEMU_DTB_Y2038) -initrd initrd-y2038.gz -append $(KERNEL_COMMAND_LINE)

# run the image under QEMU
qemu-y2038-d: $(KERNEL_IMAGE_Y2038) $(KERNEL_DTB_Y2038) initrd-y2038.gz
	@echo "*******************************************"
	@echo "***                                     ***"
	@echo "***   NOW RUNNING Y2038 IMAGE IN QEMU   ***"
	@echo "***             (DEBUG)                 ***"
	@echo "***   To exit, hit Ctrl-A, then X       ***"
	@echo "***                                     ***"
	@echo "*******************************************"
	qemu-system-$(QEMU_ARCH) -nographic $(QEMU_ARGS) -kernel $(KERNEL_IMAGE_Y2038) $(QEMU_DTB_Y2038) -initrd initrd-y2038.gz -append $(KERNEL_COMMAND_LINE) -s

qemu-n2038: $(KERNEL_IMAGE_N2038) $(KERNEL_DTB_N2038) initrd-n2038.gz
	@echo "*******************************************"
	@echo "***                                     ***"
	@echo "***   NOW RUNNING N2038 IMAGE IN QEMU   ***"
	@echo "***                                     ***"
	@echo "***   To exit, hit Ctrl-A, then X       ***"
	@echo "***                                     ***"
	@echo "*******************************************"
	qemu-system-$(QEMU_ARCH) -nographic $(QEMU_ARGS) -kernel $(KERNEL_IMAGE_N2038) $(QEMU_DTB_N2038) -initrd initrd-n2038.gz -append $(KERNEL_COMMAND_LINE)

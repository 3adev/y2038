# Y2038 experimental image

# Introduction

This repository provides the means to build a (hopefully) Y2038-safe
Linux system composed of:

* the Linux kernel with Arnd Bergmann's Y2038 patch series, available at
  `git://git.kernel.org/pub/scm/linux/kernel/git/arnd/playground.git` in
  branch `y2038-4.8`;

* the GLIBC libary with in-progress Y2038 support as available from
  `git://sourceware.org/git/glibc.git` in branch `aaribaud/y2038-2.25`;

* an unmodified busybox build taken from `git://busybox.net/busybox.git`
  on branch `master`;

* a minimal root file system skeleton;

* various Y2038 test programs located in subdirectory `y2038tests/`;

* a Makefile which coordinates building the kernel, GLIBC, busoybox,
  root file system and test programs into a single system image which
  can run under QEMU.

There are actually two builds of the kernel, one with Y2038 support and
one without, and two images built accordingly. This is done so that the
Y2038-enabled GLIBC can be run over both kernels and the results compared.

Various symbols in the Makefile have a '_Y2038' or '-y2038' suffix for
the Y2038-aware version (e.g. patched kernel) and a '_N2038' or '-n2038'
for the non-Y2038-aware version (e.g. unpatched kernel).

In the following, `-[yn]2038` means either `-y2038`  or `-n2038`.

# How to use it

cd into the repository root then run `make qemu`. This will (slowly)
build a Y2038 kernel, a Y2038 GLIBC, busybox, the root image, and in the
end, qemu.

There are pseudo targets to (re)build specific parts of the image
(see `Makefile` for details):

* `kernel-[yn]2038` builds the kernel image
* `dtbs-[yn]2038` builds the kernel device trees
* `linux-[yn]2038` builds all of the kernel, `linux-install` installs it in rootfs
* `glibc` builds the GLIBC library, `glibc-install` installs it in rootfs
* `busybox` builds the busybox library, `busybox-install` installs it in rootfs
* `y2038tests` builds application test code for Y2038 test cases
* `rootfs-[yn]2038` builds the rootfs itself (includes all of the installs above)
* `qemu-[yn]2038` runs the image under QEMU

Within the image, test applications can be run, and will use the kernel
and GLIBC of the image of course; they are stored in `/usr/bin` and are
called `test_y2038` (built  with `-DTIME_BITS=64` and thus expecting to
use Y2038-proof APIs) and `test_n2038` (built without `-DTIMEBITS=64` and
thus expecting to use the legacy, non-Y2038-proof, APIs).

Each of these test applications, when run, prints one line per test,
strating with with either 'ok' or 'FAILED'. Tests are run in the same
order for both test_y2038 and test_n2038.  

# How to tune it

Tunable variables are all in the first part of the Makefile ("DEFINITIONS").
The following variables can be modified:

* `TARGET_ARCH` is the prefix of the (cross)compiler to use.
* `ARCH` to set the architecture for all builds. Default is `arm`.

* `KERNEL_GIT` and `KERNEL_COMMIT` determine which repository and
  commit/branch/tag is used for fetching the kernel source code. This
  can be redefined to e.g. fetch a local kernel repository.

* `GLIBC_GIT` and `GLIBC_COMMIT` determine which repository and
  commit/branch/tag is used for fetching the GLIBC source code. Again,
  this can be redefined to e.g. fetch a local kernel repository.

* `BUSYBOX_GIT` and `BUSYBOX_COMMIT` determine which repository and
  commit/branch/tag is used for fetching the GLIBC source code. Again,
  this can be redefined to e.g. fetch a local kernel repository.

* `QEMU_MACHINE` determines which machine will be run using qemu. This
  must be a machine compatible with the chosen architecture.

Obviously, inconsistent redefinitions (e.g. `TARGET_ARCH` set for x86
with `ARCH` set for powerpc) will cause unpredictable but most certainly
bad results.

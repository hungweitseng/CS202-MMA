#
# By default, the build is done against the running linux kernel source.
# To build against a different kernel source tree, set SYSSRC:
#
#    make SYSSRC=/path/to/kernel/source
CONFIG_MODULE_SIG=n

ifdef SYSSRC
 KERNEL_SOURCES	 = $(SYSSRC) $(SYSSRC)/src
else
 KERNEL_UNAME	:= $(shell uname -r)
 KERNEL_SOURCES	 = /lib/modules/$(KERNEL_UNAME)/build
endif

# Use make M=dir to specify directory of external module to build
# Old syntax make ... SUBDIRS=$PWD is still supported
# Setting the environment variable KBUILD_EXTMOD take precedence
ifdef SUBDIRS
  KBUILD_EXTMOD ?= $(SUBDIRS)
endif
ifeq ("$(origin M)", "command line")
  KBUILD_EXTMOD := $(M)
endif

default: modules
.PHONY: default
install: modules_install
	cp blockmma-blacklist.conf /etc/modprobe.d
#	cp 80-blockmma.rules /etc/udev/rules.d
	mkdir -p /usr/local/include/blockmma/
	cp include/blockmma.h /usr/local/include/blockmma/
	update-initramfs -u
#	cp blockmma.ko /lib/modules/$(KERNEL_UNAME)/

.PHONY: install


.PHONY:


%::
	$(MAKE) -C $(KERNEL_SOURCES) \
        M=$$PWD $@

# Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved


#############################################################
# Platform definitions
#############################################################

BOARD ?= X300
ifeq ($(BOARD),X300)
    ARCH := rv32
    RISCV_ARCH := $(ARCH)imac
    RISCV_ABI := ilp32
else
    $(error Unsupported board $(BOARD))
endif


#############################################################
# Arguments/variables available to all submakes
#############################################################

export BOARD
export RISCV_ARCH
export RISCV_ABI


#############################################################
# Toolchain definitions
#############################################################

ifndef RISCV
$(error RISCV not set)
endif

export CROSS_COMPILE := $(abspath $(RISCV))/bin/riscv64-unknown-elf-
export CC      := $(CROSS_COMPILE)gcc
export OBJDUMP := $(CROSS_COMPILE)objdump
export OBJCOPY := $(CROSS_COMPILE)objcopy
export GDB     := $(CROSS_COMPILE)gdb
export AR      := $(CROSS_COMPILE)ar
export SIZE    := $(CROSS_COMPILE)size

#############################################################
# Rules for building multizone
#############################################################

.PHONY: all 
all: clean
	$(MAKE) -C zone1
	$(MAKE) -C zone2
	$(MAKE) -C zone3
	$(MAKE) -C zone3.1
	$(MAKE) -C zone4
	$(MAKE) -C zone4.1
	$(MAKE) -C zone4.2
	$(MAKE) -C bsp/$(BOARD)/boot
	
	java -jar ./ext/multizone/multizone.jar \
		--arch $(BOARD) \
		--config bsp/$(BOARD)/multizone.cfg \
		--boot bsp/$(BOARD)/boot/boot.hex \
		zone1/zone1.hex \
		zone2/zone2.hex \
		zone3/zone3.hex \
		zone4/zone4.hex

.PHONY: clean
clean:
	$(MAKE) -C zone1 clean
	$(MAKE) -C zone2 clean
	$(MAKE) -C zone3 clean
	$(MAKE) -C zone3.1 clean
	$(MAKE) -C zone4 clean
	$(MAKE) -C zone4.1 clean
	$(MAKE) -C zone4.2 clean
	$(MAKE) -C bsp/$(BOARD)/boot clean
	rm -f multizone.hex multizone.bin

#############################################################
# Load to flash
#############################################################

ifndef OPENOCD
$(error OPENOCD not set)
endif

OPENOCD := $(abspath $(OPENOCD))/bin/openocd

OPENOCDCFG ?= bsp/$(BOARD)/openocd.cfg
OPENOCDARGS += -f $(OPENOCDCFG)

GDB_PORT ?= 3333
GDB_LOAD_ARGS ?= --batch
GDB_LOAD_CMDS += -ex "set mem inaccessible-by-default off"
GDB_LOAD_CMDS += -ex "set remotetimeout 240"
GDB_LOAD_CMDS += -ex "set arch riscv:$(ARCH)"
GDB_LOAD_CMDS += -ex "target extended-remote localhost:$(GDB_PORT)"
GDB_LOAD_CMDS += -ex "monitor reset init"
GDB_LOAD_CMDS += -ex "monitor flash protect 0 64 last off"
GDB_LOAD_CMDS += -ex "load"
GDB_LOAD_CMDS += -ex "monitor resume"
GDB_LOAD_CMDS += -ex "monitor shutdown"
GDB_LOAD_CMDS += -ex "quit"

.PHONY: load

load:
	$(OPENOCD) $(OPENOCDARGS) & \
	$(GDB) multizone.hex $(GDB_LOAD_ARGS) $(GDB_LOAD_CMDS)
	

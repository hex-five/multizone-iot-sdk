# Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved

TARGET = zone2.elf

C_SRCS += main.c strtok.c
INCLUDES += -I ../ext/multizone
LINKER_SCRIPT := linker.lds

### printf
CFLAGS   += -DPRINTF_INCLUDE_CONFIG_H
INCLUDES += -I ../ext/printf
C_SRCS   += ../ext/printf/printf.c 

### bsp
BSP_BASE := ../bsp
PLATFORM_DIR := $(BSP_BASE)/$(BOARD)
NEWLIB_DIR := $(PLATFORM_DIR)/newlib

include $(NEWLIB_DIR)/newlib.mk

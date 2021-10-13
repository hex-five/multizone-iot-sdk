# Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved

TARGET = zone1.elf

C_SRCS += main.c mqtt_wrap.c queue.c 
INCLUDES += -I ../ext/multizone
LINKER_SCRIPT := linker.lds

### printf
CFLAGS   += -DPRINTF_INCLUDE_CONFIG_H
INCLUDES += -I ../ext/printf
C_SRCS   += ../ext/printf/printf.c 

### libs
INCLUDES += -I ../ext
INCLUDES += -I ../ext/lwip/src/include
INCLUDES += -I ../ext/mbedtls/include

### BSP
BSP_BASE := ../bsp
PLATFORM_DIR := $(BSP_BASE)/$(BOARD)
NEWLIB_DIR := $(PLATFORM_DIR)/newlib

include $(NEWLIB_DIR)/newlib.mk


# Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved

TARGET = zone1.elf

C_SRCS += main.c mqtt_wrap.c queue.c

INCLUDES += -I ../ext
INCLUDES += -I ../ext/multizone
INCLUDES += -I ../ext/lwip/src/include
INCLUDES += -I ../ext/mbedtls/include

LINKER_SCRIPT := linker.lds

### BSP
BSP_BASE := ../bsp
PLATFORM_DIR := $(BSP_BASE)/$(BOARD)
NEWLIB_DIR := $(PLATFORM_DIR)/newlib

include $(NEWLIB_DIR)/newlib.mk


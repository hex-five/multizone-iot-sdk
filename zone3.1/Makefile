# Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved

TARGET = zone3.elf

BSP_BASE := ../bsp
PLATFORM_DIR := $(BSP_BASE)/$(BOARD)
NEWLIB_DIR := $(PLATFORM_DIR)/newlib

LINKER_SCRIPT := linker.lds

C_SRCS += main.c

INCLUDES += -I ../ext/multizone

include $(NEWLIB_DIR)/newlib.mk

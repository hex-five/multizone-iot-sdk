# Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved

TARGET = zone4.elf

BSP_BASE := ../bsp
PLATFORM_DIR := $(BSP_BASE)/$(BOARD)
NEWLIB_DIR := $(PLATFORM_DIR)/newlib

LINKER_SCRIPT := linker.lds

C_SRCS += main.c owi_sequence.c

FREERTOS_SRC_DIR := ../ext/freertos
C_SRCS += \
	$(FREERTOS_SRC_DIR)/croutine.c \
	$(FREERTOS_SRC_DIR)/event_groups.c \
	$(FREERTOS_SRC_DIR)/list.c \
	$(FREERTOS_SRC_DIR)/queue.c \
	$(FREERTOS_SRC_DIR)/stream_buffer.c \
	$(FREERTOS_SRC_DIR)/tasks.c \
	$(FREERTOS_SRC_DIR)/timers.c \
	$(FREERTOS_SRC_DIR)/portable/MemMang/heap_4.c \

C_SRCS += $(FREERTOS_SRC_DIR)/portable/GCC/RISC-V/port.c
ASM_SRCS += $(FREERTOS_SRC_DIR)/portable/GCC/RISC-V/portASM.S

INCLUDES += \
	-I. \
	-I$(FREERTOS_SRC_DIR)/include \
	-I$(FREERTOS_SRC_DIR)/portable/GCC/RISC-V \
	-I$(FREERTOS_SRC_DIR)/portable/GCC/RISC-V/chip_specific_extensions/RV32I_CLINT_no_extensions \

INCLUDES += -I ../ext/multizone

include $(NEWLIB_DIR)/newlib.mk

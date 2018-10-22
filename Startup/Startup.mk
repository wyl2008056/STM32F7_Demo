SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

SRCS += $(SUB_DIR)/Source/main.c \
        $(SUB_DIR)/Source/syscalls.c \
        $(SUB_DIR)/Source/printf.c \
        $(SUB_DIR)/Source/stm32f7xx_it.c \
		$(SUB_DIR)/Source/bsp.c


ASMS += $(SUB_DIR)/Source/startup_stm32f746xx.S

INCLUDE += -I$(SUB_DIR)/Include/

#		$(SUB_DIR)/Source/stm32f7xx_hal_msp.c
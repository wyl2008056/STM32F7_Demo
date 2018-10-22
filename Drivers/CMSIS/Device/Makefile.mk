SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

SRCS 	+= $(SUB_DIR)/Source/system_stm32f7xx.c

INCLUDE += -I$(SUB_DIR)/Include/
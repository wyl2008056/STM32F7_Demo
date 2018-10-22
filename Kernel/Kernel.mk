SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

SRCS += $(SUB_DIR)/CMSIS_OS/cmsis_os2.c

INCLUDE += -I$(SUB_DIR)/CMSIS_OS/
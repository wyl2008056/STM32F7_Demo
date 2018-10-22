SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

SRCS += $(shell find $(SUB_DIR)/uCOS-III/Source -name '*.c') \
        $(SUB_DIR)/uCOS-III/Ports/os_cpu_c.c \
        $(SUB_DIR)/uC-CPU/Source/cpu_core.c \
        $(SUB_DIR)/uC-CPU/Ports/cpu_c.c \
        $(SUB_DIR)/uC-LIB/Source/lib_ascii.c \
        $(SUB_DIR)/uC-LIB/Source/lib_math.c \
        $(SUB_DIR)/uC-LIB/Source/lib_mem.c \
        $(SUB_DIR)/uC-LIB/Source/lib_str.c \
        $(SUB_DIR)/Config/os_app_hooks.c

ASMS += $(SUB_DIR)/uCOS-III/Ports/os_cpu_a.S \
        $(SUB_DIR)/uC-CPU/Ports/cpu_a.S \
        $(SUB_DIR)/uC-LIB/Ports/lib_mem_a.S

INCLUDE += -I$(SUB_DIR)/Config/ \
           -I$(SUB_DIR)/uCOS-III/Source/ \
           -I$(SUB_DIR)/uCOS-III/Ports/ \
           -I$(SUB_DIR)/uC-CPU/Source/ \
           -I$(SUB_DIR)/uC-LIB/Source/ \
           -I$(SUB_DIR)/uC-CPU/Ports/ \
           -I$(SUB_DIR)/uC-LIB/Ports/
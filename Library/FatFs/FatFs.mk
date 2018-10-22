SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

SRCS    += $(SUB_DIR)/Core/diskio.c \
           $(SUB_DIR)/Core/ff.c \
           $(SUB_DIR)/Core/ff_gen_drv.c \
           $(SUB_DIR)/Drivers/sd_diskio_dma.c \
           $(SUB_DIR)/Option/unicode.c

INCLUDE += -I$(SUB_DIR)/Include/ \
           -I$(SUB_DIR)/Drivers/
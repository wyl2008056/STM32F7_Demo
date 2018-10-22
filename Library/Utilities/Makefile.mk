SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

#SRCS 	+= $(shell find $(SUB_DIR)/Fonts -name '*.c')

##SRCS 	+= $(SUB_DIR)/Log/lcd_log.c

#INCLUDE += -I$(SUB_DIR)/Log/
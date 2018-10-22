SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

SRCS 	+= $(SUB_DIR)/Source/DateTime.cpp \
           $(SUB_DIR)/Source/gpio.cpp \
           $(SUB_DIR)/Source/Task.cpp \
		   $(SUB_DIR)/Source/Charger.cpp

INCLUDE += -I$(SUB_DIR)/Include/ \
           -I$(SUB_DIR)/Config/
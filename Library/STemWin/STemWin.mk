SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

SRCS += $(SUB_DIR)/Config/GUIConf.c \
        $(SUB_DIR)/Config/LCDConf.c \
        $(SUB_DIR)/Config/GUITouch.c \
        $(SUB_DIR)/OS/GUI_X_OS.c

LIBS += $(SUB_DIR)/Lib/STemWin540_CM7_OS_GCC.a

INCLUDE += -I$(SUB_DIR)/Include/ \
           -I$(SUB_DIR)/Config/
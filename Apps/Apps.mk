SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

SRCS 	+= $(SUB_DIR)/app_task_create.c \
           $(SUB_DIR)/app_led/app_led.cpp \
           $(SUB_DIR)/app_lcd/app_lcd.cpp \
           $(SUB_DIR)/app_key/app_key.c \
           $(SUB_DIR)/app_sd/app_sd.c \
           $(SUB_DIR)/app_ts/app_gui_ts.cpp \
           $(SUB_DIR)/app_state/app_state.c \
           $(SUB_DIR)/app_launcher/app_launcher.cpp \
           $(SUB_DIR)/app_launcher/events.cpp \
           $(SUB_DIR)/app_charger/app_charger.cpp \
           $(SUB_DIR)/app_charger/charger.c \
           $(SUB_DIR)/app_settings/app_settings.cpp
		   
SRCS 	+= $(shell find $(SUB_DIR)/app_launcher/Pictures -name '*.c')
SRCS 	+= $(shell find $(SUB_DIR)/app_charger/icons -name '*.c')

#INCLUDE += -I$(SUB_DIR)/
INCLUDE += -I$(SUB_DIR)/app_led/ \
           -I$(SUB_DIR)/app_lcd/ \
           -I$(SUB_DIR)/app_key/ \
           -I$(SUB_DIR)/app_sd/ \
           -I$(SUB_DIR)/app_ts/ \
           -I$(SUB_DIR)/app_state/ \
           -I$(SUB_DIR)/app_launcher/ \
           -I$(SUB_DIR)/app_charger/ \
           -I$(SUB_DIR)/app_settings/
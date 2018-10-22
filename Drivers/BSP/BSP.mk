SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

SRCS += $(SUB_DIR)/STM32746G-Discovery/stm32746g_discovery.c \
        $(SUB_DIR)/STM32746G-Discovery/stm32746g_discovery_audio.c \
        $(SUB_DIR)/STM32746G-Discovery/stm32746g_discovery_eeprom.c \
        $(SUB_DIR)/STM32746G-Discovery/stm32746g_discovery_lcd.c \
        $(SUB_DIR)/STM32746G-Discovery/stm32746g_discovery_qspi.c \
        $(SUB_DIR)/STM32746G-Discovery/stm32746g_discovery_sd.c \
        $(SUB_DIR)/STM32746G-Discovery/stm32746g_discovery_sdram.c \
        $(SUB_DIR)/STM32746G-Discovery/stm32746g_discovery_ts.c \
        $(SUB_DIR)/Components/ft5336/ft5336.c


INCLUDE += -I$(SUB_DIR)/STM32746G-Discovery/ \
           -I$(SUB_DIR)/Components/rk043fn48h/ \
           -I$(SUB_DIR)/Components/ft5336/
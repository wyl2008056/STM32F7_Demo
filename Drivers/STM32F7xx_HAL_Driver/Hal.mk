SUB_DIR:=$(shell dirname $(shell pwd)/$(lastword $(MAKEFILE_LIST)))

#SRCS 	+= $(shell find $(SUB_DIR)/Src -name '*.c')

SRCS += $(SUB_DIR)/Src/stm32f7xx_hal_ltdc.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_adc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_irda.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_cryp.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_dac.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_pcd.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_sai_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_dfsdm.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_i2c_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_dma.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_tim_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_rcc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_adc_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_dcmi.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_qspi.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_gpio.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_pwr.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_smartcard_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_mmc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_rtc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_jpeg.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_i2c.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_crc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_nand.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_utils.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_nor.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_flash_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_fmc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_adc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_usart.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_spi.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_hash.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_dma2d.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_i2c.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_tim.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_spdifrx.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_sram.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_crc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_lptim.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_dma.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_spi.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_rng.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_pwr_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_crc_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_pwr.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_flash.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_dma2d.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_dac.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_dcmi_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_dma_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_mdios.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_eth.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_rng.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_pcd_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_sdmmc.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_usart.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_rtc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_gpio.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_rcc.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_uart.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_cortex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_iwdg.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_dac_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_rcc_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_can.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_exti.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_hcd.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_wwdg.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_cryp_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_tim.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_smartcard.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_cec.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_rtc_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_i2s.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_smbus.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_ltdc_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_usb.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_hash_ex.c \
        $(SUB_DIR)/Src/stm32f7xx_hal.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_dsi.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_sd.c \
        $(SUB_DIR)/Src/stm32f7xx_ll_lptim.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_sai.c \
        $(SUB_DIR)/Src/stm32f7xx_hal_sdram.c \
        $(SUB_DIR)/config/stm32f7xx_hal_timebase_tim.c



INCLUDE += -I$(SUB_DIR)/Inc/ \
           -I$(SUB_DIR)/config/


#INCLUDE += -I$(SUB_DIR)/Inc/Legacy/

#        $(shell find $(SUB_DIR)/Src/Legacy -name '*.c')
#SRCS 	+= $(SUB_DIR)/config/stm32f7xx_hal_msp_template.c
#SRCS 	+= $(SUB_DIR)/config/stm32f7xx_hal_timebase_rtc_wakeup_template.c
#SRCS 	+= $(SUB_DIR)/config/stm32f7xx_hal_timebase_rtc_alarm_template.c
#SRCS 	+= $(SUB_DIR)/config/stm32f7xx_hal_timebase_tim_template.c

#ifndef __CHARGER_H
#define __CHARGER_H

#include "stdio.h"
#include "cmsis_os2.h"
#include "stm32f7xx_hal.h"

#ifdef  __cplusplus
extern "C"
{
#endif

#define PRINT                           printf


#define CHARGER_I2C_ADDRESS             0x6B<<1
#define CHARGER_I2C_TIMING              0x0040154A
#define CHARGER_I2C_TIMEOUT             1000
#define READ_WRITE_TRY_TIMES            5

#define I2Cx                            I2C1
#define RCC_PERIPHCLK_I2Cx              RCC_PERIPHCLK_I2C1
#define RCC_I2CxCLKSOURCE_SYSCLK        RCC_I2C1CLKSOURCE_PCLK1
#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SCL_SDA_AF                 GPIO_AF4_I2C1

/* Definition for ADC Pins */
#define BATT_VOLTAGE_Pin                GPIO_PIN_0
#define BATT_VOLTAGE_GPIO_Port          GPIOA

#define CHARGER_RST_Pin                 GPIO_PIN_14
#define CHARGER_RST_GPIO_PORT           GPIOB

#define REG_CHIP_VERSION_0              0x00
#define REG_CHIP_VERSION_1              0x01
#define REG_CHIP_VERSION_2              0x02
#define REG_CHIP_VERSION_3              0x03
#define REG_CHIP_VERSION_4              0x04
#define REG_CHIP_VERSION_5              0x05

#define REG_CHARGE_STATUS0              0x06
#define REG_BUCK_REG0                   0x60  
#define REG_CHG_REG0                    0x90  
#define REG_CHG_REG1                    0x91
#define REG_CHG_REG2                    0x92

#define STATUS_MASK_BUCK_ON             0x80
#define STATUS_MASK_OTG_ON              0x40
#define STATUS_MASK_CHARGE_STATE        0x30
#define STATUS_MASK_RECHARGE_STATE      0x01

#define CONTROL_CHARGE_ENABLE_MASK      0x20
#define CONTROL_BATFET_ONOFF_MASK       0x10

#define CURRENT_FAST_ICHG_MASK          0x1F
#define VOLTAGE_FAST_VCHG_MASK          0x78
#define VOLTAGE_TERM_ICHG_MASK          0x07

#define CHARGE_STATE_NOT_CHARGING       0x00
#define CHARGE_STATE_PRE_CHARGING       0x10
#define CHARGE_STATE_FAST_CHARGING      0x20
#define CHARGE_STATE_CHARGE_DONE        0x30

#define BUCK_ILIMIT_100_MA              0x00 << 2
#define BUCK_ILIMIT_130_MA              0x01 << 2
#define BUCK_ILIMIT_470_MA              0x02 << 2
#define BUCK_ILIMIT_900_MA              0x03 << 2
#define BUCK_ILIMIT_1000_MA             0x04 << 2
#define BUCK_ILIMIT_1080_MA             0x05 << 2
#define BUCK_ILIMIT_1200_MA             0x06 << 2
#define BUCK_ILIMIT_1300_MA             0x07 << 2
#define BUCK_ILIMIT_1400_MA             0x08 << 2
#define BUCK_ILIMIT_1500_MA             0x09 << 2
#define BUCK_ILIMIT_1600_MA             0x0A << 2
#define BUCK_ILIMIT_1700_MA             0x0B << 2
#define BUCK_ILIMIT_1800_MA             0x0C << 2
#define BUCK_ILIMIT_1900_MA             0x0D << 2
#define BUCK_ILIMIT_2000_MA             0x0E << 2
#define BUCK_ILIMIT_2100_MA             0x0F << 2
#define BUCK_ILIMIT_2200_MA             0x10 << 2
#define BUCK_ILIMIT_2280_MA             0x11 << 2
#define BUCK_ILIMIT_2400_MA             0x12 << 2
#define BUCK_ILIMIT_2480_MA             0x13 << 2
#define BUCK_ILIMIT_2600_MA             0x14 << 2
#define BUCK_ILIMIT_2680_MA             0x15 << 2
#define BUCK_ILIMIT_2800_MA             0x16 << 2
#define BUCK_ILIMIT_2880_MA             0x17 << 2
#define BUCK_ILIMIT_3000_MA             0x18 << 2
#define BUCK_ILIMIT_3080_MA             0x19 << 2
#define BUCK_ILIMIT_3200_MA             0x1A << 2

#define FAST_ICHG_100_MA                0x00
#define FAST_ICHG_200_MA                0x01
#define FAST_ICHG_300_MA                0x02
#define FAST_ICHG_400_MA                0x03
#define FAST_ICHG_500_MA                0x04
#define FAST_ICHG_600_MA                0x05
#define FAST_ICHG_700_MA                0x06
#define FAST_ICHG_800_MA                0x07
#define FAST_ICHG_900_MA                0x08
#define FAST_ICHG_1000_MA               0x09
#define FAST_ICHG_1200_MA               0x0A
#define FAST_ICHG_1400_MA               0x0B
#define FAST_ICHG_1600_MA               0x0C
#define FAST_ICHG_1800_MA               0x0D
#define FAST_ICHG_2000_MA               0x0E
#define FAST_ICHG_2100_MA               0x0F
#define FAST_ICHG_2200_MA               0x10
#define FAST_ICHG_2300_MA               0x11
#define FAST_ICHG_2400_MA               0x12
#define FAST_ICHG_2500_MA               0x13
#define FAST_ICHG_2600_MA               0x14
#define FAST_ICHG_2700_MA               0x15
#define FAST_ICHG_2800_MA               0x16
#define FAST_ICHG_2900_MA               0x17
#define FAST_ICHG_3000_MA               0x18
#define FAST_ICHG_3200_MA               0x19
#define FAST_ICHG_3400_MA               0x1A
#define FAST_ICHG_3600_MA               0x1B
#define FAST_ICHG_3800_MA               0x1C
#define FAST_ICHG_4000_MA               0x1D
#define FAST_ICHG_4200_MA               0x1E
#define FAST_ICHG_4500_MA               0x1F

#define FAST_VCHG_3800_MV               0x00 << 3
#define FAST_VCHG_3850_MV               0x01 << 3
#define FAST_VCHG_3900_MV               0x02 << 3
#define FAST_VCHG_3950_MV               0x03 << 3
#define FAST_VCHG_4000_MV               0x04 << 3
#define FAST_VCHG_4050_MV               0x05 << 3
#define FAST_VCHG_4100_MV               0x06 << 3
#define FAST_VCHG_4150_MV               0x07 << 3
#define FAST_VCHG_4200_MV               0x08 << 3
#define FAST_VCHG_4250_MV               0x09 << 3
#define FAST_VCHG_4300_MV               0x0A << 3
#define FAST_VCHG_4350_MV               0x0B << 3
#define FAST_VCHG_4400_MV               0x0C << 3
#define FAST_VCHG_4450_MV               0x0D << 3
#define FAST_VCHG_4500_MV               0x0E << 3
#define FAST_VCHG_4550_MV               0x0F << 3



typedef enum __charge_state
{
    NotCharging,
    PreCharging,
    FastCharging,
    ChargeDone,
    UnKnow
}ChargeState;


uint32_t Charger_Get_Batt_Voltage();
uint32_t Charger_Get_Batt_VoltageEx();

uint8_t Charger_Init(void);
void Charger_Reset(void);
uint8_t Charger_Enable(void);
uint8_t Charger_Disable(void);
uint8_t Charger_Get_Enabled();
ChargeState Charger_Get_ChargeState(void);
uint8_t Charger_Set_Ichg(int ichg);



#ifdef  __cplusplus
}
#endif


#endif
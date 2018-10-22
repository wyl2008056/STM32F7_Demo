/*
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "charger.h"
#include "stdio.h"

static I2C_HandleTypeDef hi2c1;
static ADC_HandleTypeDef hadc1;

static HAL_StatusTypeDef Charge_I2C_Init(void);
static HAL_StatusTypeDef Charger_IO_Init(void);
static HAL_StatusTypeDef Charger_Read_Reg(uint8_t reg, uint8_t *readdata);
static HAL_StatusTypeDef Charger_Write_Reg(uint8_t reg, uint8_t writedata);

static void MX_ADC1_Init(void);

uint8_t Charger_Init(void) {
    uint8_t i = 0, data = 0;
    uint8_t id[5] = {0, 0, 0, 0, 0}, version[2];

    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2Cx;
    PeriphClkInitStruct.I2c1ClockSelection = RCC_I2CxCLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        return HAL_ERROR;
    }


    MX_ADC1_Init();

    if (Charge_I2C_Init() != HAL_OK) {
        return HAL_ERROR;
    }
    PRINT("I2C stat = %d\n", HAL_I2C_GetState(&hi2c1));
    Charger_Read_Reg(REG_CHIP_VERSION_0, &id[0]);
    Charger_Read_Reg(REG_CHIP_VERSION_1, &id[1]);
    Charger_Read_Reg(REG_CHIP_VERSION_2, &id[2]);
    Charger_Read_Reg(REG_CHIP_VERSION_3, &id[3]);
    Charger_Read_Reg(REG_CHIP_VERSION_4, &version[0]);
    Charger_Read_Reg(REG_CHIP_VERSION_5, &version[1]);
    PRINT("Chip id is :Hi%s v%d%x\n", id, version[0], version[1]);
    if (id[0] == '6' && id[1] == '5' && id[2] == '2' && id[3] == '3') {
        return HAL_OK;
    }
    return HAL_ERROR;
}

void Charger_Reset(void) {
    PRINT("Charger_Reset\n");
    HAL_GPIO_WritePin(CHARGER_RST_GPIO_PORT, CHARGER_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(CHARGER_RST_GPIO_PORT, CHARGER_RST_Pin, GPIO_PIN_SET);
}

uint32_t Charger_Get_Batt_Voltage()
{
    uint32_t adcValue = 0;
    HAL_ADC_Start(&hadc1);
    if(HAL_ADC_PollForConversion(&hadc1, 1000) != HAL_OK)
    {
        PRINT("ADC Conversion error!\n");
    }
    adcValue = HAL_ADC_GetValue(&hadc1);
    return adcValue*3300*2/4096;
}

uint32_t Charger_Get_Batt_VoltageEx()
{
    uint32_t adcValue = 0, i;
    for(i=0; i<10; i++)
    {
        adcValue += Charger_Get_Batt_Voltage();
    }
    adcValue = adcValue/10;
    return adcValue;
}


uint8_t Charger_Enable(void) {
    uint8_t regValue;
    Charger_Read_Reg(REG_CHG_REG0, &regValue);
    regValue |= CONTROL_CHARGE_ENABLE_MASK;
    Charger_Write_Reg(REG_CHG_REG0, regValue);
    Charger_Read_Reg(REG_CHG_REG0, &regValue);
    if (regValue & CONTROL_CHARGE_ENABLE_MASK) {
        return HAL_OK;
    }
    return HAL_ERROR;
}

uint8_t Charger_Disable(void) {
    uint8_t regValue;
    Charger_Read_Reg(REG_CHG_REG0, &regValue);
    regValue &= ~CONTROL_CHARGE_ENABLE_MASK;
    Charger_Write_Reg(REG_CHG_REG0, regValue);
    Charger_Read_Reg(REG_CHG_REG0, &regValue);
    if (!(regValue & CONTROL_CHARGE_ENABLE_MASK)) {
        return HAL_OK;
    }
    return HAL_ERROR;
}

uint8_t Charger_Get_Enabled() {
    uint8_t regValue;
    Charger_Read_Reg(REG_CHG_REG0, &regValue);
    if (regValue & CONTROL_CHARGE_ENABLE_MASK) {
        return 1;
    }
    return 0;
}

ChargeState Charger_Get_ChargeState(void) {
    uint8_t status = 0;
    Charger_Read_Reg(REG_CHARGE_STATUS0, &status);
    switch (status & STATUS_MASK_CHARGE_STATE) {
    case CHARGE_STATE_NOT_CHARGING:
        return NotCharging;
    case CHARGE_STATE_PRE_CHARGING:
        return PreCharging;
    case CHARGE_STATE_FAST_CHARGING:
        return FastCharging;
    case CHARGE_STATE_CHARGE_DONE:
        return ChargeDone;
    default:
        return UnKnow;
    }
}

uint8_t Charger_Set_Ichg(int ichg) {
    uint8_t regValue, regichg;

    if (ichg < 0) {
        return HAL_ERROR;
    }
    if (ichg < 200) {
        regichg = FAST_ICHG_100_MA;
    } else if (ichg < 300) {
        regichg = FAST_ICHG_200_MA;
    } else if (ichg < 400) {
        regichg = FAST_ICHG_300_MA;
    } else if (ichg < 500) {
        regichg = FAST_ICHG_400_MA;
    } else if (ichg < 600) {
        regichg = FAST_ICHG_500_MA;
    } else if (ichg < 700) {
        regichg = FAST_ICHG_600_MA;
    } else if (ichg < 800) {
        regichg = FAST_ICHG_700_MA;
    } else if (ichg < 900) {
        regichg = FAST_ICHG_800_MA;
    } else if (ichg < 1000) {
        regichg = FAST_ICHG_900_MA;
    } else if (ichg < 1200) {
        regichg = FAST_ICHG_1000_MA;
    } else if (ichg < 1400) {
        regichg = FAST_ICHG_1200_MA;
    } else if (ichg < 1600) {
        regichg = FAST_ICHG_1400_MA;
    } else if (ichg < 1800) {
        regichg = FAST_ICHG_1600_MA;
    } else if (ichg < 2000) {
        regichg = FAST_ICHG_1800_MA;
    } else if (ichg < 2100) {
        regichg = FAST_ICHG_2000_MA;
    } else if (ichg < 2200) {
        regichg = FAST_ICHG_2100_MA;
    } else if (ichg < 2300) {
        regichg = FAST_ICHG_2200_MA;
    } else if (ichg < 2400) {
        regichg = FAST_ICHG_2300_MA;
    } else if (ichg < 2500) {
        regichg = FAST_ICHG_2400_MA;
    } else if (ichg < 2600) {
        regichg = FAST_ICHG_2500_MA;
    } else if (ichg < 2700) {
        regichg = FAST_ICHG_2600_MA;
    } else if (ichg < 2800) {
        regichg = FAST_ICHG_2700_MA;
    } else if (ichg < 2900) {
        regichg = FAST_ICHG_2800_MA;
    } else if (ichg < 3000) {
        regichg = FAST_ICHG_2900_MA;
    } else if (ichg < 3200) {
        regichg = FAST_ICHG_3000_MA;
    } else if (ichg < 3400) {
        regichg = FAST_ICHG_3200_MA;
    } else if (ichg < 3600) {
        regichg = FAST_ICHG_3400_MA;
    } else if (ichg < 3800) {
        regichg = FAST_ICHG_3600_MA;
    } else if (ichg < 4000) {
        regichg = FAST_ICHG_3800_MA;
    } else if (ichg < 4200) {
        regichg = FAST_ICHG_4000_MA;
    } else if (ichg < 4500) {
        regichg = FAST_ICHG_4200_MA;
    } else {
        regichg = FAST_ICHG_4500_MA;
    }
    Charger_Write_Reg(0x60, 0xfe);

    Charger_Read_Reg(REG_CHG_REG1, &regValue);
    regValue &= ~CURRENT_FAST_ICHG_MASK;
    regValue |= regichg;
    Charger_Write_Reg(REG_CHG_REG1, regValue);
    Charger_Read_Reg(REG_CHG_REG1, &regValue);
    if ((regValue & CURRENT_FAST_ICHG_MASK) == regichg) {
        return HAL_OK;
    }
    return HAL_ERROR;
}

uint8_t Charger_Set_Vchg(int vchg) {
    uint8_t regValue, regvchg;
    if (vchg < 3800) {
        return HAL_ERROR;
    }
    if (vchg < 3850) {
        regvchg = FAST_VCHG_3800_MV;
    } else if (vchg < 3900) {
        regvchg = FAST_VCHG_3850_MV;
    } else if (vchg < 3950) {
        regvchg = FAST_VCHG_3900_MV;
    } else if (vchg < 4000) {
        regvchg = FAST_VCHG_3950_MV;
    } else if (vchg < 4050) {
        regvchg = FAST_VCHG_4000_MV;
    } else if (vchg < 4100) {
        regvchg = FAST_VCHG_4050_MV;
    } else if (vchg < 4150) {
        regvchg = FAST_VCHG_4100_MV;
    } else if (vchg < 4200) {
        regvchg = FAST_VCHG_4150_MV;
    } else if (vchg < 4250) {
        regvchg = FAST_VCHG_4200_MV;
    } else if (vchg < 4300) {
        regvchg = FAST_VCHG_4250_MV;
    } else if (vchg < 4350) {
        regvchg = FAST_VCHG_4300_MV;
    } else if (vchg < 4400) {
        regvchg = FAST_VCHG_4350_MV;
    } else if (vchg < 4450) {
        regvchg = FAST_VCHG_4400_MV;
    } else if (vchg < 4500) {
        regvchg = FAST_VCHG_4450_MV;
    } else if (vchg < 4550) {
        regvchg = FAST_VCHG_4500_MV;
    } else {
        regvchg = FAST_VCHG_4550_MV;
    }

    Charger_Read_Reg(REG_CHG_REG2, &regValue);
    regValue &= ~VOLTAGE_FAST_VCHG_MASK;
    regValue |= regvchg;
    Charger_Write_Reg(REG_CHG_REG2, regValue);
    Charger_Read_Reg(REG_CHG_REG2, &regValue);
    if ((regValue & VOLTAGE_FAST_VCHG_MASK) == regvchg) {
        return HAL_OK;
    }
    return HAL_ERROR;
}

// return HAL_I2C_Mem_Read(&hi2c1, CHARGER_I2C_ADDRESS, reg,
// I2C_MEMADD_SIZE_8BIT, readdata, 1, CHARGER_I2C_TIMEOUT);

// while(HAL_I2C_Master_Transmit(&hi2c1, CHARGER_I2C_ADDRESS, &reg, 1,
// CHARGER_I2C_TIMEOUT) != HAL_OK)
// {
//     printf("error 1\n");
//     if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//     {
//         return HAL_ERROR;
//     }
// }
static HAL_StatusTypeDef Charger_Read_Reg(uint8_t reg, uint8_t *readdata) {
    uint8_t i;
    for (i = 0; i < READ_WRITE_TRY_TIMES; i++) {
        if (HAL_I2C_Master_Transmit(&hi2c1, CHARGER_I2C_ADDRESS, &reg, 1,
                                    CHARGER_I2C_TIMEOUT) == HAL_OK) {
            break;
        } else {
            if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF) {
                return HAL_ERROR;
            }
            if (i == READ_WRITE_TRY_TIMES - 1) {
                return HAL_TIMEOUT;
            }
        }
    }

    if (HAL_I2C_Master_Receive(&hi2c1, CHARGER_I2C_ADDRESS, readdata, 1,
                               CHARGER_I2C_TIMEOUT) != HAL_OK) {
        if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF) {
            return HAL_ERROR;
        }
    }
    return HAL_OK;
}

static HAL_StatusTypeDef Charger_Write_Reg(uint8_t reg, uint8_t writedata) {
    return HAL_I2C_Mem_Write(&hi2c1, CHARGER_I2C_ADDRESS, reg,
                             I2C_MEMADD_SIZE_8BIT, &writedata, 1,
                             CHARGER_I2C_TIMEOUT);
}

// static HAL_StatusTypeDef Charger_Write_Reg(uint8_t reg, uint8_t writedata)
// {
//     uint8_t i;
//     for(i=0; i<READ_WRITE_TRY_TIMES; i++)
//     {
//         if(HAL_I2C_Master_Transmit(&hi2c1, CHARGER_I2C_ADDRESS, &reg, 1,
//         CHARGER_I2C_TIMEOUT) == HAL_OK)
//         {
//             break;
//         }
//         else
//         {
//             if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//             {
//                 return HAL_ERROR;
//             }
//             if(i == READ_WRITE_TRY_TIMES-1)
//             {
//                 return HAL_TIMEOUT;
//             }
//         }
//     }

//     if(HAL_I2C_Master_Transmit(&hi2c1, CHARGER_I2C_ADDRESS, &writedata, 1,
//     CHARGER_I2C_TIMEOUT) != HAL_OK)
//     {
//         if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//         {
//             return HAL_ERROR;
//         }
//     }
//     return HAL_OK;
// }

static HAL_StatusTypeDef Charge_I2C_Init(void) {
    hi2c1.Instance = I2Cx;
    hi2c1.Init.Timing = CHARGER_I2C_TIMING;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    Charger_IO_Init();
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        PRINT("I2C init error\n");
        return HAL_ERROR;
    }

    // if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) !=
    // HAL_OK) {
    //     PRINT("HAL_I2CEx_ConfigAnalogFilter error\n");
    //     return HAL_ERROR;
    // }

    // if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
    //     PRINT("HAL_I2CEx_ConfigDigitalFilter init error\n");
    //     return HAL_ERROR;
    // }
    return HAL_OK;
}

static HAL_StatusTypeDef Charger_IO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    I2Cx_SDA_GPIO_CLK_ENABLE();

    // init Reset GPIO
    GPIO_InitStruct.Pin = CHARGER_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CHARGER_RST_GPIO_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(CHARGER_RST_GPIO_PORT, CHARGER_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(CHARGER_RST_GPIO_PORT, CHARGER_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(500);

    // init I2c CLK and SDA GPIO
    GPIO_InitStruct.Pin = I2Cx_SCL_PIN | I2Cx_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = I2Cx_SCL_SDA_AF;
    HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

    I2Cx_CLK_ENABLE();

    return HAL_OK;
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
      PRINT("ADC Init errror \n");
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
        PRINT("ADC Init errror \n");
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration    
    PA0/WKUP     ------> ADC1_IN0 
    */
    GPIO_InitStruct.Pin = BATT_VOLTAGE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BATT_VOLTAGE_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PA0/WKUP     ------> ADC1_IN0 
    */
    HAL_GPIO_DeInit(BATT_VOLTAGE_GPIO_Port, BATT_VOLTAGE_Pin);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }

}
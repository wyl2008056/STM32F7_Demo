/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/
#include "gpio.h"

using namespace System::IO;

GPIO::GPIO(GPIO_Port port, GPIO_Pin pin)
{
    GPIO{port, pin, OutputPP};
}

GPIO::GPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode)
{
    GPIO{port, pin, mode, NoPull};
}

GPIO::GPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_Pull pull)
{
    GPIO{port, pin, mode, pull, High};
}

GPIO::GPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_Pull pull, GPIO_Speed speed)
{
    switch(port)
    {
        case GPIO_A:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case GPIO_B:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case GPIO_C:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case GPIO_D:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        case GPIO_E:
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
        case GPIO_F:
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
        case GPIO_G:
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
        case GPIO_H:
            __HAL_RCC_GPIOH_CLK_ENABLE();
            break;
        case GPIO_I:
            __HAL_RCC_GPIOI_CLK_ENABLE();
            break;
        case GPIO_J:
            __HAL_RCC_GPIOJ_CLK_ENABLE();
            break;
        case GPIO_K:
            __HAL_RCC_GPIOK_CLK_ENABLE();
            break;
    }
    _gpio = (GPIO_TypeDef *)port;
    _pin = (uint32_t)pin;
    GPIO_InitTypeDef gpio_init;
    gpio_init.Pin = _pin;
    gpio_init.Mode = (uint32_t)mode;
    gpio_init.Pull = (uint32_t)pull;
    gpio_init.Speed = (uint32_t)speed;
    HAL_GPIO_Init(_gpio, &gpio_init);
}

GPIO::~GPIO()
{
    HAL_GPIO_DeInit(_gpio, _pin);
}

void GPIO::Write(bool value)
{
    HAL_GPIO_WritePin(_gpio, _pin, value? GPIO_PIN_SET:GPIO_PIN_RESET);
}

bool GPIO::Read()
{
    return (GPIO_PIN_SET == HAL_GPIO_ReadPin(_gpio, _pin));
}

void GPIO::Toggle()
{
    HAL_GPIO_TogglePin(_gpio, _pin);
}

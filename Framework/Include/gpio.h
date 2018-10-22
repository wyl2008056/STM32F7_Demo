/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  __GPIO_H
#define  __GPIO_H

#include "fw_cfg.h"

namespace System::IO
{
	enum GPIO_Port
	{
		GPIO_A = GPIOA_BASE,
		GPIO_B = GPIOB_BASE,
		GPIO_C = GPIOC_BASE,
		GPIO_D = GPIOD_BASE,
		GPIO_E = GPIOE_BASE,
		GPIO_F = GPIOF_BASE,
		GPIO_G = GPIOG_BASE,
		GPIO_H = GPIOH_BASE,
		GPIO_I = GPIOI_BASE,
		GPIO_J = GPIOJ_BASE,
		GPIO_K = GPIOK_BASE
	};

	enum GPIO_Pin
	{
		Pin0 	= GPIO_PIN_0, 
		Pin1 	= GPIO_PIN_1,  
		Pin2 	= GPIO_PIN_2,  
		Pin3 	= GPIO_PIN_3,  
		Pin4 	= GPIO_PIN_4,  
		Pin5 	= GPIO_PIN_5,  
		Pin6 	= GPIO_PIN_6,  
		Pin7 	= GPIO_PIN_7,  
		Pin8 	= GPIO_PIN_8,  
		Pin9 	= GPIO_PIN_9,  
		Pin10 	= GPIO_PIN_10, 
		Pin11 	= GPIO_PIN_11, 
		Pin12 	= GPIO_PIN_12, 
		Pin13 	= GPIO_PIN_13,  
		Pin14 	= GPIO_PIN_14, 
		Pin15 	= GPIO_PIN_15,  
		PinAll 	= GPIO_PIN_All
	};

	enum GPIO_Mode
	{
		Inout 	 = GPIO_MODE_INPUT,
		OutputPP = GPIO_MODE_OUTPUT_PP,
		OutputOD = GPIO_MODE_OUTPUT_OD,
		AfPP 	 = GPIO_MODE_AF_PP,
		AfOD	 = GPIO_MODE_AF_OD
	};

	enum GPIO_Pull
	{
		NoPull 	 = GPIO_NOPULL,
		PullUp   = GPIO_PULLUP,
		PullDown = GPIO_PULLDOWN
	};

	enum GPIO_Speed
	{
		Low 	 = GPIO_SPEED_FREQ_LOW,
		Medium   = GPIO_SPEED_FREQ_MEDIUM,
		High 	 = GPIO_SPEED_FREQ_HIGH,
		VeryHigh = GPIO_SPEED_FREQ_VERY_HIGH,
	};

	class GPIO
	{
		private:
			GPIO_TypeDef *_gpio;
			uint32_t _pin;
		public:
			GPIO(GPIO_Port port, GPIO_Pin pin);
			GPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode);
			GPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_Pull pull);
			GPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_Pull pull, GPIO_Speed speed);
			~GPIO();

			void Write(bool value);
			bool Read();
			void Toggle();
	};
}



/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of CPU cfg module include.                       */


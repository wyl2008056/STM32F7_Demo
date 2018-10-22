/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  __DATETIME_H
#define  __DATETIME_H

#include "fw_cfg.h"

namespace System
{
	class DateTime
	{
		public:
			int Second;
			int Minute;
			int Hour;
			int WeekDay;
			int Day;
			int Month;
			int Year;
			//static RTC_HandleTypeDef rtcHandle;
			DateTime();
			DateTime(int year, int month, int day, int weekday, int hour, int minute, int second);
			~DateTime();
			DateTime* Now();
			static void Init();
			void SetDate(int year, int month, int day, int weekday);
			void SetTime(int hour, int minute, int second);
			void SetDateTime(int year, int month, int day, int weekday, int hour, int minute, int second);
			void SetTicks(int tick);
			const char* GetTimeStr(const char* fmt, char* res);
			
		private:
			int iTicks;
			RTC_HandleTypeDef *rtc;
			static const uint32_t RTC_ASYNC_PREDIV = 0x1F;
  			static const uint32_t RTC_SYNC_PREDIV = 0x3FF;
	};
};



/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of CPU cfg module include.                       */


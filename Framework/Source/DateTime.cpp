/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/
#include "DateTime.h"
#include "string.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rtc.h"

using namespace System;

extern RTC_HandleTypeDef rtcHandle;

DateTime::DateTime():
DateTime::DateTime(0, 0, 0, 0, 0, 0, 0)
{
	
}

DateTime::DateTime(int year, int month, int day, int weekday, int hour, int minute, int second)
{
	rtc = &rtcHandle;
	if(HAL_RTC_GetState(rtc) != HAL_RTC_STATE_READY)
	{
		printf("RTC not yet initialized!\n");
	}
	this->Year 		= year;
	this->Month 	= month;
	this->Day   	= day;
	this->WeekDay 	= weekday;
	this->Hour  	= hour;
	this->Minute	= minute;
	this->Second	= second;
}

DateTime::~DateTime()
{
	//HAL_RTC_DeInit(&(this->rtcHandle));
}

DateTime* DateTime::Now()
{
	RTC_DateTypeDef rtcDate;
  	RTC_TimeTypeDef rtcTime;
	
	HAL_RTC_GetTime(rtc, &rtcTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(rtc, &rtcDate, RTC_FORMAT_BIN);

	this->Year 		= rtcDate.Year + 2000;
	this->Month 	= rtcDate.Month;
	this->Day   	= rtcDate.Date;
	this->WeekDay 	= rtcDate.WeekDay;
	this->Hour  	= rtcTime.Hours;
	this->Minute	= rtcTime.Minutes;
	this->Second	= rtcTime.Seconds;

	return this;
}

void DateTime::SetDate(int year, int month, int day, int weekday)
{
	this->Year 		= year;
	this->Month 	= month;
	this->Day   	= day;
	this->WeekDay 	= weekday;
	RTC_DateTypeDef rtcDate;
	rtcDate.Date 	= day;
	rtcDate.Month 	= month;
	rtcDate.Year 	= year % 100;
	rtcDate.WeekDay = weekday;
	HAL_RTC_SetDate(rtc, &rtcDate, FORMAT_BIN);
}

void DateTime::SetTime(int hour, int minute, int second)
{
	this->Hour  	= hour;
	this->Minute	= minute;
	this->Second	= second;
	RTC_TimeTypeDef rtcTime;
	rtcTime.TimeFormat = RTC_HOURFORMAT_24;
	rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtcTime.StoreOperation = RTC_STOREOPERATION_SET;
	rtcTime.SubSeconds = 0;
	rtcTime.Seconds = second;
	rtcTime.Minutes = minute;
	rtcTime.Hours = hour;
	HAL_RTC_SetTime(rtc, &rtcTime, FORMAT_BIN);
}

void DateTime::SetDateTime(int year, int month, int day, int weekday, int hour, int minute, int second)
{
	SetTime(hour, minute, second);
	SetDate(year, month, day, weekday);
}

void DateTime::SetTicks(int tick)
{
	this->iTicks = tick;
}

/*
	frm: yyyy: 	year(2018)
		 mm  : 	month(01)
		 mmm :	month(Jan)
		 dd  :	day(28)
		 w   :	weekday(1)
		 www :	weekday(Mon)
		 hh  ：	hour(22)
		 MM  ：	minute(50)
		 ss  ：	second(12)
	eg：yyyy-mm-dd hh:MM:ss    		-> 2018-01-28 22:50:12
	    [mmm dd yyyy www hh:MM:ss]  -> [Jan 01 2018 Mon 22:50:12]
*/
const char* DateTime::GetTimeStr(const char* fmt, char *timeStr)
{
	char *p;
	char tmp[5];
	char i = 0;
	stpcpy(timeStr, fmt);
	p = strstr(timeStr, "yyyy");
	if(p != NULL)
	{
		sprintf(tmp, "%04d", this->Year);
		strncpy(p, tmp, 4);
	}
	p = strstr(timeStr, "mmm");
	if(p != NULL)
	{
		//char* months[12] = {"JANU", "FEBR", "MAR ", "APR ", "MAY ", "JUNE", "JULY", "AUGU", "SEP ", "OCT " "NOV ", "DEC "};
		char months[12][3] = {{'J','a','n'}, {'F','e','b'}, {'M','a','r'}, {'A','p','r'}, {'M','a','y'}, {'J','u','n'},  
				            {'J','u','l'}, {'A','u','g'}, {'S','e','p'}, {'O','c','t'}, {'N','o','v'}, {'D','e','c'}};
		strncpy(p, months[this->Month-1], 3);
	}
	p = strstr(timeStr, "mm");
	if(p != NULL)
	{
		sprintf(tmp, "%02d", this->Month);
		strncpy(p, tmp, 2);
	}
	p = strstr(timeStr, "dd");
	if(p != NULL)
	{
		sprintf(tmp, "%02d", this->Day);
		strncpy(p, tmp, 2);
	}
	p = strstr(timeStr, "www");
	if(p != NULL)
	{
		//char* weekdays[7] = {"MON", "TUE", "WED", "THU", "FRI" ,"SAT" , "SUN"};
		char weekdays[7][3] = {{'M','o','n'}, {'T','u','e'}, {'W','e','d'}, {'T','h','u'}, 
							   {'F','r','i'} ,{'S','a','t'} , {'S','u','n'}};
		strncpy(p, weekdays[this->WeekDay-1], 3);
	}
	p = strstr(timeStr, "w");
	if(p != NULL)
	{
		sprintf(tmp, "%d", this->WeekDay);
		strncpy(p, tmp, 1);
	}
	p = strstr(timeStr, "hh");
	if(p != NULL)
	{
		sprintf(tmp, "%02d", this->Hour);
		strncpy(p, tmp, 2);
	}
	p = strstr(timeStr, "MM");
	if(p != NULL)
	{
		sprintf(tmp, "%02d", this->Minute);
		strncpy(p, tmp, 2);
	}
	p = strstr(timeStr, "ss");
	if(p != NULL)
	{
		sprintf(tmp, "%02d", this->Second);
		strncpy(p, tmp, 2);
	}
	return timeStr;
}


/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <date/date.h>

int32_t date::GetYear(void)
{
	return BUILD_YEAR;
}

int32_t date::GetMonth(void)
{
	return BUILD_MONTH;
}

int32_t date::GetDay(void)
{
	return BUILD_DAY;
}

int32_t date::GetHour(void)
{
	return BUILD_HOUR;
}

int32_t date::GetMinute(void)
{
	return BUILD_MINUTE;
}

int32_t date::GetSecond(void)
{
	return BUILD_SECOND;
}



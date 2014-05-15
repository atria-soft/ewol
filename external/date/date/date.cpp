/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <date/date.h>
#ifndef BUILD_YEAR
	#define BUILD_YEAR 1070
#endif
#ifndef BUILD_MONTH
	#define BUILD_MONTH 1
#endif
#ifndef BUILD_DAY
	#define BUILD_DAY 1
#endif
#ifndef BUILD_HOUR
	#define BUILD_HOUR 0
#endif
#ifndef BUILD_MINUTE
	#define BUILD_MINUTE 0
#endif
#ifndef BUILD_SECOND
	#define BUILD_SECOND 0
#endif

int32_t date::getYear() {
	return BUILD_YEAR;
}

int32_t date::getMonth() {
	return BUILD_MONTH;
}

int32_t date::getDay() {
	return BUILD_DAY;
}

int32_t date::getHour() {
	return BUILD_HOUR;
}

int32_t date::getMinute() {
	return BUILD_MINUTE;
}

int32_t date::getSecond() {
	return BUILD_SECOND;
}



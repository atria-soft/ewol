/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __DATE_H__
#define __DATE_H__

#include <stdint.h>

namespace date {
	int32_t getYear(void);
	int32_t getMonth(void);
	int32_t getDay(void);
	int32_t getHour(void);
	int32_t getMinute(void);
	int32_t getSecond(void);
};


#endif

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
	int32_t getYear();
	int32_t getMonth();
	int32_t getDay();
	int32_t getHour();
	int32_t getMinute();
	int32_t getSecond();
};


#endif

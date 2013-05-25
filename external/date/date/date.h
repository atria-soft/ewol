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

namespace date
{
	int32_t GetYear(void);
	int32_t GetMonth(void);
	int32_t GetDay(void);
	int32_t GetHour(void);
	int32_t GetMinute(void);
	int32_t GetSecond(void);
};


#endif

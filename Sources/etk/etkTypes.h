/**
 *******************************************************************************
 * @file etkTypes.h
 * @brief Ewol Tool Kit : generique define type
 * @author Edouard DUPIN
 * @date 08/06/2010
 * @par Project
 * Ewol TK
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __ETK_TYPES_H__
#define __ETK_TYPES_H__

// includes system, malloc, EXIT_SUCCESS
#include <stdlib.h>
// includes fopen, fwrite, fseek, ftell
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>


#ifndef __int8_t_defined
#   define __int8_t_defined
    typedef   signed char                    int8_t;
    typedef   signed short int               int16_t;
    typedef   int                            int32_t;
    typedef   signed long long int           int64_t;
#endif

#ifndef __uint8_t_defined
#   define __uint8_t_defined
	typedef unsigned char                   uint8_t;
	typedef unsigned short int              uint16_t;
	typedef unsigned int                    uint32_t;
	#ifndef __uint64_t_defined
	typedef unsigned long int          uint64_t;
	#endif
#endif

#define etk_min(elemA, elemB)               ((elemA)<(elemB)) ? (elemA) : (elemB)
#define etk_max(elemA, elemB)               ((elemA)<(elemB)) ? (elemB) : (elemA)
#define etk_avg(minimim, elem, maximum)     ((minimim)>(elem)) ? (minimim) : ((maximum)<(elem)) ? (maximum) : (elem)

extern "C"
{
	struct etkPointAndPosition{
		double x;
		double y;
	};
	typedef etkPointAndPosition     point_ts;
	typedef etkPointAndPosition     position_ts;
	typedef etkPointAndPosition     size_ts;
}



#endif



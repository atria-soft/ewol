/**
 *******************************************************************************
 * @file etk/Types.h
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
#	if __WORDSIZE == 64
		typedef unsigned long int               uint64_t;
#	else
		typedef unsigned long long int          uint64_t;
#	endif
#endif
typedef uint32_t       uniChar_t;




#define etk_min(elemA, elemB)               ((elemA)<(elemB)) ? (elemA) : (elemB)
#define etk_max(elemA, elemB)               ((elemA)<(elemB)) ? (elemB) : (elemA)
#define etk_avg(minimim, elem, maximum)     ((minimim)>(elem)) ? (minimim) : ((maximum)<(elem)) ? (maximum) : (elem)

extern "C"
{
	#ifdef EWOL_PECISION_DOUBLE
		typedef double       etkFloat_t;
		#define oglTypeFloat_t   GL_DOUBLE
	#else
		typedef float        etkFloat_t;
		#define oglTypeFloat_t   GL_FLOAT
	#endif
	
	struct etkPointAndPositionDouble{
		etkFloat_t x;
		etkFloat_t y;
	};
	
	struct etkPointAndPositionInt{
		int32_t x;
		int32_t y;
	};
	
	typedef etkPointAndPositionDouble     point_ts;
	typedef etkPointAndPositionDouble     position_ts;
	typedef etkPointAndPositionDouble     size_ts;
	typedef etkPointAndPositionInt        intSize_ts;
	
	typedef struct {
		etkFloat_t x;
		etkFloat_t y;
		etkFloat_t z;
	}coord3D_ts;
	typedef struct {
		etkFloat_t x;
		etkFloat_t y;
	}coord2D_ts;
	typedef struct {
		etkFloat_t u;
		etkFloat_t v;
	}texCoord_ts;
	typedef struct {
		etkFloat_t red;
		etkFloat_t green;
		etkFloat_t blue;
		etkFloat_t alpha;
	}color_ts;
}



#endif



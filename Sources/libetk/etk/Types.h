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

#include <stdlib.h>
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


typedef enum {
    ERR_NONE = 0,   //!< No error, luckily everything went fine
    ERR_FAIL,       //!< Miscellaneous failure
    ERR_INVAL,      //!< Invalid entry parameter
    ERR_MEM,        //!< Dynamic memory allocation failure
    ERR_TIMEOUT,    //!< Request time out
    ERR_BUSY,       //!< Element curently Busy
}erreurCode_te;


#define etk_min(elemA, elemB)               (((elemA)<(elemB)) ? (elemA) : (elemB))
#define etk_max(elemA, elemB)               (((elemA)<(elemB)) ? (elemB) : (elemA))
#define etk_avg(minimim, elem, maximum)     (((minimim)>(elem)) ? (minimim) : ((maximum)<(elem)) ? (maximum) : (elem))

typedef struct {
	float u;
	float v;
}texCoord_ts;

typedef struct {
	float x;
	float y;
	float w;
	float h;
}clipping_ts;

#include <etk/math/math.h>

#endif

/**
 *******************************************************************************
 * @file etk/DebugInternal.h
 * @brief Ewol Tool Kit : log wrapper (header)
 * @author Edouard DUPIN
 * @date 19/10/2011
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

#ifndef __ETK_DEBUG_INTERNAL_H__
#define __ETK_DEBUG_INTERNAL_H__

#include "etk/Debug.h"

extern const char * etkLibName;

#define TK_CRITICAL(data)			ETK_CRITICAL(etkLibName, data)
// General 
#if ETK_DEBUG_LEVEL > 0
#	define TK_WARNING(data)			ETK_WARNING(etkLibName, data)
#	define TK_ERROR(data)			ETK_ERROR(etkLibName, data)
#else
#	define TK_WARNING(data)			do {}while(0)
#	define TK_ERROR(data)			do {}while(0)
#endif

#if ETK_DEBUG_LEVEL > 1
#	define TK_INFO(data)			ETK_INFO(etkLibName, data)
#else
#	define TK_INFO(data)			do {}while(0)
#endif

#if ETK_DEBUG_LEVEL > 2
#	define TK_DEBUG(data)			ETK_DEBUG(etkLibName, data)
#else
#	define TK_DEBUG(data)			do {}while(0)
#endif

#if ETK_DEBUG_LEVEL > 3
#	define TK_VERBOSE(data)			ETK_VERBOSE(etkLibName, data)
#else
#	define TK_VERBOSE(data)			do {}while(0)
#endif

#define TK_ASSERT(cond, data)		ETK_ASSERT(etkLibName, cond, data)

#if ETK_DEBUG_LEVEL > 1
#   define TK_CHECK_INOUT(cond)		ETK_CHECK_INOUT_ASSERT(etkLibName, cond)
#elif ETK_DEBUG_LEVEL > 0
#   define TK_CHECK_INOUT(cond)		ETK_CHECK_INOUT_WARNING(etkLibName, cond)
#else
#   define TK_CHECK_INOUT(cond)		do { } while (0)
#endif

#endif


/**
 *******************************************************************************
 * @file ewolDebug.h
 * @brief Ewol : log wrapper (header)
 * @author Edouard DUPIN
 * @date 19/10/2010
 * @par Project
 * Ewol
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

#ifndef __EWOL_DEBUG_H__
#define __EWOL_DEBUG_H__

#include <etkTypes.h>
#include <etkDebug.h>

extern const char * ewolLibName;

#define EWOL_CRITICAL(data)         ETK_CRITICAL(ewolLibName, data)
// General 
#if EWOL_DEBUG_LEVEL > 0
#	define EWOL_WARNING(data)       ETK_WARNING(ewolLibName, data)
#	define EWOL_ERROR(data)         ETK_ERROR(ewolLibName, data)
#else
#	define EWOL_WARNING(data)       do {}while(0)
#	define EWOL_ERROR(data)         do {}while(0)
#endif

#if EWOL_DEBUG_LEVEL > 1
#	define EWOL_INFO(data)          ETK_INFO(ewolLibName, data)
#else
#	define EWOL_INFO(data)          do {}while(0)
#endif

#if EWOL_DEBUG_LEVEL > 2
#	define EWOL_DEBUG(data)         ETK_DEBUG(ewolLibName, data)
#else
#	define EWOL_DEBUG(data)         do {}while(0)
#endif

#define EWOL_TODO(data)             EWOL_WARNING("TODO : " << data)

#define EWOL_ASSERT(cond, data)     ETK_ASSERT(ewolLibName, cond, data)

#if EWOL_DEBUG_LEVEL > 1
#	define EWOL_CHECK_INOUT(cond)   ETK_CHECK_INOUT_ASSERT(ewolLibName, cond)
#elif EWOL_DEBUG_LEVEL > 0
#	define EWOL_CHECK_INOUT(cond)   ETK_CHECK_INOUT_WARNING(ewolLibName, cond)
#else
#	define EWOL_CHECK_INOUT(cond)   do { } while (0)
#endif

#endif


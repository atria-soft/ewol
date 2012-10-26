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

#include "etk/Debug.h"

#ifndef __ETK_DEBUG_INTERNAL_H__
#define __ETK_DEBUG_INTERNAL_H__


extern const char * etkLibName;

#define TK_CRITICAL(data)			ETK_CRITICAL(etkLibName, data)
#define TK_WARNING(data)			ETK_WARNING(etkLibName, data)
#define TK_ERROR(data)				ETK_ERROR(etkLibName, data)
#define TK_INFO(data)				ETK_INFO(etkLibName, data)
#define TK_DEBUG(data)				ETK_DEBUG(etkLibName, data)
#define TK_VERBOSE(data)			ETK_VERBOSE(etkLibName, data)
#define TK_ASSERT(cond, data)		ETK_ASSERT(etkLibName, cond, data)
#define TK_CHECK_INOUT(cond)		ETK_CHECK_INOUT(etkLibName, cond)
#define TK_TODO(cond)				ETK_TODO(etkLibName, cond)

#endif


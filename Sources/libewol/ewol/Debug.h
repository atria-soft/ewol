/**
 *******************************************************************************
 * @file ewol/Debug.h
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

#include <etk/Types.h>
#include <etk/Debug.h>

extern const char * ewolLibName;

#define EWOL_CRITICAL(data)			ETK_CRITICAL(ewolLibName, data)
#define EWOL_WARNING(data)			ETK_WARNING(ewolLibName, data)
#define EWOL_ERROR(data)			ETK_ERROR(ewolLibName, data)
#define EWOL_INFO(data)				ETK_INFO(ewolLibName, data)
#define EWOL_DEBUG(data)			ETK_DEBUG(ewolLibName, data)
#define EWOL_VERBOSE(data)			ETK_VERBOSE(ewolLibName, data)
#define EWOL_ASSERT(cond, data)		ETK_ASSERT(ewolLibName, cond, data)
#define EWOL_CHECK_INOUT(cond)		ETK_CHECK_INOUT(ewolLibName, cond)
#define EWOL_TODO(cond)				ETK_TODO(ewolLibName, cond)

#endif


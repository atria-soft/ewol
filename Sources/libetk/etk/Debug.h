/**
 *******************************************************************************
 * @file etk/Debug.h
 * @brief Ewol Tool Kit : log implementation
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

#ifndef __ETK_DEBUG_H__
#define __ETK_DEBUG_H__

#include <etk/Stream.h>
#include <etk/Types.h>

// Log Message System For EDN
void TOOLS_DisplayFuncName(int32_t ligne, const char* className, const char* funcName, const char* libName);
void TOOLS_DisplayTime(void);


#undef  __class__
#define __class__				(NULL)

#define ETK_DBG_COMMON(libName, info, data)			do { \
														etk::cout << etk::cstart << info; \
														TOOLS_DisplayTime(); \
														TOOLS_DisplayFuncName(__LINE__, __class__, __func__, libName); \
														etk::cout << data; \
														etk::cout <<etk::endl; \
													}while(0)
#define ETK_CRITICAL(libName, data)					ETK_DBG_COMMON(libName, etk::LOG_LEVEL_CRITICAL, data)
#define ETK_ERROR(libName, data)					ETK_DBG_COMMON(libName, etk::LOG_LEVEL_ERROR, data)
#define ETK_WARNING(libName, data)					ETK_DBG_COMMON(libName, etk::LOG_LEVEL_WARNING, data)
#define ETK_INFO(libName, data)						ETK_DBG_COMMON(libName, etk::LOG_LEVEL_INFO, data)
#define ETK_DEBUG(libName, data)					ETK_DBG_COMMON(libName, etk::LOG_LEVEL_DEBUG, data)
#define ETK_VERBOSE(libName, data)					ETK_DBG_COMMON(libName, etk::LOG_LEVEL_VERBOSE, data)
#define ETK_ASSERT(libName, cond, data)				do { \
														if (!(cond)) { \
															ETK_CRITICAL(libName, data); \
															assert(!#cond); \
														} \
													} while (0)
#define ETK_CHECK_INOUT_ASSERT(libName, cond)		ETK_ASSERT(libName, (cond), "Internal input error : "#cond)
#define ETK_CHECK_INOUT_WARNING(libName, cond)		do { \
														if (!(cond)) { \
															ETK_CRITICAL(libName, "Internal input error : "#cond); \
														} \
													} while (0)
#define ETK_CHECK_MAGIC(libName, cond)				ETK_ASSERT(libName, (cond), "MAGIC check error : "#cond)

#endif

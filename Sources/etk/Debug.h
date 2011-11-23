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

#include <iostream>
#include <etk/Types.h>

// Log Message System For EDN
void TOOLS_DisplayFuncName(int32_t ligne, const char* className, const char* funcName, const char* libName);
void TOOLS_DisplayTime(void);

//regular colors
#define ETK_BASH_COLOR_BLACK				"\e[0;30m"
#define ETK_BASH_COLOR_RED				"\e[0;31m"
#define ETK_BASH_COLOR_GREEN				"\e[0;32m"
#define ETK_BASH_COLOR_YELLOW			"\e[0;33m"
#define ETK_BASH_COLOR_BLUE				"\e[0;34m"
#define ETK_BASH_COLOR_MAGENTA			"\e[0;35m"
#define ETK_BASH_COLOR_CYAN				"\e[0;36m"
#define ETK_BASH_COLOR_WHITE				"\e[0;37m"
//emphasized (bolded) colors
#define ETK_BASH_COLOR_BOLD_BLACK		"\e[1;30m"
#define ETK_BASH_COLOR_BOLD_RED			"\e[1;31m"
#define ETK_BASH_COLOR_BOLD_GREEN		"\e[1;32m"
#define ETK_BASH_COLOR_BOLD_YELLOW		"\e[1;33m"
#define ETK_BASH_COLOR_BOLD_BLUE			"\e[1;34m"
#define ETK_BASH_COLOR_BOLD_MAGENTA		"\e[1;35m"
#define ETK_BASH_COLOR_BOLD_CYAN			"\e[1;36m"
#define ETK_BASH_COLOR_BOLD_WHITE		"\e[1;37m"
//background colors
#define ETK_BASH_COLOR_BG_BLACK			"\e[40m"
#define ETK_BASH_COLOR_BG_RED			"\e[41m"
#define ETK_BASH_COLOR_BG_GREEN			"\e[42m"
#define ETK_BASH_COLOR_BG_YELLOW			"\e[43m"
#define ETK_BASH_COLOR_BG_BLUE			"\e[44m"
#define ETK_BASH_COLOR_BG_MAGENTA		"\e[45m"
#define ETK_BASH_COLOR_BG_CYAN			"\e[46m"
#define ETK_BASH_COLOR_BG_WHITE			"\e[47m"
// Return to the normal color setings
#define ETK_BASH_COLOR_NORMAL			"\e[0m"
//go to the Top of bash
#define ETK_BASH_GO_TOP					"\e[0;0f"

#undef  __class__
#define __class__				(NULL)

#define ETK_DBG_COMMON(libName, color, info, data)	do { \
														std::cout << color; \
														TOOLS_DisplayTime(); \
														TOOLS_DisplayFuncName(__LINE__, __class__, __func__, libName); \
														std::cout << "[" << info << "] " << data; \
														std::cout << ETK_BASH_COLOR_NORMAL <<std::endl; \
													}while(0)
#define ETK_CRITICAL(libName, data)					ETK_DBG_COMMON(libName, ETK_BASH_COLOR_BOLD_RED, 'C', data)
#define ETK_WARNING(libName, data)					ETK_DBG_COMMON(libName, ETK_BASH_COLOR_MAGENTA,  'W', data)
#define ETK_ERROR(libName, data)					ETK_DBG_COMMON(libName, ETK_BASH_COLOR_BOLD_RED, 'E', data)
#define ETK_INFO(libName, data)						ETK_DBG_COMMON(libName, ETK_BASH_COLOR_CYAN,     'I', data)
#define ETK_DEBUG(libName, data)					ETK_DBG_COMMON(libName, ETK_BASH_COLOR_YELLOW,   'D', data)
#define ETK_VERBOSE(libName, data)					ETK_DBG_COMMON(libName, ETK_BASH_COLOR_WHITE,    'V', data)
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

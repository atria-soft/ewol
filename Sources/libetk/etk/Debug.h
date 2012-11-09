/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/Stream.h>
#include <etk/Types.h>

#ifndef __ETK_DEBUG_H__
#define __ETK_DEBUG_H__

// Log Message System For EDN
void TOOLS_DisplayFuncName(int32_t ligne, const char* className, const char* funcName, const char* libName);
void TOOLS_DisplayTime(void);


#undef  __class__
#define __class__				(NULL)

extern etk::logLevel_te g_requestedLevel;
void GeneralDebugSetLevel(etk::logLevel_te ccc);

#define ETK_DBG_COMMON(libName, info, data)			do { \
														if (info <= g_requestedLevel) { \
															etk::cout << etk::cstart << info; \
															TOOLS_DisplayTime(); \
															TOOLS_DisplayFuncName(__LINE__, __class__, __func__, libName); \
															etk::cout << data; \
															etk::cout <<etk::endl; \
														} \
													}while(0)


#define ETK_CRITICAL(libName, data)					ETK_DBG_COMMON(libName, etk::LOG_LEVEL_CRITICAL, data)

#if DEBUG_LEVEL > 0
#	define ETK_WARNING(libName, data)				ETK_DBG_COMMON(libName, etk::LOG_LEVEL_WARNING, data)
#	define ETK_ERROR(libName, data)					ETK_DBG_COMMON(libName, etk::LOG_LEVEL_ERROR, data)
#else
#	define ETK_WARNING(libName, data)				do {}while(0)
#	define ETK_ERROR(libName, data)					do {}while(0)
#endif

#if DEBUG_LEVEL > 1
#	define ETK_INFO(libName, data)					ETK_DBG_COMMON(libName, etk::LOG_LEVEL_INFO, data)
#else
#	define ETK_INFO(libName, data)					do {}while(0)
#endif

#if DEBUG_LEVEL > 2
#	define ETK_DEBUG(libName, data)					ETK_DBG_COMMON(libName, etk::LOG_LEVEL_DEBUG, data)
#else
#	define ETK_DEBUG(libName, data)					do {}while(0)
#endif

#if DEBUG_LEVEL > 3
#	define ETK_VERBOSE(libName, data)				ETK_DBG_COMMON(libName, etk::LOG_LEVEL_VERBOSE, data)
#else
#	define ETK_VERBOSE(libName, data)				do {}while(0)
#endif

#define ETK_ASSERT(libName, cond, data)				do { \
														if (!(cond)) { \
															ETK_CRITICAL(libName, data); \
															assert(!#cond); \
														} \
													} while (0)

#if DEBUG_LEVEL > 1
#   define ETK_CHECK_INOUT(libName, cond)			ETK_ASSERT(libName, (cond), "Internal input error : "#cond)
#elif DEBUG_LEVEL > 0
#   define ETK_CHECK_INOUT(libName, cond)			do { \
														if (!(cond)) { \
															ETK_CRITICAL(libName, "Internal input error : "#cond); \
														} \
													} while (0)
#else
#   define ETK_CHECK_INOUT(libName, cond)			do { } while (0)
#endif

#define ETK_TODO(libName, data)						ETK_INFO(libName, "TODO : " << data)

#endif

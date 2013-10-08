/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_DEBUG_H__
#define __EWOL_DEBUG_H__

#include <etk/types.h>
#include <etk/Debug.h>

extern const char * g_ewolLibName;

#define EWOL_CRITICAL(data)     ETK_CRITICAL(g_ewolLibName, data)
#define EWOL_WARNING(data)      ETK_WARNING(g_ewolLibName, data)
#define EWOL_ERROR(data)        ETK_ERROR(g_ewolLibName, data)
#define EWOL_INFO(data)         ETK_INFO(g_ewolLibName, data)
#define EWOL_DEBUG(data)        ETK_DEBUG(g_ewolLibName, data)
#define EWOL_VERBOSE(data)      ETK_VERBOSE(g_ewolLibName, data)
#define EWOL_ASSERT(cond, data) ETK_ASSERT(g_ewolLibName, cond, data)
#define EWOL_CHECK_INOUT(cond)  ETK_CHECK_INOUT(g_ewolLibName, cond)
#define EWOL_TODO(cond)         ETK_TODO(g_ewolLibName, cond)

#endif


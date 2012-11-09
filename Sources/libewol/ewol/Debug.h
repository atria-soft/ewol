/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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


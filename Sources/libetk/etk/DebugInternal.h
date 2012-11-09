/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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


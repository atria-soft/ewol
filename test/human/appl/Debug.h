/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <etk/types.h>
#include <etk/Debug.h>

extern const char * applLog;

#define APPL_CRITICAL(data)			ETK_CRITICAL(applLog, data)
#define APPL_WARNING(data)			ETK_WARNING(applLog, data)
#define APPL_ERROR(data)			ETK_ERROR(applLog, data)
#define APPL_INFO(data)				ETK_INFO(applLog, data)
#define APPL_DEBUG(data)			ETK_DEBUG(applLog, data)
#define APPL_VERBOSE(data)			ETK_VERBOSE(applLog, data)
#define APPL_ASSERT(cond, data)		ETK_ASSERT(applLog, cond, data)
#define APPL_CHECK_INOUT(cond)		ETK_CHECK_INOUT(applLog, cond)
#define APPL_TODO(cond)				ETK_TODO(applLog, cond)

#endif

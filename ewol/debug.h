/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <elog/log.h>

namespace ewol {
	int32_t getLogId();
};
#define EWOL_BASE(info,data) ELOG_BASE(ewol::getLogId(),info,data)

#define EWOL_PRINT(data)         EWOL_BASE(-1, data)
#define EWOL_CRITICAL(data)      EWOL_BASE(1, data)
#define EWOL_ERROR(data)         EWOL_BASE(2, data)
#define EWOL_WARNING(data)       EWOL_BASE(3, data)
#ifdef DEBUG
	#define EWOL_INFO(data)          EWOL_BASE(4, data)
	#define EWOL_DEBUG(data)         EWOL_BASE(5, data)
	#define EWOL_VERBOSE(data)       EWOL_BASE(6, data)
	#define EWOL_TODO(data)          EWOL_BASE(4, "TODO : " << data)
#else
	#define EWOL_INFO(data)          do { } while(false)
	#define EWOL_DEBUG(data)         do { } while(false)
	#define EWOL_VERBOSE(data)       do { } while(false)
	#define EWOL_TODO(data)          do { } while(false)
#endif

#define EWOL_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			EWOL_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)


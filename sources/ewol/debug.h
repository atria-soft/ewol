/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_DEBUG_H__
#define __EWOL_DEBUG_H__

#include <etk/log.h>

namespace ewol {
	int32_t getLogId(void);
};
// TODO : Review this problem of multiple intanciation of "std::stringbuf sb"
#define EWOL_BASE(info,data) \
	do { \
		if (info <= etk::log::getLevel(ewol::getLogId())) { \
			std::stringbuf sb; \
			std::ostream tmpStream(&sb); \
			tmpStream << data; \
			etk::log::logStream(ewol::getLogId(), info, __LINE__, __class__, __func__, tmpStream); \
		} \
	} while(0)

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

#endif


/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD 3 clauses (see license file)
 */

#ifndef __APPL_DEBUG_H__
#define __APPL_DEBUG_H__

#include <etk/log.h>

namespace appl {
	int32_t getLogId();
};
// TODO : Review this problem of multiple intanciation of "std::stringbuf sb"
#define APPL_BASE(info,data) \
	do { \
		if (info <= etk::log::getLevel(appl::getLogId())) { \
			std::stringbuf sb; \
			std::ostream tmpStream(&sb); \
			tmpStream << data; \
			etk::log::logStream(appl::getLogId(), info, __LINE__, __class__, __func__, tmpStream); \
		} \
	} while(0)

#define APPL_CRITICAL(data)      APPL_BASE(1, data)
#define APPL_ERROR(data)         APPL_BASE(2, data)
#define APPL_WARNING(data)       APPL_BASE(3, data)
#ifdef DEBUG
	#define APPL_INFO(data)          APPL_BASE(4, data)
	#define APPL_DEBUG(data)         APPL_BASE(5, data)
	#define APPL_VERBOSE(data)       APPL_BASE(6, data)
	#define APPL_TODO(data)          APPL_BASE(4, "TODO : " << data)
#else
	#define APPL_INFO(data)          do { } while(false)
	#define APPL_DEBUG(data)         do { } while(false)
	#define APPL_VERBOSE(data)       do { } while(false)
	#define APPL_TODO(data)          do { } while(false)
#endif

#define APPL_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			APPL_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)

#endif


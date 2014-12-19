/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_LUA_DEBUG_H__
#define __EWOL_LUA_DEBUG_H__

#include <etk/log.h>

namespace ewolLua {
	int32_t getLogId();
};
// TODO : Review this problem of multiple intanciation of "std::stringbuf sb"
#define EWOLLUA_BASE(info,data) \
	do { \
		if (info <= etk::log::getLevel(ewolLua::getLogId())) { \
			std::stringbuf sb; \
			std::ostream tmpStream(&sb); \
			tmpStream << data; \
			etk::log::logStream(ewolLua::getLogId(), info, __LINE__, __class__, __func__, tmpStream); \
		} \
	} while(0)

#define EWOLLUA_CRITICAL(data)      EWOLLUA_BASE(1, data)
#define EWOLLUA_ERROR(data)         EWOLLUA_BASE(2, data)
#define EWOLLUA_WARNING(data)       EWOLLUA_BASE(3, data)
#ifdef DEBUG
	#define EWOLLUA_INFO(data)          EWOLLUA_BASE(4, data)
	#define EWOLLUA_DEBUG(data)         EWOLLUA_BASE(5, data)
	#define EWOLLUA_VERBOSE(data)       EWOLLUA_BASE(6, data)
	#define EWOLLUA_TODO(data)          EWOLLUA_BASE(4, "TODO : " << data)
#else
	#define EWOLLUA_INFO(data)          do { } while(false)
	#define EWOLLUA_DEBUG(data)         do { } while(false)
	#define EWOLLUA_VERBOSE(data)       do { } while(false)
	#define EWOLLUA_TODO(data)          do { } while(false)
#endif

#define EWOLLUA_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			EWOLLUA_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)

#endif


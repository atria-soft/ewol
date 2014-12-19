/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_LUA_H__
#define __EWOL_LUA_H__

#include <etk/types.h>

struct lua_State;
namespace ewolLua {
	int32_t loadEwolLuaWrapper(lua_State* _L);
};

#endif


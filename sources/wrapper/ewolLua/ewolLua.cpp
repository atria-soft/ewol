/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include "ewolLua.h"
#include "object.h"
#include "objectManager.h"


int32_t ewolLua::loadEwolLuaWrapper(lua_State* _L) {
	loadObject(_L);
	loadObjectManager(_L);
	return 0;
}

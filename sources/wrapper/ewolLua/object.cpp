/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/object/Object.h>
#include <luaWrapper/luaWrapper.hpp>
#include "object.h"
#include "debug.h"


std::shared_ptr<ewol::Object> ewolObjectNew(lua_State *_L) {
	const char* name = luaL_checkstring(_L, 1);
	return ewol::Object::create(name);
}

static luaL_Reg s_metatable[] = {
	//{ "getIdFunc", luaU_get<ewol::Object, int32_t, &ewol::Object::getId> },
	//{ "NameFunc", luaU_getset<ewol::Object, std::string, &ewol::Object::getName, &ewol::Object::setName> },
	
	{ "getObjectType", luaU_func(&ewol::Object::getObjectType) },
	{ "getTypeDescription", luaU_func(&ewol::Object::getTypeDescription) },
	{ "isTypeCompatible", luaU_func(&ewol::Object::isTypeCompatible) },
	//{ "addObjectType", luaU_func(&ewol::Object::addObjectType) },
	{ "getStatic", luaU_func(&ewol::Object::getStatic) },
	{ NULL, NULL }
};

int32_t ewolLua::loadObject(lua_State* _L) {
	luaW_register<ewol::Object>(_L, "ewol__Object", NULL, s_metatable, ewolObjectNew);
	return 0;
}

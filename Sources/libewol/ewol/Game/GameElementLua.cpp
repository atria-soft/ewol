/**
 *******************************************************************************
 * @file ewol/Game/GameElementLua.cpp
 * @brief ewol game lua element system (Sources)
 * @author Edouard DUPIN
 * @date 30/05/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */


#include <ewol/Game/GameElementLua.h>

/*******************************************************************************
 ** Lua abstraction (START)
 ******************************************************************************* */

#include <lua/lua.hpp>

const char *metaname = "mine.type_t"; // associated with userdata of type type_t*

typedef struct {
    int a ;
    int b ;
}type_t;


int lib_a_f_4(type_t *t)
{
	return t->a * t->b ;
}

LUAMOD_API int lua_lib_a_f_4(lua_State *L) {
	type_t *t = (type_t *)luaL_checkudata(L, 1, metaname);  // check argument type
	lua_pushnumber(L, (lua_Number)lib_a_f_4(t));
	return 1;
}

LUAMOD_API int lua_new_t(lua_State *L) { // get Lua to allocate an initialize a type_t*
	int a = luaL_checkint(L, 1);
	int b = luaL_checkint(L, 2);
	type_t *t = (type_t *)lua_newuserdata(L, sizeof(*t));
	luaL_getmetatable(L, metaname);
	lua_setmetatable(L, -2);
	t->a = a;
	t->b = b;
	return 1;
}

static const luaL_Reg functionsTable[] = {
	{ "lib_a_f_4", lua_lib_a_f_4 },
	{ "new_t", lua_new_t },
	{ NULL, NULL }
};


//http://www.swig.org/Doc1.3/Lua.html#Lua_nn13
//http://stackoverflow.com/questions/2521244/how-to-wrap-a-c-function-whose-parameters-are-pointer-to-structs-so-that-it-can
LUAMOD_API int luaopen_myLib(lua_State *L) {
	luaL_register(L, "mylib", functionsTable);
	luaL_newmetatable(L, metaname);
	lua_pop(L, 1);
	//luaL_newlib(L, functionsTable);
	//lua_pop(L, 1);
	return 1;
}

/*******************************************************************************
 ** Lua abstraction (END)
 ******************************************************************************* */


ewol::GameElementLua::GameElementLua(ewol::SceneElement & sceneElement, etk::UString& tmpName, int32_t group) : 
	ewol::GameElement(sceneElement, tmpName),
	m_luaState(NULL)
{
	// create state
	m_luaState = luaL_newstate();
	if (m_luaState == NULL) {
		EWOL_ERROR("LUA: cannot create state: not enough memory");
		return;
	}
	luaL_openlibs(m_luaState);
	// open internal specific elements ...
	luaopen_myLib(m_luaState);
	
	if (luaL_loadfile(m_luaState, "/home/edupin/dev/perso/TethysDefender/last.lua") || lua_pcall(m_luaState, 0, 0, 0)) {
		EWOL_ERROR("LUA: " << lua_tostring(m_luaState, -1));
		return;
	}
	/*
	lua_getglobal(m_luaState, "f");
	if(!lua_isfunction(m_luaState,-1))
	{
		EWOL_ERROR("LUA: Not a function ... ");
		lua_pop(m_luaState,1);
		return;
	}
	lua_pushnumber(m_luaState, 21);   // push 1st argument
	lua_pushnumber(m_luaState, 31);   // push 2nd argument
	
	// do the call (2 arguments, 1 result)
	if (lua_pcall(m_luaState, 2, 1, 0) != 0) {
		EWOL_ERROR("LUA: error running function 'f':" << lua_tostring(m_luaState, -1));
		return;
	}
	
	// retrieve result
	if (!lua_isnumber(m_luaState, -1)) {
		EWOL_ERROR("LUA: function `f' must return a number");
		return;
	}
	double z = lua_tonumber(m_luaState, -1);
	EWOL_ERROR("LUA: Result:" << z);
	lua_pop(m_luaState, 1);
	*/
}


ewol::GameElementLua::~GameElementLua(void)
{
	if (NULL != m_luaState) {
		EWOL_DEBUG("RemoveLua Element");
		lua_close(m_luaState);
		m_luaState = NULL;
	}
}


bool ewol::GameElementLua::Process(int64_t time, int32_t deltaTime, ewol::SceneElement & sceneElement)
{
	return false;
}


void ewol::GameElementLua::Draw(etk::VectorType<ewol::Sprite*> & listOfSprite, etk::VectorType<ewol::Sprite*> & listOfEffects)
{
	
}


bool ewol::GameElementLua::HaveImpact(int32_t group, int32_t type, coord2D_ts position, etkFloat_t size)
{
	return false;
}


void ewol::GameElementLua::Explosion(int32_t group, int32_t type, coord2D_ts position, etkFloat_t pxAtenuation, etkFloat_t power)
{
	
}


void ewol::GameElementLua::Add(int32_t group, coord2D_ts position, coord2D_ts speed, int32_t groupEnemy, int32_t dificulty)
{
	
}


bool ewol::GameElementLua::GetElementProperty(ewol::gameElementGenericProperty_ts &element, int32_t id)
{
	return false;
}



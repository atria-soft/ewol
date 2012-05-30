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

//For every acces : 

static ewol::GameElementLua * tmpObj = NULL;

const char *metaname = "mine.type_t"; // associated with userdata of type type_t*

typedef struct {
    int a ;
    int b ;
}type_t;


int lib_a_f_4(type_t *t)
{
	return t->a * t->b ;
}




LUAMOD_API int GetPosX(lua_State *L)
{
	if (NULL==tmpObj) {
		lua_pushnumber(L, (lua_Number)0 );
		return 1;
	}
	coord2D_ts tmpPos = tmpObj->PositionGet();
	
	lua_pushnumber(L, (lua_Number)((int32_t)tmpPos.x) );
	
	return 1;
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
	{ "GetPosX", GetPosX },
	{ NULL, NULL }
};


//http://www.swig.org/Doc1.3/Lua.html#Lua_nn13
//http://stackoverflow.com/questions/2521244/how-to-wrap-a-c-function-whose-parameters-are-pointer-to-structs-so-that-it-can
LUAMOD_API int luaopen_myLib(lua_State *L) {
	luaL_register(L, "GameElement", functionsTable);
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
	tmpObj = this;
	etk::File fileElement(tmpName, etk::FILE_TYPE_DATA);
	etk::UString tmpCompleatName = fileElement.GetCompleateName();
	
	// create state
	m_luaState = luaL_newstate();
	if (m_luaState == NULL) {
		EWOL_ERROR("LUA: cannot create state: not enough memory");
		return;
	}
	luaL_openlibs(m_luaState);
	// open internal specific elements ...
	luaopen_myLib(m_luaState);
	
	int32_t fileSize = fileElement.Size();
	if (0==fileSize) {
		EWOL_ERROR("This file is empty : " << fileElement);
		return;
	}
	if (false == fileElement.fOpenRead()) {
		EWOL_ERROR("Can not open the file : " << fileElement);
		return;
	}
	// allocate data
	char * fileBuffer = new char[fileSize+5];
	if (NULL == fileBuffer) {
		EWOL_ERROR("Error Memory allocation size=" << fileSize);
		return;
	}
	memset(fileBuffer, 0, (fileSize+5)*sizeof(char));
	// load data from the file :
	fileElement.fRead(fileBuffer, 1, fileSize);
	// close the file:
	fileElement.fClose();
	
	if (luaL_loadbuffer(m_luaState, fileBuffer, fileSize, tmpName.Utf8Data())) {
	//if (luaL_loadfile(m_luaState, "/home/heero/dev/perso/TethysDefender/assets/elementGame/Cube.lua"  ) ) {
		EWOL_ERROR("LUA: " << lua_tostring(m_luaState, -1));
		return;
	}
	
	if (NULL != fileBuffer) {
		delete[] fileBuffer;
	}
	// Call the basic for initialise all the element defined by the user...
	if (lua_pcall(m_luaState, 0, 0, 0)) {
		EWOL_ERROR("LUA: " << lua_tostring(m_luaState, -1));
		return;
	}
	
	// call the init function
	lua_getglobal(m_luaState, "Init");
	if(!lua_isfunction(m_luaState,-1)) {
		EWOL_WARNING("LUA: Not Find Init function ");
		lua_pop(m_luaState,1);
	} else {
		// do the call (0 arguments, 0 result)
		if (lua_pcall(m_luaState, 0, 0, 0) != 0) {
			EWOL_ERROR("LUA: error running function 'Init':" << lua_tostring(m_luaState, -1));
			return;
		}
	}
	/*
	lua_getglobal(m_luaState, "f");
	if(!lua_isfunction(m_luaState,-1))
	{
		EWOL_WARNING("LUA: Not a function ... ");
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
	tmpObj = NULL;
}


ewol::GameElementLua::~GameElementLua(void)
{
	tmpObj = this;
	if (NULL != m_luaState) {
		// call the init function
		lua_getglobal(m_luaState, "UnInit");
		if(!lua_isfunction(m_luaState,-1)) {
			EWOL_WARNING("LUA: Not Find 'UnInit' function ");
			lua_pop(m_luaState,1);
		} else {
			// do the call (0 arguments, 0 result)
			if (lua_pcall(m_luaState, 0, 0, 0) != 0) {
				EWOL_ERROR("LUA: error running function 'UnInit':" << lua_tostring(m_luaState, -1));
			}
		}
		EWOL_DEBUG("RemoveLua Element");
		lua_close(m_luaState);
		m_luaState = NULL;
	}
	tmpObj = NULL;
}


bool ewol::GameElementLua::Process(int64_t time, int32_t deltaTime, ewol::SceneElement & sceneElement)
{
	tmpObj = this;
	bool retVal = false;
	if (NULL != m_luaState) {
		// call the init function
		lua_getglobal(m_luaState, "Process");
		if(!lua_isfunction(m_luaState,-1)) {
			EWOL_WARNING("LUA: Not Find 'Process' function ");
			lua_pop(m_luaState,1);
		} else {
			lua_pushnumber(m_luaState, (int32_t)time);   // push 1st argument
			lua_pushnumber(m_luaState, (int32_t)deltaTime);   // push 2nd argument
			// do the call (2 arguments, 1 result)
			if (lua_pcall(m_luaState, 2, 1, 0) != 0) {
				EWOL_ERROR("LUA: error running function 'Process':" << lua_tostring(m_luaState, -1));
			} else {
				// retrieve result
				if (!lua_isboolean(m_luaState, -1)) {
					EWOL_ERROR("LUA: function 'Process' must return a boolean");
				} else {
					retVal = lua_toboolean(m_luaState, -1);
					lua_pop(m_luaState, 1);
				}
			}
		}
	}
	tmpObj = NULL;
	
	return retVal;
}


void ewol::GameElementLua::Draw(etk::VectorType<ewol::Sprite*> & listOfSprite, etk::VectorType<ewol::Sprite*> & listOfEffects)
{
	tmpObj = this;
	if (NULL != m_luaState) {
		// call the init function
		lua_getglobal(m_luaState, "Draw");
		if(!lua_isfunction(m_luaState,-1)) {
			EWOL_WARNING("LUA: Not Find 'Draw' function ");
			lua_pop(m_luaState,1);
		} else {
			// do the call (0 arguments, 0 result)
			if (lua_pcall(m_luaState, 0, 0, 0) != 0) {
				EWOL_ERROR("LUA: error running function 'Draw':" << lua_tostring(m_luaState, -1));
			}
		}
	}
	tmpObj = NULL;
}


bool ewol::GameElementLua::HaveImpact(int32_t group, int32_t type, coord2D_ts position, etkFloat_t size)
{
	//HaveImpact(group, type, posX, posY, size)
	tmpObj = this;
	bool retVal = false;
	if (NULL != m_luaState) {
		// call the init function
		lua_getglobal(m_luaState, "Process");
		if(!lua_isfunction(m_luaState,-1)) {
			EWOL_WARNING("LUA: Not Find 'Process' function ");
			lua_pop(m_luaState,1);
		} else {
			lua_pushnumber(m_luaState, (int32_t)group);
			lua_pushnumber(m_luaState, (int32_t)type);
			lua_pushnumber(m_luaState, (int32_t)position.x);
			lua_pushnumber(m_luaState, (int32_t)position.y);
			lua_pushnumber(m_luaState, (int32_t)size);
			// do the call (5 arguments, 1 result)
			if (lua_pcall(m_luaState, 5, 1, 0) != 0) {
				EWOL_ERROR("LUA: error running function 'Process':" << lua_tostring(m_luaState, -1));
			} else {
				// retrieve result
				if (!lua_isboolean(m_luaState, -1)) {
					EWOL_ERROR("LUA: function 'Process' must return a boolean");
				} else {
					retVal = lua_toboolean(m_luaState, -1);
					lua_pop(m_luaState, 1);
				}
			}
		}
	}
	tmpObj = NULL;
	
	return retVal;
}


void ewol::GameElementLua::Explosion(int32_t group, int32_t type, coord2D_ts position, etkFloat_t pxAtenuation, etkFloat_t power)
{
	tmpObj = this;
	if (NULL != m_luaState) {
		// call the init function
		lua_getglobal(m_luaState, "Process");
		if(!lua_isfunction(m_luaState,-1)) {
			EWOL_WARNING("LUA: Not Find 'Process' function ");
			lua_pop(m_luaState,1);
		} else {
			lua_pushnumber(m_luaState, (int32_t)group);
			lua_pushnumber(m_luaState, (int32_t)type);
			lua_pushnumber(m_luaState, (int32_t)position.x);
			lua_pushnumber(m_luaState, (int32_t)position.y);
			lua_pushnumber(m_luaState, (int32_t)pxAtenuation);
			lua_pushnumber(m_luaState, (int32_t)power);
			// do the call (6 arguments, 0 result)
			if (lua_pcall(m_luaState, 6, 0, 0) != 0) {
				EWOL_ERROR("LUA: error running function 'Process':" << lua_tostring(m_luaState, -1));
			}
		}
	}
	tmpObj = NULL;
}



bool ewol::GameElementLua::GetElementProperty(ewol::gameElementGenericProperty_ts &element, int32_t id)
{
	return false;
}



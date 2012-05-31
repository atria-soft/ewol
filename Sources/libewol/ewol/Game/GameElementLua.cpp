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

static ewol::GameElementLua *           tmpObj = NULL;
static etk::VectorType<ewol::Sprite*> * tmpSprite = NULL;
static etk::VectorType<ewol::Sprite*> * tmpEffects = NULL;
static ewol::SceneElement *             tmpScene = NULL;


const char *metaname = "mine.type_t"; // associated with userdata of type type_t*

typedef struct {
    int a ;
    int b ;
}type_t;


int lib_a_f_4(type_t *t)
{
	return t->a * t->b ;
}




LUAMOD_API int lua_GetPos(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, (lua_Number)0 );
		lua_pushnumber(L, (lua_Number)0 );
		return 2;
	}
	coord2D_ts tmpPos = tmpObj->PositionGet();
	lua_pushnumber(L, (lua_Number)tmpPos.x );
	lua_pushnumber(L, (lua_Number)tmpPos.y );
	// return number of parameters
	return 2;
}

LUAMOD_API int lua_SetPos(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		return 0;
	}
	float x = luaL_checkint(L, 1);
	float y = luaL_checkint(L, 2);
	coord2D_ts tmpPos;
	tmpPos.x = x;
	tmpPos.y = y;
	tmpObj->PositionSet(tmpPos);
	// return number of parameters
	return 0;
}

LUAMOD_API int lua_GetSpeed(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, (lua_Number)0 );
		lua_pushnumber(L, (lua_Number)0 );
		return 2;
	}
	coord2D_ts tmpPos = tmpObj->SpeedGet();
	lua_pushnumber(L, (lua_Number)tmpPos.x );
	lua_pushnumber(L, (lua_Number)tmpPos.y );
	// return number of parameters
	return 2;
}

LUAMOD_API int lua_SetSpeed(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		return 0;
	}
	float x = luaL_checkint(L, 1);
	float y = luaL_checkint(L, 2);
	coord2D_ts tmpPos;
	tmpPos.x = x;
	tmpPos.y = y;
	tmpObj->SpeedSet(tmpPos);
	// return number of parameters
	return 0;
}


LUAMOD_API int lua_SpriteLoad(lua_State *L)
{
	//LUA : int SpriteLoad("fileName", maxSize); => -1 in error ...
	if (NULL==tmpScene || NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, (lua_Number)-1 );
		return 1;
	}
	const char *filename = luaL_checkstring(L, 1);
	int32_t maxSize = luaL_checkint(L, 2);
	coord2D_ts size;
	size.x = maxSize;
	size.y = size.x;
	int32_t spriteID = tmpObj->LoadSprite( tmpScene->animated, filename, size);
	if (spriteID < 0) {
		EWOL_ERROR("Error to load the sprite : " << filename);
	} else {
		EWOL_INFO("Load the sprite : " << filename << " ==> " << spriteID);
	}
	lua_pushnumber(L, (lua_Number)spriteID );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_SpriteUnLoad(lua_State *L)
{
/*
	//LUA : SpriteUnLoad(int);
	if (NULL==tmpScene) {
		return 1;
	}
	float a = luaL_checkint(L, 1);
	coord2D_ts tmpPos = tmpObj->PositionGet();
	tmpPos.y = a;
	tmpObj->PositionSet(tmpPos);
*/
	// return number of parameters
	return 0;
}

LUAMOD_API int lua_SpriteDraw(lua_State *L)
{
	//LUA : SpriteDraw(int id, posX, posY, angle, size)
	
	if (NULL==tmpSprite) {
		return 0;
	}
	float spriteID = luaL_checknumber(L, 1);
	coord2D_ts tmpPos;
	tmpPos.x = luaL_checknumber(L, 2);
	tmpPos.y = luaL_checknumber(L, 3);
	float angle = luaL_checknumber(L, 4);
	float size = luaL_checknumber(L, 5);
	if (spriteID < 0) {
		EWOL_ERROR("Can not Draw the sprite : " << spriteID);
		return 0;
	}
	//EWOL_INFO("Draw the sprite : " << spriteID << " pos=" << tmpPos << " angle=" << angle);
	(*tmpSprite)[spriteID]->Element(tmpPos, size, angle);
	// return number of parameters
	return 0;
}



LUAMOD_API int lua_lib_a_f_4(lua_State *L) {
	type_t *t = (type_t *)luaL_checkudata(L, 1, metaname);  // check argument type
	lua_pushnumber(L, (lua_Number)lib_a_f_4(t));
	// return number of parameters
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
	// return number of parameters
	return 1;
}

static const luaL_Reg functionsTable[] = {
	{ "lib_a_f_4",      lua_lib_a_f_4 },
	{ "new_t",          lua_new_t },
	{ "GetPos",         lua_GetPos },
	{ "SetPos",         lua_SetPos },
	{ "GetSpeed",       lua_GetSpeed },
	{ "SetSpeed",       lua_SetSpeed },
	{ "SpriteLoad",     lua_SpriteLoad },
	{ "SpriteUnLoad",   lua_SpriteUnLoad },
	{ "SpriteDraw",     lua_SpriteDraw },
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
	tmpScene = &m_sceneElement;
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
	tmpObj = NULL;
	tmpScene = NULL;
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
	tmpScene = &m_sceneElement;
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
	tmpScene = NULL;
	
	return retVal;
}


void ewol::GameElementLua::Draw(etk::VectorType<ewol::Sprite*> & listOfSprite, etk::VectorType<ewol::Sprite*> & listOfEffects)
{
	tmpObj = this;
	tmpSprite = &listOfSprite;
	tmpEffects = &listOfEffects;
	if (NULL != m_luaState) {
		// call the Draw function
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
	tmpSprite = NULL;
	tmpEffects = NULL;
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
			lua_pushnumber(m_luaState, group);
			lua_pushnumber(m_luaState, type);
			lua_pushnumber(m_luaState, position.x);
			lua_pushnumber(m_luaState, position.y);
			lua_pushnumber(m_luaState, size);
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
			lua_pushnumber(m_luaState, group);
			lua_pushnumber(m_luaState, type);
			lua_pushnumber(m_luaState, position.x);
			lua_pushnumber(m_luaState, position.y);
			lua_pushnumber(m_luaState, pxAtenuation);
			lua_pushnumber(m_luaState, power);
			// do the call (6 arguments, 0 result)
			if (lua_pcall(m_luaState, 6, 0, 0) != 0) {
				EWOL_ERROR("LUA: error running function 'Process':" << lua_tostring(m_luaState, -1));
			}
		}
	}
	tmpObj = NULL;
}

void ewol::GameElementLua::Message(etk::UString control, etk::UString message)
{
	tmpObj = this;
	if (NULL != m_luaState) {
		// call the init function
		lua_getglobal(m_luaState, "Message");
		if(!lua_isfunction(m_luaState,-1)) {
			lua_pop(m_luaState,1);
		} else {
			lua_pushstring(m_luaState, control.Utf8Data());
			lua_pushstring(m_luaState, message.Utf8Data());
			// do the call (2 arguments, 0 result)
			if (lua_pcall(m_luaState, 2, 0, 0) != 0) {
				EWOL_ERROR("LUA: error running function 'Message':" << lua_tostring(m_luaState, -1));
			}
		}
	}
	tmpObj = NULL;
}



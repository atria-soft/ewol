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

template <typename T> int index(lua_State* L);

template <> int index<bool>(lua_State* L)
{
	bool* ptr = (bool*)lua_touserdata(L, 1);
	lua_pushboolean(L, *ptr ? 1 : 0);
	return 1;
}

template <> int index<float>(lua_State* L)
{
	float* ptr = (float*)lua_touserdata(L, 1);
	lua_pushnumber(L, *ptr);
	return 1;
}

template <typename T> int newindex(lua_State* L);

template <> int newindex<bool>(lua_State* L)
{
	bool* ptr = (bool*)lua_touserdata(L, 1);
	*ptr = lua_toboolean(L, 3)!=0;
	return 0;
}

template <> int newindex<float>(lua_State* L)
{
	float* ptr = (float*)lua_touserdata(L, 1);
	if (!lua_isnumber(L, 3)) {
		return luaL_error(L, "new value must be a number");
	}
	*ptr = (float)lua_tonumber(L, 3);
	return 1;
}

template <typename T> class LuaValue
{
	private:
		lua_State* L;
		etk::UString name;
		T* ptr;
	public:
	LuaValue(lua_State* _L, etk::UString _name)
		: L(_L), name(_name), ptr(0)
	{
		ptr = (T*)lua_newuserdata(L, sizeof(T));
		*ptr = T();
		lua_createtable(L, 0, 2);
		lua_pushcfunction(L, index<T>);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, newindex<T>);
		lua_setfield(L, -2, "__newindex");
		lua_setmetatable(L, -2);
		lua_setglobal(L, name.Utf8Data());
	}
	virtual ~LuaValue(void)
	{
		lua_pushnil(L);
		lua_setglobal(L, name.Utf8Data());
		ptr = 0;
		L = 0;
	}
	LuaValue<T>& operator=(const T& value) { *ptr = value; return *this; }
	operator T() { return *ptr; }
};


/*
template <> int lua_Set<etkFloat_t>(lua_State* L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 1;
	}
	etkFloat_t value = luaL_checknumber(L, 1);
	tmpObj->AngleSet(value);
	// return number of parameters
	return 1;
}


template <> int lua_Get<etkFloat_t>(lua_State* L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 1;
	}
	etkFloat_t value = tmpObj->AngleGet();
	lua_pushnumber(L, value );
	// return number of parameters
	return 1;
}
*/

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
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);
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
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);
	coord2D_ts tmpPos;
	tmpPos.x = x;
	tmpPos.y = y;
	tmpObj->SpeedSet(tmpPos);
	// return number of parameters
	return 0;
}

LUAMOD_API int lua_GetPower(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 1;
	}
	int32_t value = tmpObj->PowerGet();
	lua_pushinteger(L, value );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_SetPower(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 1;
	}
	int32_t value = luaL_checkint(L, 1);
	tmpObj->PowerSet(value);
	// return number of parameters
	return 1;
}


LUAMOD_API int lua_GetGroup(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 1;
	}
	int32_t value = tmpObj->GroupGet();
	lua_pushinteger(L, value );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_SetGroup(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 1;
	}
	int32_t value = luaL_checkint(L, 1);
	tmpObj->GroupSet(value);
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_GetAngle(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 1;
	}
	etkFloat_t value = tmpObj->AngleGet();
	lua_pushnumber(L, value );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_SetAngle(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 1;
	}
	etkFloat_t value = luaL_checknumber(L, 1);
	tmpObj->AngleSet(value);
	// return number of parameters
	return 1;
}


LUAMOD_API int lua_GetSize(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 1;
	}
	etkFloat_t value = tmpObj->SizeGet();
	lua_pushnumber(L, value );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_SetSize(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushnumber(L, 0 );
		return 0;
	}
	etkFloat_t value = luaL_checknumber(L, 1);
	tmpObj->SizeSet(value);
	// return number of parameters
	return 0;
}


LUAMOD_API int lua_GetCanBeCibled(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushboolean(L, false );
		return 1;
	}
	etkFloat_t value = tmpObj->CanBeCibledGet();
	lua_pushboolean(L, value );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_SetCanBeCibled(lua_State *L)
{
	if (NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		return 0;
	}
	bool value = false;
	if ( lua_isboolean( L, 1 ) ) {
		value = lua_toboolean( L, 1 );
	}
	tmpObj->CanBeCibledSet(value);
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
	}
	lua_pushnumber(L, (lua_Number)spriteID );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_SpriteUnLoad(lua_State *L)
{
	// TODO : when supported ... 
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


LUAMOD_API int lua_ElementAdd(lua_State *L)
{
	//LUA : int ElementAdd("Bullet", group)
	
	if (NULL==tmpScene) {
		EWOL_ERROR("NULL obj...");
		lua_pushinteger(L, 0 );
		return 1;
	}
	etk::UString elementName = luaL_checkstring(L, 1);
	int32_t group = luaL_checkint(L, 2);
	// TODO : Remove this when find an other way do do it ...
	ewol::GameElementLua *           ttmpObj = tmpObj;
	etk::VectorType<ewol::Sprite*> * ttmpSprite = tmpSprite;
	etk::VectorType<ewol::Sprite*> * ttmpEffects = tmpEffects;
	ewol::SceneElement *             ttmpScene = tmpScene;
	uint32_t elementId = tmpScene->AddElementNamed(group, elementName);
	tmpObj = ttmpObj;
	tmpSprite = ttmpSprite;
	tmpEffects = ttmpEffects;
	tmpScene = ttmpScene;
	
	if (0==elementId) {
		EWOL_ERROR("Error creating element : " << elementName);
	}
	lua_pushinteger(L, elementId );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_ElementSetPos(lua_State *L)
{
	//LUA : ElementSetPos(newElementId, posX, posY);
	if (NULL==tmpScene) {
		EWOL_ERROR("ploppp ");
		return 0;
	}
	int32_t idElement = luaL_checkint(L, 1);
	ewol::GameElement* tmpElem = tmpScene->GetElement(idElement);
	if (NULL != tmpElem) {
		coord2D_ts tmpPos;
		tmpPos.x = luaL_checknumber(L, 2);
		tmpPos.y = luaL_checknumber(L, 3);
		tmpElem->PositionSet(tmpPos);
	} else {
		EWOL_ERROR("Get element unique ID : " << idElement);
	}
	// return number of parameters
	return 0;
}

LUAMOD_API int lua_ElementGetPos(lua_State *L)
{
	//LUA : ElementSetPos(newElementId, posX, posY);
	if (NULL==tmpScene) {
		EWOL_ERROR("ploppp ");
		lua_pushnumber(L, (lua_Number)0 );
		lua_pushnumber(L, (lua_Number)0 );
		return 2;
	}
	int32_t idElement = luaL_checkint(L, 1);
	ewol::GameElement* tmpElem = tmpScene->GetElement(idElement);
	if (NULL != tmpElem) {
		coord2D_ts tmpPos = tmpElem->PositionGet();
		lua_pushnumber(L, (lua_Number)tmpPos.x );
		lua_pushnumber(L, (lua_Number)tmpPos.y );
	} else {
		lua_pushnumber(L, (lua_Number)0.0 );
		lua_pushnumber(L, (lua_Number)0.0 );
		EWOL_ERROR("Get element unique ID : " << idElement);
	}
	// return number of parameters
	return 2;
}

LUAMOD_API int lua_ElementSetPower(lua_State *L)
{
	//LUA : ElementSetPower(newElementId, 1);
	
	if (NULL==tmpScene) {
		return 0;
	}
	int32_t idElement = luaL_checkint(L, 1);
	ewol::GameElement* tmpElem = tmpScene->GetElement(idElement);
	if (NULL != tmpElem) {
		int32_t power = luaL_checkint(L, 2);
		tmpElem->PowerSet(power);
	}
	// return number of parameters
	return 0;
}

LUAMOD_API int lua_ElementSetAngle(lua_State *L)
{
	//LUA : ElementSetAngle(newElementId, m_angle);
	
	if (NULL==tmpScene) {
		return 0;
	}
	int32_t idElement = luaL_checkint(L, 1);
	ewol::GameElement* tmpElem = tmpScene->GetElement(idElement);
	if (NULL != tmpElem) {
		float angle = luaL_checknumber(L, 2);
		tmpElem->AngleSet(angle);
	}
	// return number of parameters
	return 1;
}


LUAMOD_API int lua_GetNearestEnemy(lua_State *L)
{
	//LUA : int GetNearestEnemy()
	if (NULL==tmpScene || NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushinteger(L, 0 );
		return 1;
	}
	uint32_t elementId = tmpScene->GetNearestEnemy(tmpObj->PositionGet(), tmpObj->GroupGet());
	lua_pushinteger(L, elementId );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_ElmentExisted(lua_State *L)
{
	//LUA : ElementSetPos(newElementId, posX, posY);
	if (NULL==tmpScene) {
		EWOL_ERROR("NULL obj...");
		lua_pushboolean(L, false );
		return 1;
	}
	int32_t idElement = luaL_checkint(L, 1);
	if (0 != idElement) {
		ewol::GameElement* tmpElem = tmpScene->GetElement(idElement);
		if (NULL != tmpElem) {
			lua_pushboolean(L, true );
		} else {
			lua_pushboolean(L, false );
		}
	} else {
		lua_pushboolean(L, false );
	}
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_HaveImpact(lua_State *L)
{
	//LUA : ElementSetPos(newElementId, posX, posY);
	if (NULL==tmpScene || NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		lua_pushboolean(L, false );
		return 1;
	}

	// TODO : Remove this when find an other way do do it ...
	ewol::GameElementLua *           ttmpObj = tmpObj;
	etk::VectorType<ewol::Sprite*> * ttmpSprite = tmpSprite;
	etk::VectorType<ewol::Sprite*> * ttmpEffects = tmpEffects;
	ewol::SceneElement *             ttmpScene = tmpScene;

	bool result = tmpScene->HaveImpact(tmpObj->GroupGet(), tmpObj->TypeGet(), tmpObj->PositionGet(), tmpObj->SizeGet());

	tmpObj = ttmpObj;
	tmpSprite = ttmpSprite;
	tmpEffects = ttmpEffects;
	tmpScene = ttmpScene;

	lua_pushboolean(L, result );
	// return number of parameters
	return 1;
}

LUAMOD_API int lua_Explosion(lua_State *L)
{
	//LUA : ElementSetPos(newElementId, posX, posY);
	if (NULL==tmpScene || NULL==tmpObj) {
		EWOL_ERROR("NULL obj...");
		return 0;
	}

	// TODO : Remove this when find an other way do do it ...
	ewol::GameElementLua *           ttmpObj = tmpObj;
	etk::VectorType<ewol::Sprite*> * ttmpSprite = tmpSprite;
	etk::VectorType<ewol::Sprite*> * ttmpEffects = tmpEffects;
	ewol::SceneElement *             ttmpScene = tmpScene;

	tmpScene->Explosion(tmpObj->GroupGet(), tmpObj->TypeGet(), tmpObj->PositionGet(), 0.01, tmpObj->PowerGet());

	tmpObj = ttmpObj;
	tmpSprite = ttmpSprite;
	tmpEffects = ttmpEffects;
	tmpScene = ttmpScene;

	// return number of parameters
	return 0;
}


static const luaL_Reg functionsTable[] = {
	// local element section
	{ "GetPos",          lua_GetPos },
	{ "SetPos",          lua_SetPos },
	{ "GetSpeed",        lua_GetSpeed },
	{ "SetSpeed",        lua_SetSpeed },
	{ "GetAngle",        lua_GetAngle },
	{ "SetAngle",        lua_SetAngle },
	{ "GetSize",         lua_GetSize },
	{ "SetSize",         lua_SetSize },
	{ "GetPower",        lua_GetPower },
	{ "SetPower",        lua_SetPower },
	{ "GetGroup",        lua_GetGroup },
	{ "SetGroup",        lua_SetGroup },
	{ "GetCanBeCibled",  lua_GetCanBeCibled },
	{ "SetCanBeCibled",  lua_SetCanBeCibled },
	// other element section
	{ "ElementAdd",      lua_ElementAdd },
	{ "ElementExisted",  lua_ElmentExisted },
	{ "ElementSetPos",   lua_ElementSetPos },
	{ "ElementGetPos",   lua_ElementGetPos },
	{ "ElementSetPower", lua_ElementSetPower },
	{ "ElementSetAngle", lua_ElementSetAngle },
	{ "GetNearestEnemy", lua_GetNearestEnemy },
	{ "HaveImpact",      lua_HaveImpact },
	{ "Explosion",       lua_Explosion },
	// Sprite section
	{ "SpriteLoad",      lua_SpriteLoad },
	{ "SpriteUnLoad",    lua_SpriteUnLoad },
	{ "SpriteDraw",      lua_SpriteDraw },
	{ NULL, NULL }
};


//http://www.swig.org/Doc1.3/Lua.html#Lua_nn13
//http://stackoverflow.com/questions/2521244/how-to-wrap-a-c-function-whose-parameters-are-pointer-to-structs-so-that-it-can
LUAMOD_API int luaopen_myLib(lua_State *L) {
	luaL_register(L, "GameElement", functionsTable);
	lua_pop(L, 1);
	return 1;
}

/*******************************************************************************
 ** Lua abstraction (END)
 ******************************************************************************* */

#undef __class__
#define __class__	"GameElementLua"

ewol::GameElementLua::GameElementLua(ewol::SceneElement & sceneElement, etk::UString& tmpName) : 
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
/*
	LuaValue<bool> *myBool = new LuaValue<bool>(m_luaState, "m_testBool");
	LuaValue<float> *myValue = new LuaValue<float>(m_luaState, "m_testFloat");
	*myBool = false;
	*myValue = 18;
*/
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
/*
	EWOL_INFO("retreave element : " << *myValue << " and : " << *myBool);
*/
	tmpObj = NULL;
	tmpScene = NULL;
	Init();
	
}


ewol::GameElementLua::~GameElementLua(void)
{
	UnInit();
	if (NULL != m_luaState) {
		lua_close(m_luaState);
		m_luaState = NULL;
	}
}

void ewol::GameElementLua::Init(void)
{
	tmpObj = this;
	tmpScene = &m_sceneElement;
	if (NULL != m_luaState) {
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
	}
	tmpObj = NULL;
	tmpScene = NULL;
}


void ewol::GameElementLua::UnInit(void)
{
	tmpObj = this;
	tmpScene = &m_sceneElement;
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
	}
	tmpObj = NULL;
	tmpScene = NULL;
}


bool ewol::GameElementLua::Process(int64_t time, int32_t deltaTime)
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
	// todo set a flag that permit lua to direct control of this ...
	if (false == ewol::GameElement::HaveImpact(group, type, position, size) ) {
		return false;
	}
	//HaveImpact(group, type, posX, posY, size, quadDistance)
	tmpObj = this;
	bool retVal = true;
	if (NULL != m_luaState) {
		// call the init function
		lua_getglobal(m_luaState, "HaveImpact");
		if(!lua_isfunction(m_luaState,-1)) {
			EWOL_VERBOSE("LUA: Not Find 'HaveImpact' function ");
			lua_pop(m_luaState,1);
		} else {
			lua_pushnumber(m_luaState, group);
			lua_pushnumber(m_luaState, type);
			lua_pushnumber(m_luaState, position.x);
			lua_pushnumber(m_luaState, position.y);
			lua_pushnumber(m_luaState, size);
			etkFloat_t quadDistance = quadDist(m_position, position);
			lua_pushnumber(m_luaState, quadDistance);
			// do the call (6 arguments, 1 result)
			if (lua_pcall(m_luaState, 6, 1, 0) != 0) {
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


bool ewol::GameElementLua::Explosion(int32_t group, int32_t type, coord2D_ts position, etkFloat_t pxAtenuation, etkFloat_t power)
{
	tmpObj = this;
	bool retVal = false;
	if (NULL != m_luaState) {
		// call the init function
		lua_getglobal(m_luaState, "Explosion");
		if(!lua_isfunction(m_luaState,-1)) {
			EWOL_VERBOSE("LUA: Not Find 'Explosion' function ");
			lua_pop(m_luaState,1);
		} else {
			lua_pushnumber(m_luaState, group);
			lua_pushnumber(m_luaState, type);
			lua_pushnumber(m_luaState, position.x);
			lua_pushnumber(m_luaState, position.y);
			lua_pushnumber(m_luaState, pxAtenuation);
			lua_pushnumber(m_luaState, power);
			etkFloat_t quadDistance = quadDist(m_position, position);
			lua_pushnumber(m_luaState, quadDistance);
			// do the call (7 arguments, 1 result)
			if (lua_pcall(m_luaState, 7, 1, 0) != 0) {
				EWOL_ERROR("LUA: error running function 'Explosion':" << lua_tostring(m_luaState, -1));
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

static ewol::GameElement* LoadSceneElement_lua(ewol::SceneElement & sceneElement, etk::UString& elementName, etk::UString& userString)
{
	// try to find the file :
	etk::UString tmpName = userString;
	tmpName += elementName;
	tmpName += ".lua";
	// added a new element :
	etk::File fileElement(tmpName, etk::FILE_TYPE_DATA);
	if (false == fileElement.Exist()) {
		EWOL_ERROR("Can not find Game element : " << elementName << " ==> " << tmpName);
		return NULL;
	}
	EWOL_VERBOSE("We find Game element : " << elementName << " ==> " << tmpName);
	ewol::GameElementLua * tmpElement = new ewol::GameElementLua(sceneElement, tmpName);
	if (NULL == tmpElement) {
		EWOL_ERROR("Can not Allocat : " << elementName);
		return NULL;
	}
	return tmpElement;
}

void ewol::RegisterLuaElementInFolder(ewol::SceneElement & sceneElement, etk::UString folder)
{
	// TODO : parsing the folder ...
	sceneElement.RegisterElementType("??????", &LoadSceneElement_lua, folder);
}


void ewol::RegisterLuaElementSpecify(ewol::SceneElement & sceneElement, etk::UString folder, etk::UString name)
{
	sceneElement.RegisterElementType(name, &LoadSceneElement_lua, folder);
}

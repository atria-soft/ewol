/**
 *******************************************************************************
 * @file ewol/Game/GameElement.cpp
 * @brief ewol game element system (Sources)
 * @author Edouard DUPIN
 * @date 06/04/2012
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


#include <ewol/Game/GameElement.h>


/**
 * @brief Constructor : here are requested all the needed sprite and effect that can be used in the game
 * @param ---
 * @return ---
 */
ewol::GameElement::GameElement(SceneElement & sceneElement, etk::UString& tmpName) : m_sceneElement(sceneElement), m_fileNameConfig(tmpName)
{
	m_group = -1;
	m_type = -1;
	m_visible = true;
	m_position.x = 0.0;
	m_position.y = 0.0;
	m_speed.x = 0.0;
	m_speed.y = 0.0;
	m_size = 64.0;
	m_angle = 0.0;
	m_gravity = 0.0;
}


/**
 * @brief Requuest the draw of the current element, it will be done on the current Sprite list
 * @param[in,out] listOfElement Reference on the list of sprite that we need to find if it exist or added a new one
 * @param[in] fileName Sprite name
 * @param[in] maxSize maximum size of the sprite
 * @return the id of the sprite requested or -1 if it does not existed
 */
int32_t ewol::GameElement::LoadSprite(etk::VectorType<ewol::Sprite*> listOfElement[NB_BOUBLE_BUFFER], etk::UString fileName, coord2D_ts maxSize)
{
	for (int32_t iii=0; iii<listOfElement[0].Size(); iii++) {
		if (listOfElement[0][iii] != NULL) {
			if (listOfElement[0][iii]->HasName(fileName) == true) {
				// TODO : control the size ...
				return iii;
			}
		}
	}
	for(int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		// we did not find the sprite ==> created it ...
		ewol::Sprite* tmpSprite = new ewol::Sprite(fileName, maxSize.x, maxSize.y);
		if (NULL == tmpSprite) {
			EWOL_ERROR("Allocation error on the sprite : " << fileName);
			return -1;
		}
		// add it : 
		listOfElement[iii].PushBack(tmpSprite);
	}
	return listOfElement[0].Size() -1;
}


etkFloat_t quadDist(coord2D_ts pos1, coord2D_ts pos2)
{
	etkFloat_t tmpVal1 = pos1.x - pos2.x;
	etkFloat_t tmpVal2 = pos1.y - pos2.y;
	
	return tmpVal1*tmpVal1 + tmpVal2*tmpVal2;
}


#include <lua/lua.hpp>
extern "C" {
#include <lua/lauxlib.h>
}
const char *metaname = "mine.type_t"; // associated with userdata of type type_t*

typedef struct {
    int a ;
    int b ;
}type_t;


int lib_a_f_4(type_t *t)
{
     return t->a * t->b ;
}

static int lua_lib_a_f_4(lua_State *L) {
  type_t *t = (type_t *)luaL_checkudata(L, 1, metaname);  // check argument type
  lua_pushnumber(L, (lua_Number)lib_a_f_4(t));
  return 1;
}

static int lua_new_t(lua_State *L) { // get Lua to allocate an initialize a type_t*
  int a = luaL_checkint(L, 1);
  int b = luaL_checkint(L, 2);
  type_t *t = (type_t *)lua_newuserdata(L, sizeof(*t));
  luaL_getmetatable(L, metaname);
  lua_setmetatable(L, -2);
  t->a = a;
  t->b = b;
  return 1;
}

static const struct luaL_reg functions[] = {
  { "lib_a_f_4", lua_lib_a_f_4 },
  { "new_t", lua_new_t },
  { NULL, NULL }
};
//http://www.swig.org/Doc1.3/Lua.html#Lua_nn13
//http://stackoverflow.com/questions/2521244/how-to-wrap-a-c-function-whose-parameters-are-pointer-to-structs-so-that-it-can
int mylib_open(lua_State *L) {
  luaL_register(L, "mylib", functions);
  luaL_newmetatable(L, metaname);
  lua_pop(L, 1);
  //luaL_newlib(L, functionsTable);
  //lua_pop(L, 1);
  return 1;
}

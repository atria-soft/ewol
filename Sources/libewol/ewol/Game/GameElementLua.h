/**
 *******************************************************************************
 * @file ewol/Game/GameElementLua.h
 * @brief ewol game lua element system (header)
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

#ifndef __EWOL_GAME_ELEMENT_LUA_H__
#define __EWOL_GAME_ELEMENT_LUA_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/OObject/Sprite.h>
#include <ewol/Widget.h>
#include <ewol/Game/SceneElement.h>
#include <ewol/Game/GameElement.h>

#include <lua/lua.hpp>

namespace ewol {
	class GameElementLua : public ewol::GameElement
	{
		private:
			lua_State *m_luaState;  // internal Lua state
		public:
			GameElementLua(ewol::SceneElement & sceneElement, etk::UString& tmpName);
			~GameElementLua(void);
			
			virtual void Init(void);
			virtual void UnInit(void);
			virtual bool Process(int64_t time, int32_t deltaTime);
			virtual void Draw(etk::VectorType<ewol::Sprite*> & listOfSprite, etk::VectorType<ewol::Sprite*> & listOfEffects);
			virtual bool HaveImpact(int32_t group, int32_t type, Vector2D<float>  position, etkFloat_t size);
			virtual bool Explosion(int32_t group, int32_t type, Vector2D<float>  position, etkFloat_t pxAtenuation, etkFloat_t power);
			virtual void Message(etk::UString control, etk::UString message);
	};
	void RegisterLuaElementInFolder(ewol::SceneElement & sceneElement, etk::UString folder);
	void RegisterLuaElementSpecify(ewol::SceneElement & sceneElement, etk::UString folder, etk::UString name);
};

#endif

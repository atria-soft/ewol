/**
 *******************************************************************************
 * @file ewol/Game/GameElement.h
 * @brief ewol game element system (header)
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

#ifndef __EWOL_GAME_ELEMENT_H__
#define __EWOL_GAME_ELEMENT_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/OObject/Sprite.h>
#include <ewol/Widget.h>
#include <ewol/Game/SceneElement.h>

namespace ewol {
	
	class GameElement
	{
		private:
			etk::UString m_fileNameConfig;
		protected:
			SceneElement & m_sceneElement; //!< all element neede in the scene
			int32_t      m_group;
			int32_t      m_type;
			bool         m_visible;
			coord2D_ts   m_position;
			coord2D_ts   m_speed;
			etkFloat_t   m_size;
			etkFloat_t   m_angle;
			etkFloat_t   m_gravity;
		public:
			/**
			 * @brief Constructor : here are requested all the needed sprite and effect that can be used in the game
			 * @param ---
			 * @return ---
			 */
			GameElement(SceneElement & sceneElement, etk::UString& tmpName);
			/**
			 * @brief Destructor : This does not remove the sprite requested, they will be supressed when the scene is removed ...
			 * @param ---
			 * @return ---
			 */
			virtual ~GameElement(void) { };
			
			bool        HasName(etk::UString tmpName)                  { return (tmpName == m_fileNameConfig); };
			bool        IsVisible(void)                                { return m_visible; };
			void        SetVisible(bool state)                         { m_visible = state; };
			coord2D_ts  PositionGet(void)                              { return m_position; };
			void        PositionSet(coord2D_ts state)                  { m_position = state; };
			void        PositionSet(etkFloat_t xxx, etkFloat_t yyy)    { m_position.x = xxx; m_position.y = yyy; };
			coord2D_ts  SpeedGet(void)                                 { return m_speed; };
			void        SpeedSet(coord2D_ts state)                     { m_speed = state; };
			void        SpeedSet(etkFloat_t xxx, etkFloat_t yyy)       { m_speed.x = xxx; m_speed.y = yyy; };
			etkFloat_t  SizeGet(void)                                  { return m_size; };
			void        SizeSet(etkFloat_t state)                      { m_size = state; };
			etkFloat_t  AngleGet(void)                                 { return m_angle; };
			void        AngleSet(etkFloat_t state)                     { m_angle = state; };
			etkFloat_t  GravityGet(void)                               { return m_gravity; };
			void        GravitySet(etkFloat_t state)                   { m_gravity = state; };
			
			int32_t     GetType(void)                                  { return m_type; };
			int32_t     GroupGet(void)                                 { return m_group; };
			void        GroupSet(int32_t state)                        { m_group = state; };
			
			virtual bool GetElementProperty(gameElementGenericProperty_ts &element, int32_t id) {return false;};
			virtual int32_t GetNearestEnemy(coord2D_ts position, etkFloat_t& lastQuadDistance) { return -1;};
			/**
			 * @brief Periodicly this fuction will be call tu change property of all the dynamic obbjects
			 * @param[in] time Current game time (start at 0)
			 * @param[in] deltaTime delta time before the previous call
			 * @param[in,out] listOfSprite Reference on the list where the display must be done for every sprite
			 * @param[in,out] listOfEffects Reference on the list where the display must be done for every effects
			 * @return true if the object must be remove
			 * @return false if the object must be keep
			 */
			virtual bool Process(int64_t time, int32_t deltaTime, ewol::SceneElement & sceneElement) { return false; };
			
			/**
			 * @brief Requuest the draw of the current element, it will be done on the current Sprite list
			 * @param[in,out] listOfSprite Reference on the list where the display must be done for every sprite
			 * @param[in,out] listOfEffects Reference on the list where the display must be done for every effects
			 * @return ---
			 */
			virtual void Draw(etk::VectorType<ewol::Sprite*> & listOfSprite, etk::VectorType<ewol::Sprite*> & listOfEffects) { };
			/**
			 * @brief an element has been remove, just remove reference on it or ID on IT, it can be replace whith an other that have no link
			 * @param[in] idOfElement Id of the element that has been removed
			 * @return ---
			 */
			virtual void RemoveElement(int32_t idOfElement) { };
			virtual bool HaveImpact(int32_t group, int32_t type, coord2D_ts position, etkFloat_t size) {return false;};
			virtual void Explosion(int32_t group, int32_t type, coord2D_ts position, etkFloat_t pxAtenuation, etkFloat_t power) { } ;
			/**
			 * @brief Requuest the draw of the current element, it will be done on the current Sprite list
			 * @param[in,out] listOfElement Reference on the list of sprite that we need to find if it exist or added a new one
			 * @param[in] fileName Sprite name
			 * @param[in] maxSize maximum size of the sprite
			 * @return the id of the sprite requested or -1 if it does not existed
			 */
			int32_t LoadSprite(etk::VectorType<ewol::Sprite*> listOfElement[NB_BOUBLE_BUFFER], etk::UString fileName, coord2D_ts maxSize);
	};
	
};

#include <ewol/widget/Scene.h>

etkFloat_t quadDist(coord2D_ts pos1, coord2D_ts pos2);

#include <lua/lua.hpp>

extern "C" {
LUAMOD_API int luaopen_myLib(lua_State *L);
}
#endif

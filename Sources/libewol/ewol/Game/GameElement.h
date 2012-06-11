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

#define CYCLIC_CALL_PERIODE_US     (10000)

namespace ewol {
	
	class GameElement
	{
		private:
			etk::UString m_fileNameConfig;
		protected:
			SceneElement & m_sceneElement; //!< all element neede in the scene
			uint16_t     m_uniqueId;
			uint16_t     m_group;
			int32_t      m_type;
			int32_t      m_power;
			bool         m_visible;
			Vector2D<float>    m_position;
			Vector2D<float>    m_speed;
			float   m_size;
			float   m_angle;
			float   m_gravity;
			bool         m_canBeCibled;
			float   m_life;
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
			
			virtual void Init(void) { };
			virtual void UnInit(void) { };
			
			uint16_t    GetUniqueId(void)                              { return m_uniqueId; };
			
			bool        HasName(etk::UString tmpName)                  { return (tmpName == m_fileNameConfig); };
			bool        IsVisible(void)                                { return m_visible; };
			void        SetVisible(bool state)                         { m_visible = state; };
			Vector2D<float>   PositionGet(void)                              { return m_position; };
			void        PositionSet(Vector2D<float>  state)                  { m_position = state; };
			void        PositionSet(float xxx, float yyy)    { m_position.x = xxx; m_position.y = yyy; };
			Vector2D<float>   SpeedGet(void)                                 { return m_speed; };
			void        SpeedSet(Vector2D<float>  state)                     { m_speed = state; };
			void        SpeedSet(float xxx, float yyy)       { m_speed.x = xxx; m_speed.y = yyy; };
			float  SizeGet(void)                                  { return m_size; };
			void        SizeSet(float state)                      { m_size = state; };
			float  AngleGet(void)                                 { return m_angle; };
			void        AngleSet(float state)                     { m_angle = state; };
			float  GravityGet(void)                               { return m_gravity; };
			void        GravitySet(float state)                   { m_gravity = state; };
			int32_t     PowerGet(void)                                 { return m_power; };
			void        PowerSet(int32_t state)                        { m_power = state; };
			bool        CanBeCibledGet(void)                           { return m_canBeCibled; };
			void        CanBeCibledSet(bool state)                     { m_canBeCibled = state; };
			
			int32_t     GetType(void)                                  { return m_type; }; // DEPRECATED ...
			int32_t     TypeGet(void)                                  { return m_type; };
			uint16_t    GroupGet(void)                                 { return m_group; };
			void        GroupSet(uint16_t state)                       { m_group = state; };
			
			void GetElementProperty(gameElementGenericProperty_ts &element);
			/**
			 * @brief Periodicly this fuction will be call tu change property of all the dynamic obbjects
			 * @param[in] time Current game time (start at 0)
			 * @param[in] deltaTime delta time before the previous call
			 * @param[in,out] listOfSprite Reference on the list where the display must be done for every sprite
			 * @param[in,out] listOfEffects Reference on the list where the display must be done for every effects
			 * @return true if the object must be remove
			 * @return false if the object must be keep
			 */
			virtual bool Process(int64_t time, int32_t deltaTime) { return false; };
			
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
			virtual bool HaveImpact(int32_t group, int32_t type, Vector2D<float>  position, float size);
			virtual bool Explosion(int32_t group, int32_t type, Vector2D<float>  position, float pxAtenuation, float power) { return false; } ;
			/**
			 * @brief Requuest the draw of the current element, it will be done on the current Sprite list
			 * @param[in,out] listOfElement Reference on the list of sprite that we need to find if it exist or added a new one
			 * @param[in] fileName Sprite name
			 * @param[in] maxSize maximum size of the sprite
			 * @return the id of the sprite requested or -1 if it does not existed
			 */
			int32_t LoadSprite(etk::VectorType<ewol::Sprite*> listOfElement[NB_BOUBLE_BUFFER], etk::UString fileName, Vector2D<float>  maxSize);
			
			virtual void Message(etk::UString control, etk::UString message) { } ;
	};
	
};

#include <ewol/widget/Scene.h>

float quadDist(Vector2D<float>  pos1, Vector2D<float>  pos2);

#endif

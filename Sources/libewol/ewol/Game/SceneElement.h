/**
 *******************************************************************************
 * @file ewol/Game/SceneElement.h
 * @brief ewol Scene widget system (header)
 * @author Edouard DUPIN
 * @date 01/04/2012
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

#ifndef __EWOL_SCENE_ELEMENT_H__
#define __EWOL_SCENE_ELEMENT_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/OObject/Sprite.h>

namespace ewol {
	class GameElement;
	class SceneElement;
	
	typedef ewol::GameElement* (creatorElement_tf)(SceneElement & sceneElement, etk::UString& tmpName, etk::UString& userString);
	
	class listRegisteElement {
		public:
			etk::UString            name;          //!< name of an element
			etk::UString            userString;    //!< additionnal sting defined by the user
			creatorElement_tf *     loadElement;   //!< callback function to create it
	};
	#define MAX_GROUP_NUMBER	(32)
	
	class SceneElement {
		private:
			int16_t                             m_id;                                             //!< Unique element ID ==> to reference the element unicly
			int32_t retreviveElement;
			int32_t allocatedElements;
		public:
			SceneElement(void);
			~SceneElement(void);
			int32_t                              numberOfGroup;                                    //!< curent scene number of group
			etk::UString                         groupDescription[MAX_GROUP_NUMBER];               //!< name of all the groups
			int32_t                              groupEnemy[MAX_GROUP_NUMBER][MAX_GROUP_NUMBER];   //!< list of the ennemy
			etk::Vector<ewol::Sprite*>       animated;                                         //!< element that must be display the second
			etk::Vector<ewol::GameElement*>  listAnimatedElements[MAX_GROUP_NUMBER];           //!< generic element to display order in the diffferent group
			etk::Vector<ewol::GameElement*>  listGarbage;                                      //!< garbage of the old element allocated ==> prevent multiple alloc and free
			etk::Vector<listRegisteElement*> listCreatorElement;                               //!< list of all creatable elements
			int16_t                GetUniqueId(void) { int16_t iddd = m_id; m_id++; return iddd; };
			void                   RegisterElementType(etk::UString name, creatorElement_tf * loadElement, etk::UString userString);
			void                   RmElement(int32_t group, int32_t idElement);
			uint32_t               AddElement(int32_t group, ewol::GameElement* newElement);
			uint32_t               AddElementNamed(int32_t group, etk::UString &elementName);
			ewol::GameElement*     GetElement(uint32_t idElement);
			uint32_t               GetNearestEnemy(Vector2D<float>  position, int32_t groupId);
			bool                   HaveImpact(int32_t group, int32_t type, Vector2D<float>  position, float size);
			void                   Explosion(int32_t group, int32_t type, Vector2D<float>  position, float pxAtenuation, float power);
			uint32_t               GetElementAtPos(Vector2D<float>  position, int32_t maxDistanceDetection);
			void                   SetEventInput(uint32_t id, Vector2D<float>  position);
			void                   SetEventExternButton(uint32_t id, int32_t btId, int32_t state);
			void                   SetEventExternJoystick(uint32_t id, int32_t joyId, float angle, float distance, int32_t state);
			/**
			 * @brief Load or get a previous loaded sprite, it will be done on the current Sprite list
			 * @param[in] fileName Sprite name
			 * @param[in] maxSize maximum size of the sprite
			 * @return the id of the sprite requested or -1 if it does not existed
			 */
			int32_t LoadSprite(etk::UString fileName, float maxSize);
			/**
			 * @brief UnLoad or not(if needed) the sprite selected, it will be done on the current Sprite list
			 * @param[in,out] listOfElement Reference on the list of sprite that we need to find if it exist or added a new one
			 * @param[in] spriteId Sprite registered id
			 * @return ---
			 */
			void UnLoadSprite(int32_t spriteId);
	};
};

#endif


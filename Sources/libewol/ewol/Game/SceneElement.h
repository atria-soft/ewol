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
	
	typedef struct {
		uint32_t                id;         //!< unique id of the element
		int32_t                 group;      //!< element group
		int32_t                 type;       //!< element type
		coord2D_ts              position;   //!< current position
		coord2D_ts              speed;      //!< current speed
		coord2D_ts              size;       //!< curent size of the element
		etkFloat_t              angle;      //!< element angle
	} gameElementGenericProperty_ts;
	
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
			etk::VectorType<ewol::OObject*>      backgroundElements[NB_BOUBLE_BUFFER];             //!< element that must be display the first
			etk::VectorType<ewol::Sprite*>       animated[NB_BOUBLE_BUFFER];                       //!< element that must be display the second
			etk::VectorType<ewol::Sprite*>       effects[NB_BOUBLE_BUFFER];                        //!< element that must be display the third
			etk::VectorType<ewol::GameElement*>  listAnimatedElements[MAX_GROUP_NUMBER];           //!< generic element to display order in the diffferent group
			etk::VectorType<ewol::GameElement*>  listGarbage;                                      //!< garbage of the old element allocated ==> prevent multiple alloc and free
			etk::VectorType<listRegisteElement*> listCreatorElement;                               //!< list of all creatable elements
			int16_t                GetUniqueId(void) { int16_t iddd = m_id; m_id++; return iddd; };
			void                   RegisterElementType(etk::UString name, creatorElement_tf * loadElement, etk::UString userString);
			void                   RmElement(int32_t group, int32_t idElement);
			uint32_t               AddElement(int32_t group, ewol::GameElement* newElement);
			uint32_t               AddElementNamed(int32_t group, etk::UString &elementName);
			ewol::GameElement*     GetElement(uint32_t idElement);
			uint32_t               GetNearestEnemy(coord2D_ts position, int32_t groupId);
			bool                   HaveImpact(int32_t group, int32_t type, coord2D_ts position, etkFloat_t size);
			void                   Explosion(int32_t group, int32_t type, coord2D_ts position, etkFloat_t pxAtenuation, etkFloat_t power);
			uint32_t               GetElementAtPos(coord2D_ts position, int32_t maxDistanceDetection);
			void                   SetEventInput(uint32_t id, coord2D_ts position);
			void                   SetEventExternButton(uint32_t id, int32_t btId, int32_t state);
			void                   SetEventExternJoystick(uint32_t id, int32_t joyId, etkFloat_t angle, etkFloat_t distance, int32_t state);
	};
};

#endif


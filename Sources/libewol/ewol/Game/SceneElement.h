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

namespace ewol {
	class SceneElement;
};
#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/OObject/Sprite.h>
#include <ewol/Game/GameElement.h>


namespace ewol {
	class SceneElement {
		private:
			int32_t                             m_id;                                   //!< Unique element ID
		public:
			SceneElement(void) { m_id = 1; };
			~SceneElement(void) { };
			etk::VectorType<ewol::OObject*>     backgroundElements[NB_BOUBLE_BUFFER];   //!< element that must be display the first
			etk::VectorType<ewol::Sprite*>      animated[NB_BOUBLE_BUFFER];             //!< element that must be display the first
			etk::VectorType<ewol::Sprite*>      effects[NB_BOUBLE_BUFFER];              //!< element that must be display the first
			etk::VectorType<ewol::GameElement*> listAnimatedElements;                   //!< generic element to display...
			int32_t GetUniqueId(void) { int32_t iddd = m_id; m_id++; return iddd; };
			int32_t AddElement(ewol::GameElement* newElement);
	};
};

#endif


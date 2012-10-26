/**
 *******************************************************************************
 * @file ewol/oObject/Sprite.h
 * @brief ewol OpenGl Object system (header)
 * @author Edouard DUPIN
 * @date 04/04/2012
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

#ifndef __EWOL_O_OBJECT_SPRITE_H__
#define __EWOL_O_OBJECT_SPRITE_H__

#include <ewol/oObject/3DTextured.h>
#include <ewol/ResourceManager.h>

namespace ewol {
	class Sprite :public ewol::OObject3DTextured
	{
		private:
			etk::UString m_name;
		public:
			Sprite(etk::UString spriteName, float sizeX=-1, float sizeY=-1);
			virtual ~Sprite(void);
			void Element(etk::Vector2D<float> pos, float size, float angle);
			void Element(etk::Vector3D<float> pos, float size, float angle);
			void Element(etk::Vector2D<float> pos, float size, float angle, draw::Color tmpColor);
			void Element(etk::Vector3D<float> pos, float size, float angle, draw::Color tmpColor);
			bool HasName(etk::UString& name) { return name == m_name; };
	};
};

#endif


/**
 *******************************************************************************
 * @file ewol/OObject/2DTextured.h
 * @brief ewol OpenGl Object system (header)
 * @author Edouard DUPIN
 * @date 09/11/2011
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

#ifndef __EWOL_O_OBJECT_2D_TEXTURED_H__
#define __EWOL_O_OBJECT_2D_TEXTURED_H__

#include <ewol/OObject.h>

namespace ewol {
	class OObject2DTextured :public ewol::OObject
	{
		public:
			OObject2DTextured(etk::UString textureName);
			OObject2DTextured(etk::UString textureName, etkFloat_t sizeX, etkFloat_t sizeY);
			virtual ~OObject2DTextured(void);
		public:
			virtual void Draw(void);
			void Clear(void);
			void Rectangle(etkFloat_t x, etkFloat_t y, etkFloat_t w, etkFloat_t h, etkFloat_t texX=0.0, etkFloat_t texY=0.0, etkFloat_t texSX=1.0, etkFloat_t texSY=1.0);
		protected:
			int32_t                       m_textureId;   //!< texture internal ID
			etk::VectorType<coord2D_ts>   m_coord;       //!< internal coord of the object
			etk::VectorType<texCoord_ts>  m_coordTex;    //!< internal texture coordinate for every point
	};
};

#endif


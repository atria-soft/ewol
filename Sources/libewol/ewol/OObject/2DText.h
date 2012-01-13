/**
 *******************************************************************************
 * @file ewol/OObject/2DText.h
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

#ifndef __EWOL_O_OBJECT_2D_TEXT_H__
#define __EWOL_O_OBJECT_2D_TEXT_H__

#include <ewol/OObject.h>

namespace ewol {
	class OObject2DText :public ewol::OObject
	{
		public:
			OObject2DText(etk::String FontName, int32_t size, color_ts textColorFg);
			OObject2DText(void);
			virtual ~OObject2DText(void);
		public:
			virtual void Draw(void);
			// set a specific text
			void Text(etkFloat_t x, etkFloat_t y, const char* utf8String, int32_t clippingPositionX);
			void TextAdd(etkFloat_t x, etkFloat_t y, const char* utf8String, int32_t clippingPositionX);
		protected:
			int32_t                       m_FontId;        //!< font internal ID
			color_ts                      m_textColorFg;   //!< text color ...
			uint32_t                      m_FontTextureId; //!< font internal Texture ID
			etk::VectorType<coord2D_ts>   m_coord;         //!< internal coord of the object
			etk::VectorType<texCoord_ts>  m_coordTex;      //!< internal texture coordinate for every point
		public:
			virtual void UpdateOrigin(etkFloat_t x, etkFloat_t y);
	};
};

#endif


/**
 *******************************************************************************
 * @file ewol/OObject/2DTextColored.h
 * @brief ewol OpenGl Object system (header)
 * @author Edouard DUPIN
 * @date 16/01/2012
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

#ifndef __EWOL_O_OBJECT_2D_TEXT_COLORED_H__
#define __EWOL_O_OBJECT_2D_TEXT_COLORED_H__

#include <ewol/OObject.h>

namespace ewol {
	class OObject2DTextColored :public ewol::OObject
	{
		public:
			OObject2DTextColored(etk::String FontName, int32_t size);
			OObject2DTextColored(void);
			virtual ~OObject2DTextColored(void);
		public:
			virtual void Draw(void);
			void SetColor(etkFloat_t red, etkFloat_t green, etkFloat_t blue, etkFloat_t alpha = 1.0);
			void SetColor(color_ts color);
			// set a specific text
			int32_t Text(etkFloat_t x, etkFloat_t y, const char* utf8String, int32_t clippingPositionX);
			int32_t TextAdd(etkFloat_t x, etkFloat_t y, const char* utf8String, int32_t clippingPositionX);
			int32_t TextAdd(coord2D_ts textPos, clipping_ts drawClipping, const uniChar_t* unicodeString);
		protected:
			int32_t                       m_FontId;        //!< font internal ID
			color_ts                      m_color;         //!< tmp text color ...
			uint32_t                      m_FontTextureId; //!< font internal Texture ID
			etk::VectorType<coord2D_ts>   m_coord;         //!< internal coord of the object
			etk::VectorType<texCoord_ts>  m_coordTex;      //!< internal texture coordinate for every point
			etk::VectorType<color_ts>     m_coordColor;    //!< internal color of the different point
		public:
			virtual void UpdateOrigin(etkFloat_t x, etkFloat_t y);
	};
};

#endif


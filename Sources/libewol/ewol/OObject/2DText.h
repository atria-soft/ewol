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
#include <etk/UString.h>

namespace ewol {
	class OObject2DText :public ewol::OObject
	{
		public:
			OObject2DText(etk::UString FontName, int32_t size, color_ts textColorFg);
			OObject2DText(void);
			virtual ~OObject2DText(void);
		public:
			virtual void Draw(void);
			// set a specific text
			void    Clear(void);
			int32_t Text(Vector2D<float>  textPos, clipping_ts drawClipping, const etk::UString& unicodeString);
			int32_t Text(Vector2D<float>  textPos, clipping_ts drawClipping, const uniChar_t     unicodeChar);
		protected:
			int32_t                       m_FontId;        //!< font internal ID
			color_ts                      m_textColorFg;   //!< text color ...
			int32_t                       m_FontTextureId; //!< font internal Texture ID
			etk::VectorType<Vector2D<float> >   m_coord;         //!< internal coord of the object
			etk::VectorType<texCoord_ts>  m_coordTex;      //!< internal texture coordinate for every point
	};
};

#endif


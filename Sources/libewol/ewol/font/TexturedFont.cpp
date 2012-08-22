/**
 *******************************************************************************
 * @file ewol/font/TexturedFont.cpp
 * @brief ewol Textured Font system (Sources)
 * @author Edouard DUPIN
 * @date 22/08/2012
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

#include <etk/Types.h>
#include <ewol/font/Font.h>
#include <ewol/font/TexturedFont.h>
#include <ewol/font/FontManager.h>


ewol::TexturedFont::TexturedFont(etk::UString fontName, int32_t size) : 
	m_font(NULL),
	m_counter(1)
{
	m_size = size;
	m_font = ewol::font::Keep(fontName);
	if (NULL == m_font) {
		return;
	}
	// initilize the texture ...
	// TODO : Do a better methode that not initialize with a stupid language ... like now ...
	m_font->GenerateBitmapFont(m_size, m_height, m_texture, m_listElement);
}

ewol::TexturedFont::~TexturedFont(void)
{
	ewol::font::Release(m_font);
}

int32_t ewol::TexturedFont::Draw(Vector2D<float>                 textPos,
                                 const etk::UString&             unicodeString,
                                 etk::Vector<Vector2D<float> > & coord,
                                 etk::Vector<texCoord_ts> &      coordTex)
{
	float totalSize = 0;
	Vector2D<float> tmpPos = textPos;
	for(int32_t iii=0; iii<unicodeString.Size(); iii++) {
		int32_t ret = Draw(tmpPos, unicodeString[iii], coord, coordTex);
		tmpPos.x += ret;
		totalSize += ret;
	}
	return totalSize;
	
	#if 0
	// To display the texture ...
		{
			/* Bitmap position
			 *   0------1
			 *   |      |
			 *   |      |
			 *   3------2
			 */
			Vector2D<float> bitmapDrawPos[4];
			bitmapDrawPos[0].x = 10;
			bitmapDrawPos[1].x = 400;
			bitmapDrawPos[2].x = 400;
			bitmapDrawPos[3].x = 10;
			
			bitmapDrawPos[0].y = 10;
			bitmapDrawPos[1].y = 10;
			bitmapDrawPos[2].y = 400;
			bitmapDrawPos[3].y = 400;
			/* texture Position : 
			 *   0------1
			 *   |      |
			 *   |      |
			 *   3------2
			 */
			texCoord_ts texturePos[4];
			texturePos[0].u = 0;
			texturePos[1].u = 1;
			texturePos[2].u = 1;
			texturePos[3].u = 0;
			
			texturePos[0].v = 0;
			texturePos[1].v = 0;
			texturePos[2].v = 1;
			texturePos[3].v = 1;
			
			// NOTE : Android does not support the Quads elements ...
			/* Step 1 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			// set texture coordonates :
			coordTex.PushBack(texturePos[0]);
			coordTex.PushBack(texturePos[1]);
			coordTex.PushBack(texturePos[2]);
			// set display positions :
			coord.PushBack(bitmapDrawPos[0]);
			coord.PushBack(bitmapDrawPos[1]);
			coord.PushBack(bitmapDrawPos[2]);
			
			/* Step 2 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			// set texture coordonates :
			coordTex.PushBack(texturePos[0]);
			coordTex.PushBack(texturePos[2]);
			coordTex.PushBack(texturePos[3]);
			// set display positions :
			coord.PushBack(bitmapDrawPos[0]);
			coord.PushBack(bitmapDrawPos[2]);
			coord.PushBack(bitmapDrawPos[3]);
		}
	#endif
}

int32_t ewol::TexturedFont::Draw(Vector2D<float>                 textPos,
                                 const uniChar_t                 unicodeChar,
                                 etk::Vector<Vector2D<float> > & coord,
                                 etk::Vector<texCoord_ts> &      coordTex)
{
	float posDrawX = textPos.x;
	
	int32_t charIndex;
	if (unicodeChar < 0x20) {
		charIndex = 0;
	} else if (unicodeChar < 0x80) {
		charIndex = unicodeChar - 0x1F;
	} else {
		charIndex = 0;
		for (int32_t iii=0x80-0x20; iii < m_listElement.Size(); iii++) {
			if (m_listElement[iii].unicodeCharVal == unicodeChar) {
				charIndex = iii;
				break;
			}
		}
	}
	// 0x01 == 0x20 == ' ';
	if (unicodeChar != 0x01) {
		/* Bitmap position
		 *      xA     xB
		 *   yC *------*
		 *      |      |
		 *      |      |
		 *   yD *------*
		 */
		float dxA = posDrawX + m_listElement[charIndex].bearing.x;
		float dxB = posDrawX + m_listElement[charIndex].bearing.x + m_listElement[charIndex].size.x;
		float dyC = textPos.y + m_listElement[charIndex].bearing.y + m_height - m_size;
		float dyD = dyC - m_listElement[charIndex].size.y;
		
		float tuA = m_listElement[charIndex].posStart.u;
		float tuB = m_listElement[charIndex].posStop.u;
		float tvC = m_listElement[charIndex].posStart.v;
		float tvD = m_listElement[charIndex].posStop.v;
		
		
		// Clipping and drawing area
		if(    dxB <= dxA
		    || dyD >= dyC) {
			// nothing to do ...
		} else {
			/* Bitmap position
			 *   0------1
			 *   |      |
			 *   |      |
			 *   3------2
			 */
			Vector2D<int32_t> bitmapDrawPos[4];
			bitmapDrawPos[0].x = (int32_t)dxA;
			bitmapDrawPos[1].x = (int32_t)dxB;
			bitmapDrawPos[2].x = (int32_t)dxB;
			bitmapDrawPos[3].x = (int32_t)dxA;
			
			bitmapDrawPos[0].y = (int32_t)dyC;
			bitmapDrawPos[1].y = (int32_t)dyC;
			bitmapDrawPos[2].y = (int32_t)dyD;
			bitmapDrawPos[3].y = (int32_t)dyD;
			/* texture Position : 
			 *   0------1
			 *   |      |
			 *   |      |
			 *   3------2
			 */
			texCoord_ts texturePos[4];
			texturePos[0].u = tuA;
			texturePos[1].u = tuB;
			texturePos[2].u = tuB;
			texturePos[3].u = tuA;
			
			texturePos[0].v = tvC;
			texturePos[1].v = tvC;
			texturePos[2].v = tvD;
			texturePos[3].v = tvD;
			
			// NOTE : Android does not support the Quads elements ...
			/* Step 1 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			// set texture coordonates :
			coordTex.PushBack(texturePos[0]);
			coordTex.PushBack(texturePos[1]);
			coordTex.PushBack(texturePos[2]);
			// set display positions :
			coord.PushBack(bitmapDrawPos[0]);
			coord.PushBack(bitmapDrawPos[1]);
			coord.PushBack(bitmapDrawPos[2]);
			
			/* Step 2 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			// set texture coordonates :
			coordTex.PushBack(texturePos[0]);
			coordTex.PushBack(texturePos[2]);
			coordTex.PushBack(texturePos[3]);
			// set display positions :
			coord.PushBack(bitmapDrawPos[0]);
			coord.PushBack(bitmapDrawPos[2]);
			coord.PushBack(bitmapDrawPos[3]);
			
		}
	}
	posDrawX += m_listElement[charIndex].advance;
	int32_t sizeOut = posDrawX - textPos.x;
	textPos.x = posDrawX;
	return sizeOut;
}

Vector2D<float> ewol::TexturedFont::GetSize(const etk::UString & unicodeString)
{
	Vector2D<float> outputSize(0,m_height);
	for(int32_t iii=0; iii<unicodeString.Size(); iii++) {
		Vector2D<float> tmpp = GetSize(unicodeString[iii]);
		outputSize.x += tmpp.x;
	}
	return outputSize;
}


Vector2D<float> ewol::TexturedFont::GetSize(const uniChar_t unicodeChar)
{
	Vector2D<float> outputSize(0,m_height);
	int32_t charIndex;
	if (unicodeChar >= 0x80) {
		charIndex = 0;
	} else if (unicodeChar < 0x20) {
		charIndex = 0;
	} else if (unicodeChar < 0x80) {
		charIndex = unicodeChar - 0x1F;
	} else {
		for (int32_t iii=0x80-0x20; iii < m_listElement.Size(); iii++) {
			if (m_listElement[iii].unicodeCharVal == unicodeChar) {
				charIndex = iii;
				break;
			}
		}
		// TODO : Update if possible the mapping
		charIndex = 0;
	}
	outputSize.x = m_listElement[charIndex].advance;
	return outputSize;
}



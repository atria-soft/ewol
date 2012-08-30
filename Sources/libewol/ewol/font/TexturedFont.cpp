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
#include <ewol/ResourceManager.h>


static int32_t nextP2(int32_t value)
{
	int32_t val=1;
	for (int32_t iii=1; iii<31; iii++) {
		if (value <= val) {
			return val;
		}
		val *=2;
	}
	EWOL_CRITICAL("impossible CASE....");
	return val;
}

static int32_t simpleSQRT(int32_t value)
{
	int32_t val=1;
	for (int32_t iii=1; iii<1000; iii++) {
		val =iii*iii;
		if (value <= val) {
			return iii;
		}
	}
	EWOL_CRITICAL("impossible CASE....");
	return val;
}


ewol::TexturedFont::TexturedFont(etk::UString fontName) : 
	ewol::Texture(fontName),
	m_font(NULL),
	m_lastGlyphPos(0,0),
	m_lastRawHeigh(0)
{
	int32_t tmpSize = 0;
	// extarct name and size :
	char * tmpData = fontName.c_str();
	char * tmpPos = strchr(tmpData, ':');
	
	if (tmpPos==NULL) {
		m_size = 1;
		EWOL_CRITICAL("Can not parse the font name : \"" << fontName << "\" ??? ':' " );
		return;
	} else {
		if (sscanf(tmpPos+1, "%d", &tmpSize)!=1) {
			m_size = 1;
			EWOL_CRITICAL("Can not parse the font name : \"" << fontName << "\" ==> size ???");
			return;
		}
	}
	m_name = fontName.Extract(0, (tmpPos - tmpData));
	m_size = tmpSize;
	
	//EWOL_CRITICAL("Load FONT  name : \"" << m_name << "\" ==> size=" << m_size);
	ewol::resource::Keep(m_name, m_font);
	if (NULL == m_font) {
		return;
	}
	
	// set the bassic charset:
	m_listElement.Clear();
	freeTypeFontElement_ts tmpchar1;
	tmpchar1.property.m_UVal = 0;
	m_listElement.PushBack(tmpchar1);
	for (int32_t iii=0x20; iii<0xFF; iii++) {
		freeTypeFontElement_ts tmpchar;
		tmpchar.property.m_UVal = iii;
		m_listElement.PushBack(tmpchar);
		if (0x7F == iii) {
			iii = 0x9F;
		}
	}
	
	/* this is a bad code for now ... */
	// ==> determine the texture Size
	GlyphProperty tmpproperty;
	tmpproperty.m_UVal = 'A';
	m_font->GetGlyphProperty(m_size, tmpproperty);
	
	int32_t nbElement = 0xFF - 0x20 + 1;
	int32_t coter = simpleSQRT(nbElement);
	// note : +1 is for the overlapping of the glyph (Part 1)
	int32_t glyphMaxWidth = tmpproperty.m_advance.x +1;
	int32_t glyphMaxHeight = tmpproperty.m_advance.y +1;
	int32_t textureWidth = nextP2(coter*glyphMaxWidth);
	int32_t nbRaws = textureWidth / glyphMaxWidth;
	if (nbRaws <= 0) {
		EWOL_ERROR("devide by 0");
		nbRaws = 1;
	}
	int32_t nbLine = (nbElement / nbRaws) + 1;
	int32_t textureHeight = nextP2(nbLine*glyphMaxHeight);
	// for android : 
	textureHeight = etk_max(textureHeight, textureWidth);
	textureWidth = textureHeight;
	
	
	EWOL_DEBUG("Generate a text texture for char(" << nbRaws << "," << nbLine << ") with size=(" << textureWidth << "," << textureHeight << ")");
	// resize must be done on the texture ...
	SetImageSize(Vector2D<int32_t>(textureWidth,textureHeight));
	// now we can acces directly on the image
	m_data.SetFillColor(draw::Color(0xFFFFFF00));
	m_data.Clear();
	
	m_height = m_font->GetHeight(m_size);
	
	int32_t CurrentLineHigh = 0;
	Vector2D<int32_t>    glyphPosition(1,1);
	for (int32_t iii=0; iii<m_listElement.Size(); iii++) {
		if (true == m_font->GetGlyphProperty(m_size, m_listElement[iii].property)) {
			/*
			// check internal property:
			// enought in the texture :
			//if (m_data.GetWidth() < m_lastGlyphPos.x + m_listElement[iii].property.m_sizeTexture.x
				// resize if needed ...
			
			// line size :
			
			// draw
			
			// move the curent pointer of drawing:
			*/
			// change line if needed ...
			if (glyphPosition.x+m_listElement[iii].property.m_sizeTexture.x > textureWidth) {
				glyphPosition.x = 0;
				glyphPosition.y += CurrentLineHigh;
				CurrentLineHigh = 0;
			}
			// draw the glyph
			m_font->DrawGlyph(m_data, m_size, glyphPosition, m_listElement[iii].property);
			// set video position
			m_listElement[iii].posStart.u = (float)(glyphPosition.x) / (float)textureWidth;
			m_listElement[iii].posStart.v = (float)(glyphPosition.y) / (float)textureHeight;
			m_listElement[iii].posStop.u  = (float)(glyphPosition.x + m_listElement[iii].property.m_sizeTexture.x) / (float)textureWidth;
			m_listElement[iii].posStop.v  = (float)(glyphPosition.y + m_listElement[iii].property.m_sizeTexture.y) / (float)textureHeight;
			/*
			EWOL_DEBUG("generate '" << (char)m_listElement[iii].property.m_UVal << "'");
			EWOL_DEBUG("     in tex : " << glyphPosition << " ==> " << m_listElement[iii].posStart.u<< "," << m_listElement[iii].posStart.v );
			EWOL_DEBUG("     m_sizeTexture =" << m_listElement[iii].property.m_sizeTexture );
			EWOL_DEBUG("     m_bearing     =" << m_listElement[iii].property.m_bearing );
			EWOL_DEBUG("     m_advance     =" << m_listElement[iii].property.m_advance );
			*/
			
			// update the maximum of the line hight : 
			if (CurrentLineHigh<m_listElement[iii].property.m_sizeTexture.y) {
				// note : +1 is for the overlapping of the glyph (Part 2)
				CurrentLineHigh = m_listElement[iii].property.m_sizeTexture.y+1;
			}
			// note : +1 is for the overlapping of the glyph (Part 3)
			// update the Bitmap position drawing : 
			glyphPosition.x += m_listElement[iii].property.m_sizeTexture.x+1;
		}
		
	}
	// For testing cheree the box are set)
	#if 0
	draw::Color tlpppp(0xFF,0xFF,0xFF,0x00);
	for(int32_t jjj=0; jjj < textureHeight;jjj++) {
		for(int32_t iii=0; iii < textureWidth; iii++){
			tlpppp = m_data.Get(Vector2D<int32_t>(iii, jjj) );
			// set only alpha :
			tlpppp.a = etk_min( tlpppp.a+0x60, 0xFF);
			// real set of color
			m_data.Set(Vector2D<int32_t>(iii, jjj), tlpppp );
		}
	}
	#endif
	EWOL_DEBUG("End generation of the Fond bitmap, start adding texture");
	Flush();
	
	// initilize the texture ...
	// TODO : Do a better methode that not initialize with a stupid language ... like now ...
	//m_font->GenerateBitmapFont(m_size, m_height, m_texture, m_listElement);
}

ewol::TexturedFont::~TexturedFont(void)
{
	if (NULL!= m_font) {
		ewol::resource::Release(m_font);
	}
}


bool ewol::TexturedFont::HasName(etk::UString& fileName)
{
	etk::UString tmpName = m_name;
	tmpName += ":";
	tmpName += m_size;
	EWOL_VERBOSE("S : check : " << fileName << " ?= " << tmpName << " = " << (fileName==tmpName) );
	return (fileName==tmpName);
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
			
			bitmapDrawPos[0].y = 400;
			bitmapDrawPos[1].y = 400;
			bitmapDrawPos[2].y = 10;
			bitmapDrawPos[3].y = 10;
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
	return totalSize;
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
			if (m_listElement[iii].property.m_UVal == unicodeChar) {
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
		float dxA = posDrawX + m_listElement[charIndex].property.m_bearing.x;
		float dxB = posDrawX + m_listElement[charIndex].property.m_bearing.x + m_listElement[charIndex].property.m_sizeTexture.x;
		float dyC = textPos.y + m_listElement[charIndex].property.m_bearing.y + m_height - m_size;
		float dyD = dyC - m_listElement[charIndex].property.m_sizeTexture.y;
		
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
	posDrawX += m_listElement[charIndex].property.m_advance.x;
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
			if (m_listElement[iii].property.m_UVal == unicodeChar) {
				charIndex = iii;
				break;
			}
		}
		// TODO : Update if possible the mapping
		charIndex = 0;
	}
	outputSize.x = m_listElement[charIndex].property.m_advance.x;
	return outputSize;
}



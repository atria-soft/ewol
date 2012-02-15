/**
 *******************************************************************************
 * @file ewol/FontBitmap.cpp
 * @brief ewol Font system (sources)
 * @author Edouard DUPIN
 * @date 29/10/2011
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

#ifndef EWOL_USE_FREE_TYPE

#include <ewol/Font.h>
#include <ewol/Texture.h>
#include <etk/VectorType.h>
#include <ewol/importgl.h>


#undef __class__
#define __class__	"ewol::FontBitmap"

extern "C"
{
	typedef struct {
		uniChar_t   unicodeCharVal;
		int32_t     width;
		texCoord_ts posStart;
		texCoord_ts posStop;
		etkFloat_t ratio;
	}UTF8Element_ts;
}


namespace ewol
{
	class EbtFont
	{
		public:
			EbtFont(etk::File newFile, etk::UString fontName, int32_t size)
			{
				m_loadedOK = false;
				m_filename = newFile;
				m_fontName = fontName;
				m_size = size;
				m_elements.Clear();
				for (int32_t iii=0x0; iii<0x81; iii++) {
					UTF8Element_ts tmpchar;
					tmpchar.unicodeCharVal = iii;
					tmpchar.width = 0;
					tmpchar.posStart.u = 0;
					tmpchar.posStart.v = 0;
					tmpchar.posStop.u = 0;
					tmpchar.posStop.v = 0;
					tmpchar.ratio = 0;
					m_elements.PushBack(tmpchar);
				}
				if(false == m_filename.fOpenRead()) {
					EWOL_ERROR("Can not Open the file named=\"" << m_filename << "\"");
					return;
				}
				// load all element of the file ...
				char elementLine[2048];
				int32_t lineID=1;
				while (NULL != m_filename.fGets(elementLine, 2048) )
				{
					//EWOL_DEBUG(" Read file Line : " << elementLine);
					if (    '\n' != elementLine[0] // EOL
					     && '\0' != elementLine[0] // EOF
					     && '#'  != elementLine[0] // Comment line
					   )
					{
						if (0 == strncmp("name:", elementLine, 5)) {
							char extractString[256] = "";
							sscanf(elementLine, "name:%s", extractString);
							m_fontName = extractString;
							EWOL_INFO("Find font named : \"" << m_fontName << "\"");
						} else if (0 == strncmp("source:", elementLine, 7)) {
							char extractString[256] = "";
							sscanf(elementLine, "source:%s", extractString);
							m_bitmapName = extractString;
						} else if (0 == strncmp("0x00", elementLine, 4)) {
							int32_t GlyphPositionX;
							int32_t GlyphPositionY;
							int32_t GlyphSizeX;
							int32_t GlyphSizeY;
							sscanf(elementLine, "0x00 (%d,%d) (%d,%d)", &GlyphPositionX, &GlyphPositionY, &GlyphSizeX, &GlyphSizeY);
							ClearAll(GlyphPositionX, GlyphPositionY, GlyphSizeX, GlyphSizeY);
						} else if (0 == strncmp("0x", elementLine, 2)) {
							uint32_t utf8Value;
							int32_t GlyphPositionX;
							int32_t GlyphPositionY;
							int32_t GlyphSizeX;
							int32_t GlyphSizeY;
							sscanf(elementLine, "0x%x (%d,%d) (%d,%d)", &utf8Value, &GlyphPositionX, &GlyphPositionY, &GlyphSizeX, &GlyphSizeY);
							SetGlyphID(utf8Value, lineID, GlyphPositionX, GlyphPositionY, GlyphSizeX, GlyphSizeY);
						} else {
							EWOL_ERROR("error when parsing the line : " << lineID << "\"" << elementLine << "\"");
						}
					}
					lineID++;
				}
				// close the file at end of reading...
				m_filename.fClose();
				// Load Bitmap : 
				etk::UString bitmapRealFile = m_filename.GetFolder() + "/" + m_bitmapName;
				EWOL_INFO("load text font image : \"" << bitmapRealFile << "\"");
				etk::File tmpFile(bitmapRealFile, m_filename.GetTypeAccess());
				m_textureId = ewol::LoadTexture(tmpFile);
				m_textureLoaded = true;
				m_loadedOK = true;
			};
			
			~EbtFont(void)
			{
				if (true == m_textureLoaded) {
					ewol::UnLoadTexture(m_textureId);
				}
			};
			bool loadedOK(void)
			{
				return m_loadedOK;
			};
		private:
			void ClearAll(int32_t x, int32_t y, int32_t w, int32_t h)
			{
				EWOL_DEBUG("Find default font glyph : (" << x << "," << y << ") (" << w << "," << h << ") ");
				for (int32_t iii=0; iii< 0x80; iii++) {
					m_elements[iii].posStart.u = (etkFloat_t)x / 512.0;
					m_elements[iii].posStart.v = (etkFloat_t)y / 512.0;
					m_elements[iii].posStop.u = (etkFloat_t)(x+w) / 512.0;
					m_elements[iii].posStop.v = (etkFloat_t)(y+h) / 512.0;
					m_elements[iii].ratio = (etkFloat_t)w/(etkFloat_t)h;
					m_elements[iii].width = m_size * 1.48 * m_elements[iii].ratio;
				}
			};
			void SetGlyphID(int32_t utf8Value, int32_t lineID, int32_t x, int32_t y, int32_t w, int32_t h)
			{
				EWOL_DEBUG("Add font glyph : "<< utf8Value << " (" << x << "," << y << ") (" << w << "," << h << ") ");
				if (utf8Value < 0x80) {
					m_elements[utf8Value].posStart.u = (etkFloat_t)x / 512.0;
					m_elements[utf8Value].posStart.v = (etkFloat_t)y / 512.0;
					m_elements[utf8Value].posStop.u = (etkFloat_t)(x+w) / 512.0;
					m_elements[utf8Value].posStop.v = (etkFloat_t)(y+h) / 512.0;
					m_elements[utf8Value].ratio = (etkFloat_t)w/(etkFloat_t)h;
					m_elements[utf8Value].width = m_size * 1.48 * m_elements[utf8Value].ratio;
				} else {
					EWOL_ERROR("not manage glyph with ID > 0x7F line : " << lineID);
				}
			};
			
		public:
			etk::File GetFileName(void)
			{
				return m_filename;
			};
			etk::UString GetName(void)
			{
				return m_fontName;
			};
			bool IsLoaded(void)
			{
				return m_textureLoaded;
			};
			bool Check(etk::UString fontName, int32_t size)
			{
				if (m_loadedOK == -1) {
					return false;
				}
				if(    m_fontName == fontName
				    && m_size == size)
				{
					return true;
				}
				return false;
			};
			
		private:
			etk::File      m_filename;
			bool           m_loadedOK;
			etk::UString    m_fontName;
			int32_t        m_size;
			uint32_t       m_textureId;
			bool           m_textureLoaded;
			etk::UString    m_bitmapName;
			etk::VectorType<UTF8Element_ts>   m_elements;    // 
		public:
			etk::VectorType<UTF8Element_ts> & GetRefOnElement(void)
			{
				return m_elements;
			};
			uint32_t GetOglId(void)
			{
				return GetTextureGLID(m_textureId);
			};
			int32_t GetHeight(void)
			{
				return m_size*1.42;
			};
			int32_t GetSize(void)
			{
				return m_size;
			};
	};
};

static etk::VectorType<ewol::EbtFont*> s_listLoadedFonts;

static etk::UString s_currentFolderName = "";
static etk::UString s_currentDefaultFontName = "";
static int32_t s_currentDefaultFontId = -1;

void ewol::SetFontFolder(etk::UString folderName)
{
	if (s_currentFolderName != "") {
		EWOL_WARNING("Change the FontFolder, old=\"" << s_currentFolderName << "\"");
	}
	EWOL_TODO("Check if folder exist");
	s_currentFolderName = folderName;
	EWOL_INFO("New default font folder name=\"" << s_currentFolderName << "\"");
}

void ewol::InitFont(void)
{
	// nothing to do ...
}

void ewol::UnInitFont(void)
{
	EWOL_TODO("later");
}

void ewol::SetDefaultFont(etk::UString fontName, int32_t size)
{
	if (s_currentDefaultFontName != "") {
		EWOL_WARNING("Change the default Ewol Font, old=\"" << s_currentDefaultFontName << "\"");
	}
	EWOL_INFO("New default Font Name=\"" << fontName << "\"");
	int32_t tmpId = ewol::LoadFont(fontName, size);
	if (-1 == tmpId) {
		if (s_currentDefaultFontName == "") {
			EWOL_ASSERT(-1 != tmpId, "Error to load the default Font\"" << fontName << "\"");
		} else {
			EWOL_CRITICAL("Unable to load the new default font:\"" << fontName << "\"");
		}
		return;
	}
	// save the default font parameters ...
	s_currentDefaultFontName = fontName;
	s_currentDefaultFontId = tmpId;
}

int32_t ewol::GetDefaultFontId(void)
{
	return s_currentDefaultFontId;
}

int32_t ewol::LoadFont(etk::UString fontName, int32_t size)
{
	// check if folder file
	etk::UString tmpFileName = s_currentFolderName + "/" + fontName + ".ebt";
	etk::File fileName(tmpFileName, etk::FILE_TYPE_DATA);
	if (false == fileName.Exist()) {
		EWOL_ERROR("Font does not exist: \"" << fileName.GetCompleateName() << "\"");
		return -1;
	}
	for (int32_t iii=0; iii < s_listLoadedFonts.Size(); iii++) {
		if (true == s_listLoadedFonts[iii]->Check(fontName, size)) {
			return iii;
		}
	}
	EbtFont * tmpFont = new EbtFont(fileName, fontName, size);
	s_listLoadedFonts.PushBack(tmpFont);
	return s_listLoadedFonts.Size()-1;
}



void ewol::UnloadFont(int32_t id)
{
	EWOL_TODO("I do not think it was a good idea... will be done later");
}



/*
void ewol::DrawText(int32_t                        fontID,
                    coord2D_ts &                   drawPosition,
                    const uniChar_t *              unicodeString,
                    uint32_t &                     fontTextureId,
                    etk::VectorType<coord2D_ts> &  coord,
                    etk::VectorType<texCoord_ts> & coordTex)
{
	EWOL_TODO("previous code does not work at all");

	if(fontID>=s_listLoadedFonts.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return;
	}
	etk::VectorType<UTF8Element_ts> & listOfElement = s_listLoadedFonts[fontID]->GetRefOnElement();
	
	fontTextureId = s_listLoadedFonts[fontID]->GetOglId();
	EWOL_DEBUG("****************    plop " << fontTextureId);
	int32_t size = s_listLoadedFonts[fontID]->GetHeight();
	
	etkFloat_t posDrawX = drawPosition.x;
	while(*unicodeString != 0) {
		int32_t tmpChar = *unicodeString++;
		if (tmpChar >= 0x80) {
			tmpChar = 0;
		}
		etkFloat_t sizeWidth = listOfElement[tmpChar].width;
		if (tmpChar != 0x20) {
			// set texture coordonates :
			coordTex.PushBack(listOfElement[tmpChar].posStart);
			texCoord_ts tmpTex;
			tmpTex.u = listOfElement[tmpChar].posStop.u;
			tmpTex.v = listOfElement[tmpChar].posStart.v;
			coordTex.PushBack(tmpTex);
			coordTex.PushBack(listOfElement[tmpChar].posStop);
			tmpTex.u = listOfElement[tmpChar].posStart.u;
			tmpTex.v = listOfElement[tmpChar].posStop.v;
			coordTex.PushBack(tmpTex);
			// set display positions :
			coord2D_ts tmpCoord;
			tmpCoord.x = posDrawX;
			tmpCoord.y = drawPosition.y;
			coord.PushBack(tmpCoord);
			tmpCoord.x = posDrawX + sizeWidth;
			coord.PushBack(tmpCoord);
			tmpCoord.y = drawPosition.y + size;
			coord.PushBack(tmpCoord);
			tmpCoord.x = posDrawX;
			coord.PushBack(tmpCoord);
		}
		posDrawX += sizeWidth;
	}
	drawPosition.x = posDrawX;
}
*/

void ewol::DrawText(int32_t                        fontID,        // Id of the desire font
                    coord2D_ts &                   drawPosition,  // position to start the writing (can be < 0) ==> no check
                    coord2D_ts &                   clipSize,      // clipping size
                    const char *                   utf8String,    // my UTF8 string
                    uint32_t &                     fontTextureId, // return the fontID needed to display this string
                    etk::VectorType<coord2D_ts> &  coord,         // return the coordonates
                    etk::VectorType<texCoord_ts> & coordTex)      // return the texture coordonates
{
	// TODO : This code des not work, why ????
	/*
	int32_t tmpstringLen = strlen(utf8String);
	int32_t * tmpUnicodeString = new int32_t(tmpstringLen+1);
	// TODO : generate a better convertor...
	for (int32_t iii=0; iii<tmpstringLen; iii++) {
		tmpUnicodeString[iii] = utf8String[iii];
	}
	tmpUnicodeString[tmpstringLen] = 0;
	// unicode display ...
	//DrawText(fontID, drawPosition, tmpUnicodeString, fontTextureId, coord, coordTex);
	// clean temporary data ..
	delete [] tmpUnicodeString;
	*/
	if(fontID>=s_listLoadedFonts.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return;
	}
	etk::VectorType<UTF8Element_ts> & listOfElement = s_listLoadedFonts[fontID]->GetRefOnElement();
	char * tmpVal = (char*)utf8String;
	
	fontTextureId = s_listLoadedFonts[fontID]->GetOglId();
	//EWOL_DEBUG("****************    plop2 " << fontTextureId);
	int32_t size = s_listLoadedFonts[fontID]->GetHeight();
	
	etkFloat_t posDrawX = drawPosition.x;
	while(*tmpVal != 0) {
		int32_t tmpChar = *tmpVal++;
		int32_t charIndex;
		if (tmpChar >= 0x80) {
			charIndex = 0;
		} else if (tmpChar < 0x20) {
			charIndex = 0;
		} else if (tmpChar < 0x80) {
			charIndex = tmpChar; // - 0x1F;
		} else {
			for (int32_t iii=0x80/*-0x20*/; iii < listOfElement.Size(); iii++) {
				if (listOfElement[iii].unicodeCharVal == tmpChar) {
					charIndex = iii;
					break;
				}
			}
			// TODO : Update if possible the mapping
			charIndex = 0;
		}
		etkFloat_t sizeWidth = listOfElement[charIndex].width;
		// check the clipping
		if (clipSize.x>0 && posDrawX+sizeWidth > clipSize.x) {
			// TODO : Create a better clipping methode ...
			break;
		}
		// 0x01 == 0x20 == ' ';
		if (tmpChar != 0x01) {
			// NOTE : Android does not support the Quads elements ...
			/* Step 1 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			// set texture coordonates :
			coordTex.PushBack(listOfElement[charIndex].posStart);
			texCoord_ts tmpTex;
			tmpTex.u = listOfElement[charIndex].posStop.u;
			tmpTex.v = listOfElement[charIndex].posStart.v;
			coordTex.PushBack(tmpTex);
			coordTex.PushBack(listOfElement[charIndex].posStop);
			// set display positions :
			coord2D_ts tmpCoord;
			tmpCoord.x = posDrawX;
			tmpCoord.y = drawPosition.y;
			coord.PushBack(tmpCoord);
			tmpCoord.x = posDrawX + sizeWidth;
			coord.PushBack(tmpCoord);
			tmpCoord.y = drawPosition.y + size;
			coord.PushBack(tmpCoord);
			
			/* Step 2 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			
			// set texture coordonates :
			coordTex.PushBack(listOfElement[charIndex].posStart);
			coordTex.PushBack(listOfElement[charIndex].posStop);
			tmpTex.u = listOfElement[charIndex].posStart.u;
			tmpTex.v = listOfElement[charIndex].posStop.v;
			coordTex.PushBack(tmpTex);
			
			// set display positions :
			tmpCoord.x = posDrawX;
			tmpCoord.y = drawPosition.y;
			coord.PushBack(tmpCoord);
			tmpCoord.x = posDrawX + sizeWidth;
			tmpCoord.y = drawPosition.y + size;
			coord.PushBack(tmpCoord);
			tmpCoord.x = posDrawX;
			coord.PushBack(tmpCoord);
		}
		posDrawX += sizeWidth;
	}
	drawPosition.x = posDrawX;
	
}

int32_t ewol::GetWidth(int32_t fontID, const char * utf8String)
{
	if(fontID>=s_listLoadedFonts.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 0;
	}
	etk::VectorType<UTF8Element_ts> & listOfElement = s_listLoadedFonts[fontID]->GetRefOnElement();
	char * tmpVal = (char*)utf8String;
	
	etkFloat_t posDrawX = 0.0;
	while(*tmpVal != 0) {
		int32_t tmpChar = *tmpVal++;
		int32_t charIndex;
		if (tmpChar >= 0x80) {
			charIndex = 0;
		} else if (tmpChar < 0x20) {
			charIndex = 0;
		} else if (tmpChar < 0x80) {
			charIndex = tmpChar /*- 0x1F*/;
		} else {
			for (int32_t iii=0x80/*-0x20*/; iii < listOfElement.Size(); iii++) {
				if (listOfElement[iii].unicodeCharVal == tmpChar) {
					charIndex = iii;
					break;
				}
			}
			// TODO : Update if possible the mapping
			charIndex = 0;
		}
		posDrawX += listOfElement[charIndex].width;
	}
	return posDrawX;
}

int32_t ewol::GetHeight(int32_t fontID)
{
	if(fontID>=s_listLoadedFonts.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 10;
	}
	return s_listLoadedFonts[fontID]->GetHeight();
}












#endif

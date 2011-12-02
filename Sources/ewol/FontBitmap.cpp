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


#include <ewol/Font.h>
#include <ewol/Texture.h>
#include <etk/VectorType.h>
#include <importgl.h>


#undef __class__
#define __class__	"ewol::FontBitmap"
#if 0

extern "C"
{
	typedef struct {
		texCoord_ts posStart;
		texCoord_ts posStop;
		etkFloat_t ratio;
	}UTF8Element_ts;
}


namespace ewol
{
	class Font
	{
		public:
			Font(etk::File newFile)
			{
				m_loadedOK = false;
				m_filename = newFile;
				for (int32_t iii=0; iii<FONT_MODE_NUMBER; iii++) {
					m_textureId[iii] = 0;
					m_textureLoaded[iii] = false;
				}
				if (m_filename.GetExtention() != "ebt") {
					EWOL_ERROR("Not the coorect extention of the file" << m_filename);
					return;
				}
				if (false == m_filename.Exist()) {
					EWOL_ERROR("File does not Exist ... " << m_filename);
					return;
				}
				
				FILE* File=fopen(m_filename.GetCompleateName().c_str(),"r");
				if(NULL == File) {
					EWOL_ERROR("Can not find the file name=\"" << m_filename << "\"");
					return;
				}
				// load all element of the file ...
				char elementLine[2048];
				int32_t lineID=1;
				while (NULL != fgets(elementLine, 2048, File) )
				{
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
						} else if (0 == strncmp("normal:", elementLine, 7)) {
							char extractString[256] = "";
							sscanf(elementLine, "normal:%s", extractString);
							SetModeFile(FONT_MODE_NORMAL, extractString);
							LoadMode(FONT_MODE_NORMAL);
						} else if (0 == strncmp("bold-italic:", elementLine, 12)) {
							char extractString[256] = "";
							sscanf(elementLine, "bold-italic:%s", extractString);
							SetModeFile(FONT_MODE_BOLD_ITALIC, extractString);
						} else if (0 == strncmp("bold:", elementLine, 5)) {
							char extractString[256] = "";
							sscanf(elementLine, "bold:%s", extractString);
							SetModeFile(FONT_MODE_BOLD, extractString);
						} else if (0 == strncmp("italic:", elementLine, 7)) {
							char extractString[256] = "";
							sscanf(elementLine, "italic:%s", extractString);
							SetModeFile(FONT_MODE_ITALIC, extractString);
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
				fclose(File);
				m_loadedOK = true;
			};
			
			~Font(void)
			{
				for(int32_t iii=0; iii<FONT_MODE_NUMBER; iii++) {
					if (true == m_textureLoaded[iii]) {
						ewol::UnLoadTexture(m_textureId[iii]);
					}
				}
			};
			bool loadedOK(void) { return m_loadedOK; };
		private:
			void ClearAll(int32_t x, int32_t y, int32_t w, int32_t h)
			{
				//EWOL_DEBUG("Find default font glyph : (" << x << "," << y << ") (" << w << "," << h << ") ");
				for (int32_t iii=0; iii< 0x80; iii++) {
					m_listOfElement[iii].posStart.u = (etkFloat_t)x / 512.0;
					m_listOfElement[iii].posStart.v = (etkFloat_t)y / 512.0;
					m_listOfElement[iii].posStop.u = (etkFloat_t)(x+w) / 512.0;
					m_listOfElement[iii].posStop.v = (etkFloat_t)(y+h) / 512.0;
					m_listOfElement[iii].ratio = (etkFloat_t)w/(etkFloat_t)h;
				}
			};
			void SetGlyphID(int32_t utf8Value, int32_t lineID, int32_t x, int32_t y, int32_t w, int32_t h)
			{
				//EWOL_DEBUG("Add font glyph : "<< utf8Value << " (" << x << "," << y << ") (" << w << "," << h << ") ");
				if (utf8Value < 0x80) {
					m_listOfElement[utf8Value].posStart.u = (etkFloat_t)x / 512.0;
					m_listOfElement[utf8Value].posStart.v = (etkFloat_t)y / 512.0;
					m_listOfElement[utf8Value].posStop.u = (etkFloat_t)(x+w) / 512.0;
					m_listOfElement[utf8Value].posStop.v = (etkFloat_t)(y+h) / 512.0;
					m_listOfElement[utf8Value].ratio = (etkFloat_t)w/(etkFloat_t)h;
				} else {
					EWOL_ERROR("not manage glyph with ID > 0x7F line : " << lineID);
				}
			};
			
		public:
			void SetModeFile(ewol::fontMode_te displayMode, etk::String newFile)
			{
				if (displayMode < FONT_MODE_NUMBER) {
					m_bitmapName[displayMode] = newFile;
				}
			};
			
			void LoadMode(ewol::fontMode_te displayMode)
			{
				if (displayMode < FONT_MODE_NUMBER) {
					if (m_bitmapName[displayMode] == "") {
						EWOL_ERROR("Can not load en empty file for the Font : " << m_filename );
						return;
					}
					if (m_textureLoaded[displayMode] == true) {
						EWOL_WARNING("Mode of font is alredy loaded : " << m_filename );
						return;
					}
					etk::String elementName = m_filename.GetFolder();
					elementName += '/';
					elementName += m_bitmapName[displayMode];
					EWOL_INFO("load text font image : \"" << elementName << "\"");
					m_textureId[displayMode] = ewol::LoadTexture(elementName);
					m_textureLoaded[displayMode] = true;
				}
			};
			
			etk::File GetFileName(void)
			{
				return m_filename;
			};
			etk::String GetName(void)
			{
				return m_fontName;
			};
			bool IsLoaded(ewol::fontMode_te displayMode)
			{
				return m_textureLoaded[displayMode];
			};
		private:
			etk::File      m_filename;
			bool           m_loadedOK;
			etk::String    m_fontName;
			uint32_t       m_textureId[FONT_MODE_NUMBER];
			bool           m_textureLoaded[FONT_MODE_NUMBER];
			etk::String    m_bitmapName[FONT_MODE_NUMBER];
			UTF8Element_ts m_listOfElement[0x80];
		public:
			UTF8Element_ts * GetPointerOnElement(void)
			{
				return m_listOfElement;
			};
			uint32_t GetOglId(ewol::fontMode_te displayMode)
			{
				return m_textureId[displayMode];
			};
	};
};

static etk::VectorType<ewol::Font*> listLoadedFonts;

void ewol::UnInitFont(void)
{
	for (int32_t iii=0; iii<listLoadedFonts.Size(); iii++) {
		delete(listLoadedFonts[iii]);
		listLoadedFonts[iii] = NULL;
	}
	listLoadedFonts.Clear();
}

// load a font in momory, can be done many time for a specific fontname, if you specify true the font will be loaded in memory, otherwise, font is loaded only when needed the first time
bool ewol::AddFont(etk::File fontFileName, bool bold, bool italic, bool boldItalic)
{
	for (int32_t iii=0; iii<listLoadedFonts.Size(); iii++) {
		if (listLoadedFonts[iii]->GetFileName() == fontFileName) {
			if (true == bold) {
				listLoadedFonts[iii]->LoadMode(FONT_MODE_BOLD);
			}
			if (true == italic) {
				listLoadedFonts[iii]->LoadMode(FONT_MODE_ITALIC);
			}
			if (true == boldItalic) {
				listLoadedFonts[iii]->LoadMode(FONT_MODE_BOLD_ITALIC);
			}
			return true;
		}
	}
	if (fontFileName.GetExtention() != "ebt") {
		EWOL_ERROR("Not the correct extention of the file" << fontFileName << "supported only *.ebt" );
		return false;
	}
	ewol::Font * tmpFont = new ewol::Font(fontFileName);
	if (false == tmpFont->loadedOK()) {
		EWOL_ERROR("An error apear when loading Font file : " << fontFileName);
		return false;
	}
	if (true == bold) {
		tmpFont->LoadMode(FONT_MODE_BOLD);
	}
	if (true == italic) {
		tmpFont->LoadMode(FONT_MODE_ITALIC);
	}
	if (true == boldItalic) {
		tmpFont->LoadMode(FONT_MODE_BOLD_ITALIC);
	}
	listLoadedFonts.PushBack(tmpFont);
	return true;
}


// get the name of the font 
etk::String ewol::GetFontName(int32_t Id)
{
	if(Id<listLoadedFonts.Size() && Id>=0) {
		return listLoadedFonts[Id]->GetName();
	}
	return "No-Name";
}

int32_t ewol::GetFontIdWithFileName(etk::File fontFileName)
{
	for (int32_t iii=0; iii<listLoadedFonts.Size(); iii++) {
		if (listLoadedFonts[iii]->GetFileName() == fontFileName) {
			return iii;
		}
	}
	return -1;
}

int32_t ewol::GetFontIdWithName(etk::String fontName)
{
	for (int32_t iii=0; iii<listLoadedFonts.Size(); iii++) {
		if (listLoadedFonts[iii]->GetName() == fontName) {
			return iii;
		}
	}
	return -1;
}

// get the size of a long string in UTF8 (note that \n and \r represent unknown char...)
int32_t ewol::GetStringWidth(int32_t fontID, ewol::fontMode_te displayMode, int32_t size, const char * utf8String)
{
	return 0;
}

// get the size of a specific char in UTF8
int32_t ewol::GetCharWidth(int32_t fontID, ewol::fontMode_te displayMode, int32_t size, const char * utf8String)
{
	return 0;
}

// draw the text without background
void ewol::DrawText(int32_t            fontID,
                    ewol::fontMode_te  displayMode,
                    int32_t            size,
                    coord3D_ts &       drawPosition,
                    color_ts           textColorFg,
                    const char *       utf8String)
{
	if(fontID>=listLoadedFonts.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return;
	}
	if (false == listLoadedFonts[fontID]->IsLoaded(displayMode)) {
		listLoadedFonts[fontID]->LoadMode(displayMode);
		if (false == listLoadedFonts[fontID]->IsLoaded(displayMode)) {
			EWOL_ERROR("Can not load Font mode : " << displayMode << "of font " << listLoadedFonts[fontID]->GetName() );
			return;
		}
	}
	UTF8Element_ts * listOfElement = listLoadedFonts[fontID]->GetPointerOnElement();
	char * tmpVal = (char*)utf8String;
	
	glColor4f(textColorFg.red, textColorFg.green, textColorFg.blue, textColorFg.alpha);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, listLoadedFonts[fontID]->GetOglId(displayMode));
	etkFloat_t posDrawX = drawPosition.x;
	while(*tmpVal != '\0') {
		int32_t tmpChar = (int32_t)*tmpVal;
		if (tmpChar >= 0x80) {
			tmpChar = 0;
		}
		etkFloat_t sizeWidth = size * listOfElement[tmpChar].ratio;
		if (tmpChar != 0x20) {
			// TODO : this is really not availlable in the OpenGL ES ==> make with vertex system ...
			glBegin(GL_QUADS);
				//m_listOfElement[utf8Value].ratio = (etkFloat_t)w/(etkFloat_t)h;
				glTexCoord2f(listOfElement[tmpChar].posStart.u, listOfElement[tmpChar].posStart.v);	glVertex3f(posDrawX,             drawPosition.y,             0.0);
				glTexCoord2f(listOfElement[tmpChar].posStop.u,  listOfElement[tmpChar].posStart.v);	glVertex3f(posDrawX + sizeWidth, drawPosition.y,             0.0);
				glTexCoord2f(listOfElement[tmpChar].posStop.u,  listOfElement[tmpChar].posStop.v);	glVertex3f(posDrawX + sizeWidth, drawPosition.y + size, 0.0);
				glTexCoord2f(listOfElement[tmpChar].posStart.u, listOfElement[tmpChar].posStop.v);	glVertex3f(posDrawX,             drawPosition.y + size, 0.0);
			glEnd();
		}
		tmpVal++;
		posDrawX += sizeWidth;
	}
	drawPosition.x = posDrawX;
	glDisable(GL_TEXTURE_2D);
}



void ewol::DrawText(int32_t                        fontID,
                    ewol::fontMode_te              displayMode,
                    int32_t                        size,
                    coord2D_ts &                   drawPosition,
                    const char *                   utf8String,
                    uint32_t &                     fontTextureId,
                    etk::VectorType<coord2D_ts> &  coord,
                    etk::VectorType<texCoord_ts> & coordTex)
{
	if(fontID>=listLoadedFonts.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return;
	}
	if (false == listLoadedFonts[fontID]->IsLoaded(displayMode)) {
		listLoadedFonts[fontID]->LoadMode(displayMode);
		if (false == listLoadedFonts[fontID]->IsLoaded(displayMode)) {
			EWOL_ERROR("Can not load Font mode : " << displayMode << "of font " << listLoadedFonts[fontID]->GetName() );
			return;
		}
	}
	UTF8Element_ts * listOfElement = listLoadedFonts[fontID]->GetPointerOnElement();
	char * tmpVal = (char*)utf8String;
	
	// set id of texture ... (i kwnow it was a little dangerous, but this ID is never remove while the program is running...
	fontTextureId = listLoadedFonts[fontID]->GetOglId(displayMode);
	
	etkFloat_t posDrawX = drawPosition.x;
	while(*tmpVal != '\0') {
		int32_t tmpChar = (int32_t)*tmpVal;
		if (tmpChar >= 0x80) {
			tmpChar = 0;
		}
		etkFloat_t sizeWidth = size * listOfElement[tmpChar].ratio;
		if (tmpChar != 0x20) {
			coordTex.PushBack(listOfElement[tmpChar].posStart);
			texCoord_ts tmpTex;
			tmpTex.u = listOfElement[tmpChar].posStop.u;
			tmpTex.v = listOfElement[tmpChar].posStart.v;
			coordTex.PushBack(tmpTex);
			coordTex.PushBack(listOfElement[tmpChar].posStop);
			tmpTex.u = listOfElement[tmpChar].posStart.u;
			tmpTex.v = listOfElement[tmpChar].posStop.v;
			coordTex.PushBack(tmpTex);
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
		tmpVal++;
		posDrawX += sizeWidth;
	}
	drawPosition.x = posDrawX;
}
#endif



// set default folder name of the font : 
void ewol::SetFontFolder(etk::String folderName)
{
	
}

void ewol::SetDefaultFont(etk::String fontName, int32_t size)
{
	
}

// unload all font loaded
void ewol::InitFont(void)
{
	
}

void ewol::UnInitFont(void)
{
	
}

// load the fonts...
int32_t ewol::LoadFont(etk::String fontName, int32_t size)
{
	return 0;
}

int32_t ewol::GetDefaultFontId(void)
{
	return 0;
}

void ewol::UnloadFont(int32_t id)
{
	
}

// get the size of a long string in UTF8 (note that \n and \r represent unknown char...)
int32_t ewol::GetWidth(int32_t fontID, const uniChar_t * unicodeString)
{
	return 20;
}


int32_t ewol::GetWidth(int32_t fontID, const char *      utf8String)
{
	return 20;
}

int32_t ewol::GetHeight(int32_t fontID)
{
	return 25;
}


void ewol::DrawText(int32_t                        fontID,
                     coord2D_ts &                   drawPosition,
                     const char *                   utf8String,
                     uint32_t &                     fontTextureId,
                     etk::VectorType<coord2D_ts> &  coord,
                     etk::VectorType<texCoord_ts> & coordTex)
{
	
}

void ewol::DrawText(int32_t                        fontID,
                     coord2D_ts &                   drawPosition,
                     const uniChar_t *              unicodeString,
                     uint32_t &                     fontTextureId,
                     etk::VectorType<coord2D_ts> &  coord,
                     etk::VectorType<texCoord_ts> & coordTex)
{
	
}

int32_t ewol::LoadFont(etk::File fontFileName)
{
	
}

void ewol::DrawText(etkFloat_t x, etkFloat_t y, const char * myString)
{
	
}




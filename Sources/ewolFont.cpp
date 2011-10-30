/**
 *******************************************************************************
 * @file ewolFont.cpp
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


#include <ewolFont.h>
#include <ewolTexture.h>
#include <etkVectorType.h>

extern "C"
{
	typedef struct {
		position_ts posStart;
		position_ts posStop;
		intSize_ts size;
	}UTF8Element_ts;
}

static UTF8Element_ts listOfElement[0x80];

static int32_t TextureIdNormal = -1;
static int32_t TextureIdBold = -1;
static int32_t TextureIdBoldItalic = -1;
static int32_t TextureIdItalic = -1;

static bool isInit = false;

int32_t ewol::LoadFont(etk::File fontFileName)
{
	if (true == isInit) {
		EWOL_ERROR("Font is already loaded...");
		return 0;
	}
	if (fontFileName.GetExtention() != "ebt") {
		EWOL_ERROR("Not the coorect extention of the file" << fontFileName);
		return 0;
	}
	if (false == fontFileName.Exist()) {
		EWOL_ERROR("File does not Exist ... " << fontFileName);
		return 0;
	}
	
	FILE* File=fopen(fontFileName.GetCompleateName().c_str(),"r");
	if(NULL == File) {
		EWOL_ERROR("Can not find the file name=\"" << fontFileName << "\"");
		return 0;
	}
	
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
				EWOL_INFO("Find font name : \"" << extractString << "\"");
			} else if (0 == strncmp("normal:", elementLine, 7)) {
				char extractString[256] = "";
				sscanf(elementLine, "normal:%s", extractString);
				etk::String elementName = fontFileName.GetFolder();
				elementName += '/';
				elementName += extractString;
				EWOL_INFO("Find normal font image : \"" << elementName << "\"");
				TextureIdNormal = ewol::LoadTexture(elementName);
			} else if (0 == strncmp("bold-italic:", elementLine, 12)) {
				char extractString[256] = "";
				sscanf(elementLine, "bold-italic:%s", extractString);
				etk::String elementName = fontFileName.GetFolder();
				elementName += '/';
				elementName += extractString;
				EWOL_INFO("Find bold-italic font image : \"" << elementName << "\"");
				TextureIdBoldItalic = ewol::LoadTexture(elementName);
			} else if (0 == strncmp("bold:", elementLine, 5)) {
				char extractString[256] = "";
				sscanf(elementLine, "bold:%s", extractString);
				etk::String elementName = fontFileName.GetFolder();
				elementName += '/';
				elementName += extractString;
				EWOL_INFO("Find bold font image : \"" << elementName << "\"");
				TextureIdBold = ewol::LoadTexture(elementName);
			} else if (0 == strncmp("italic:", elementLine, 7)) {
				char extractString[256] = "";
				sscanf(elementLine, "italic:%s", extractString);
				etk::String elementName = fontFileName.GetFolder();
				elementName += '/';
				elementName += extractString;
				EWOL_INFO("Find italic font image : \"" << elementName << "\"");
				TextureIdItalic = ewol::LoadTexture(elementName);
			} else if (0 == strncmp("0x00", elementLine, 4)) {
				int32_t GlyphPositionX;
				int32_t GlyphPositionY;
				int32_t GlyphSizeX;
				int32_t GlyphSizeY;
				sscanf(elementLine, "0x00 (%d,%d) (%d,%d)", &GlyphPositionX, &GlyphPositionY, &GlyphSizeX, &GlyphSizeY);
				EWOL_INFO("Find default font glyph : (" << GlyphPositionX << "," << GlyphPositionY << ") (" << GlyphSizeX << "," << GlyphSizeY << ") ");
				for (int32_t iii=0; iii< 0x80; iii++) {
					listOfElement[iii].posStart.x = (double)GlyphPositionX / 512.0;
					listOfElement[iii].posStart.y = (double)GlyphPositionY / 512.0;
					listOfElement[iii].posStop.x = (double)(GlyphPositionX+GlyphSizeX) / 512.0;
					listOfElement[iii].posStop.y = (double)(GlyphPositionY+GlyphSizeY) / 512.0;
					listOfElement[iii].size.x = GlyphSizeX;
					listOfElement[iii].size.y = GlyphSizeY;
				}
			} else if (0 == strncmp("0x", elementLine, 2)) {
				uint32_t utf8Value;
				int32_t GlyphPositionX;
				int32_t GlyphPositionY;
				int32_t GlyphSizeX;
				int32_t GlyphSizeY;
				sscanf(elementLine, "0x%x (%d,%d) (%d,%d)", &utf8Value, &GlyphPositionX, &GlyphPositionY, &GlyphSizeX, &GlyphSizeY);
				//EWOL_INFO("Find pos font glyph : " << utf8Value << " (" << GlyphPositionX << "," << GlyphPositionY << ") (" << GlyphSizeX << "," << GlyphSizeY << ") ");
				if (utf8Value < 0x80) {
					listOfElement[utf8Value].posStart.x = (double)GlyphPositionX / 512.0;
					listOfElement[utf8Value].posStart.y = (double)GlyphPositionY / 512.0;
					listOfElement[utf8Value].posStop.x = (double)(GlyphPositionX+GlyphSizeX) / 512.0;
					listOfElement[utf8Value].posStop.y = (double)(GlyphPositionY+GlyphSizeY) / 512.0;
					listOfElement[utf8Value].size.x = GlyphSizeX;
					listOfElement[utf8Value].size.y = GlyphSizeY;
				} else {
					EWOL_ERROR("not manage glyph with ID > 0x7F line : " << lineID << "\"" << elementLine << "\"");
				}
			} else {
				EWOL_ERROR("error when parsing the line : " << lineID << "\"" << elementLine << "\"");
			}
		}
		
		lineID++;
	}
	
	isInit = true;
	// return the font Id :
	return 0;
}


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glut.h>
#if defined(EWOL_X11_MODE__XF86V)
#	include <X11/extensions/xf86vmode.h>
#elif defined(EWOL_X11_MODE__XRENDER)
#	include <X11/extensions/Xrender.h>
#endif

void ewol::DrawText(double x, double y, const char * myString)
{
	char * tmpVal = (char*)myString;
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureIdBold);
	
	while(*tmpVal != '\0') {
		int32_t tmpChar = (int32_t)*tmpVal;
		if (tmpChar >= 0x80) {
			tmpChar = 0;
		}
		glBegin(GL_QUADS);
			glTexCoord2f(listOfElement[tmpChar].posStart.x, listOfElement[tmpChar].posStart.y);
			glVertex3f(x,                             y, 0.0);
			glTexCoord2f(listOfElement[tmpChar].posStop.x, listOfElement[tmpChar].posStart.y);
			glVertex3f(x + listOfElement[tmpChar].size.x, y, 0.0);
			glTexCoord2f(listOfElement[tmpChar].posStop.x, listOfElement[tmpChar].posStop.y);
			glVertex3f(x + listOfElement[tmpChar].size.x, y + listOfElement[tmpChar].size.y, 0.0);
			glTexCoord2f(listOfElement[tmpChar].posStart.x, listOfElement[tmpChar].posStop.y);
			glVertex3f(x                            , y + listOfElement[tmpChar].size.y, 0.0);
		glEnd();
		tmpVal++;
		x += listOfElement[tmpChar].size.x;
	}
	glDisable(GL_TEXTURE_2D);
}


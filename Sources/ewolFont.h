/**
 *******************************************************************************
 * @file ewolFont.h
 * @brief ewol Font system (header)
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

#ifndef __EWOL_FONT_H__
#define __EWOL_FONT_H__

#include <etkTypes.h>
#include <ewolDebug.h>
#include <etkFile.h>


namespace ewol
{
	typedef enum {
		FONT_MODE_NORMAL=0,
		FONT_MODE_BOLD,
		FONT_MODE_ITALIC,
		FONT_MODE_BOLD_ITALIC,
		FONT_MODE_NUMBER,
	}fontMode_te;
	
	void       UnInitFont(void);
	// load a font in momory, can be done many time for a specific fontname, if you specify true the font will be loaded in memory, otherwise, font is loaded only when needed the first time
	bool        AddFont(etk::File fontFileName, bool bold=false, bool italic=false, bool boldItalic=false);
	// get the name of the font 
	etk::String GetFontName(int32_t Id);
	int32_t     GetFontIdWithFileName(etk::File fontFileName);
	int32_t     GetFontIdWithName(etk::String fontName);
	
	// get the size of a long string in UTF8 (note that \n and \r represent unknown char...)
	int32_t     GetStringWidth(int32_t fontID, ewol::fontMode_te displayMode, int32_t size, const char * utf8String);
	// get the size of a specific char in UTF8
	int32_t     GetCharWidth(  int32_t fontID, ewol::fontMode_te displayMode, int32_t size, const char * utf8String);// only one element in the UTF8 string ...
	// draw the text without background
	void        DrawText(      int32_t            fontID,
	                           ewol::fontMode_te  displayMode,
	                           int32_t            size,
	                           coord3D_ts &       drawPosition,
	                           color_ts           textColorFg,
	                           const char *       utf8String);
	// draw the text with a spécify background
	void        DrawTextWithBg(int32_t            fontID,
	                           ewol::fontMode_te  displayMode,
	                           int32_t            size,
	                           coord3D_ts &       drawPosition,
	                           color_ts           textColorFg,
	                           color_ts           textColorBg,
	                           const char *       utf8String);
	
	// draw the text without background
	void        DrawText(      int32_t                        fontID,
	                           ewol::fontMode_te              displayMode,
	                           int32_t                        size,
	                           coord2D_ts &                   drawPosition,
	                           const char *                   utf8String,
	                           uint32_t &                     fontTextureId,
	                           etk::VectorType<coord2D_ts> &  coord,
	                           etk::VectorType<texCoord_ts> & coordTex);
	
	int32_t LoadFont(etk::File fontFileName);
	void    DrawText(double x, double y, const char * myString);
};

#endif


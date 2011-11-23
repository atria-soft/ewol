/**
 *******************************************************************************
 * @file ewol/Font.h
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

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <etk/File.h>


namespace ewol
{
	// TODO : Create a subNameSpace:
	/*
	namespace font {
		// set default folder name of the font : 
		void        SetFolder(etk::String folderName);
		void        SetDefault(etk::String fontName, int32_t size);
		// unload all font loaded
		void        Init(void);
		void        UnInit(void);
		// load the fonts...
		int32_t     Load(etk::String fontName, int32_t size); // return ID of font
		int32_t     GetDefaultId(void);
		void        Unload(int32_t id);
		
		// get the size of a long string in UTF8 (note that \n and \r represent unknown char...)
		int32_t     GetWidth(int32_t fontID, const uniChar_t * unicodeString);
		int32_t     GetWidth(int32_t fontID, const char *      utf8String);
		int32_t     GetHeight(int32_t fontID);
		
		
		void        DrawText(int32_t                        fontID,
		                     coord2D_ts &                   drawPosition,
		                     const char *                   utf8String,
		                     uint32_t &                     fontTextureId,
		                     etk::VectorType<coord2D_ts> &  coord,
		                     etk::VectorType<texCoord_ts> & coordTex);
		
		void        DrawText(int32_t                        fontID,
		                     coord2D_ts &                   drawPosition,
		                     const uniChar_t *              unicodeString,
		                     uint32_t &                     fontTextureId,
		                     etk::VectorType<coord2D_ts> &  coord,
		                     etk::VectorType<texCoord_ts> & coordTex);
		
		int32_t Load(etk::File fontFileName);
		void    DrawText(etkFloat_t x, etkFloat_t y, const char * myString);
	};
	*/
	// set default folder name of the font : 
	void        SetFontFolder(etk::String folderName);
	void        SetDefaultFont(etk::String fontName, int32_t size);
	// unload all font loaded
	void        InitFont(void);
	void        UnInitFont(void);
	// load the fonts...
	int32_t     LoadFont(etk::String fontName, int32_t size); // return ID of font
	int32_t     GetDefaultFontId(void);
	void        UnloadFont(int32_t id);
	
	// get the size of a long string in UTF8 (note that \n and \r represent unknown char...)
	int32_t     GetWidth(int32_t fontID, const uniChar_t * unicodeString);
	int32_t     GetWidth(int32_t fontID, const char *      utf8String);
	int32_t     GetHeight(int32_t fontID);
	
	
	void        DrawText(int32_t                        fontID,
	                     coord2D_ts &                   drawPosition,
	                     const char *                   utf8String,
	                     uint32_t &                     fontTextureId,
	                     etk::VectorType<coord2D_ts> &  coord,
	                     etk::VectorType<texCoord_ts> & coordTex);
	
	void        DrawText(int32_t                        fontID,
	                     coord2D_ts &                   drawPosition,
	                     const uniChar_t *              unicodeString,
	                     uint32_t &                     fontTextureId,
	                     etk::VectorType<coord2D_ts> &  coord,
	                     etk::VectorType<texCoord_ts> & coordTex);
	
	int32_t LoadFont(etk::File fontFileName);
	void    DrawText(etkFloat_t x, etkFloat_t y, const char * myString);
};

#endif


/**
 *******************************************************************************
 * @file ewol/font/FontFreeType.cpp
 * @brief ewol Font system wrapper on freetype(sources)
 * @author Edouard DUPIN
 * @date 05/11/2011
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

#include <ewol/font/Font.h>
#include <ewol/texture/Texture.h>
#include <etk/unicode.h>
#include <etk/Vector.h>
#include <ewol/font/FontFreeType.h>

#include <ewol/openGl.h>
extern "C" {
	#include <freetype/ft2build.h>
}
#include FT_FREETYPE_H

#undef __class__
#define __class__	"ewol::FontFreeType"


// free Font hnadle of librairies ... entry for acces ...
static FT_Library library;

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


void ewol::FreeTypeInit(void)
{
	EWOL_DEBUG("==> Init Font-Manager");
	int32_t error = FT_Init_FreeType( &library );
	if(0 != error) {
		EWOL_CRITICAL(" when loading FreeType Librairy ...");
	}
}

void ewol::FreeTypeUnInit(void)
{
	EWOL_DEBUG("==> Un-Init Font-Manager");
	/*int32_t error = FT_Done_FreeType( library );
	library = NULL;
	if(0 != error) {
		EWOL_CRITICAL(" when Un-loading FreeType Librairy ...");
	}
	*/
}



ewol::FontFreeType::FontFreeType(etk::UString fontFolder, etk::UString fontName) :
	Font(fontFolder, fontName)
{
	m_init = false;
	m_FileBuffer = NULL;
	m_FileSize = 0;
	etk::UString tmpFileName = fontFolder + "/" + fontName;
	
	etk::File myfile(tmpFileName, etk::FILE_TYPE_DATA);
	if (false == myfile.Exist()) {
		EWOL_ERROR("File Does not exist : " << myfile);
		return;
	}
	m_FileSize = myfile.Size();
	if (0==m_FileSize) {
		EWOL_ERROR("This file is empty : " << myfile);
		return;
	}
	if (false == myfile.fOpenRead()) {
		EWOL_ERROR("Can not open the file : " << myfile);
		return;
	}
	// allocate data
	m_FileBuffer = new FT_Byte[m_FileSize];
	if (NULL == m_FileBuffer) {
		EWOL_ERROR("Error Memory allocation size=" << tmpFileName);
		return;
	}
	// load data from the file :
	myfile.fRead(m_FileBuffer, 1, m_FileSize);
	// close the file:
	myfile.fClose();
	// load Face ...
	int32_t error = FT_New_Memory_Face( library, m_FileBuffer, m_FileSize, 0, &m_fftFace );
	if( FT_Err_Unknown_File_Format == error) {
		EWOL_ERROR("... the font file could be opened and read, but it appears ... that its font format is unsupported");
	} else if (0 != error) {
		EWOL_ERROR("... another error code means that the font file could not ... be opened or read, or simply that it is broken...");
	} else {
		// all OK
		EWOL_INFO("load font : \"" << tmpFileName << "\" ");
		//Display();
		m_init = true;
	}
}

ewol::FontFreeType::~FontFreeType(void)
{
	// clean the tmp memory
	if (NULL != m_FileBuffer) {
		delete[] m_FileBuffer;
		m_FileBuffer = NULL;
	}
	// must be deleted fftFace
}

int32_t ewol::FontFreeType::Draw(draw::Image&         imageOut,
                                 int32_t              fontSize,
                                 Vector2D<float>      textPos,
                                 const etk::UString&  unicodeString,
                                 draw::Color&         textColor)
{
	if(false==m_init) {
		return 0;
	}
	return 0;
}

int32_t ewol::FontFreeType::Draw(draw::Image&     imageOut,
                                 int32_t          fontSize,
                                 Vector2D<float>  textPos,
                                 const uniChar_t  unicodeChar,
                                 draw::Color&     textColor)
{
	if(false==m_init) {
		return 0;
	}
	return 0;
}

Vector2D<float> ewol::FontFreeType::GetSize(int32_t fontSize, const etk::UString & unicodeString)
{
	if(false==m_init) {
		return Vector2D<float>(0,0);
	}
	Vector2D<float> outputSize(0,0);
	return outputSize;
}

int32_t ewol::FontFreeType::GetHeight(int32_t fontSize)
{
	return 0;
}

bool ewol::FontFreeType::GenerateBitmapFont(int32_t size, int32_t &height, ewol::Texture & texture, etk::Vector<freeTypeFontElement_ts> & listElement)
{
	if(false==m_init) {
		return false;
	}
	// get the pointer on the image :
	draw::Image& myImage = texture.Get();
	
	// 300dpi (hight quality) 96 dpi (normal quality)
	int32_t fontQuality = 96;
	//int32_t fontQuality = 150;
	//int32_t fontQuality = 300;
	// Select Size ...
	// note tha <<6==*64 corespond with the 1/64th of points calculation of freetype
	int32_t error = FT_Set_Char_Size(m_fftFace, size<<6, size<<6, fontQuality, fontQuality);
	// the line height to have a correct display
	height = size*1.43f;

	// a small shortcut
	FT_GlyphSlot slot = m_fftFace->glyph;
	/*
	EWOL_DEBUG("Max size for ths glyph size=" << size << 
	           " is (" << m_fftFace->max_advance_width << "," << m_fftFace->max_advance_height << ")" <<
	           "?=(" << (m_fftFace->max_advance_width>>6) << "," << (m_fftFace->max_advance_height>>6) << ")");
	*/
	// retrieve glyph index from character code 
	int32_t glyph_index = FT_Get_Char_Index(m_fftFace, 'A' );
	// load glyph image into the slot (erase previous one)
	error = FT_Load_Glyph(m_fftFace, // handle to face object
	                      glyph_index, // glyph index
	                      FT_LOAD_DEFAULT );
	if ( error ) {
		EWOL_ERROR("FT_Load_Glyph");
	}
	EWOL_DEBUG("linearHoriAdvance=" << (slot->linearHoriAdvance >> 6));
	EWOL_DEBUG("linearVertAdvance=" << (slot->linearVertAdvance >> 6));
	EWOL_DEBUG("metrics.horiAdvance=" << (slot->metrics.horiAdvance >> 6));
	EWOL_DEBUG("metrics.vertAdvance=" << (slot->metrics.vertAdvance >> 6));
	
	int32_t nbElement = listElement.Size();
	int32_t coter = simpleSQRT(nbElement);
	// note : +1 is for the overlapping of the glyph (Part 1)
	int32_t glyphMaxWidth = /*(m_fftFace->max_advance_width>>6); */(slot->metrics.horiAdvance>>6) +1;
	int32_t glyphMaxHeight = /*(m_fftFace->max_advance_height>>6); */(slot->metrics.vertAdvance>>6) +1;
	int32_t textureWidth = nextP2(coter*glyphMaxWidth);
	int32_t nbRaws = textureWidth / glyphMaxWidth;
	if (nbRaws <= 0) {
		EWOL_ERROR("devide by 0");
		nbRaws = 1;
	}
	int32_t nbLine = (nbElement / nbRaws) + 1;
	int32_t textureHeight = nextP2(nbLine*glyphMaxHeight);
	EWOL_DEBUG("Generate a text texture for char(" << nbRaws << "," << nbLine << ") with size=(" << textureWidth << "," << textureHeight << ")");
	// resize must be done on the texture ...
	texture.SetImageSize(Vector2D<int32_t>(textureWidth,textureHeight));
	// now we can acces directly on the image
	myImage.SetFillColor(draw::Color(0xFFFFFF00));
	myImage.Clear();
	
	int32_t tmpRowStartPos = 0;
	int32_t tmpLineStartPos = 0;
	int32_t CurrentLineHigh = 0;
	// Generate for All Elements :
	for (int32_t iii=0; iii<listElement.Size(); iii++) {
		// increment the position of the texture
		/*
		if (iii!=0) {
			tmpRowId++;
			if (tmpRowId>=nbRaws) {
				tmpRowId = 0;
				tmpLineId++;
			}
		}
		*/
		// retrieve glyph index from character code 
		glyph_index = FT_Get_Char_Index(m_fftFace, listElement[iii].unicodeCharVal );
		/*
		if (glyph_index < 1) {
			EWOL_WARNING("Can not load Glyph : " << listElement[iii].unicodeCharVal);
		}
		*/
		// load glyph image into the slot (erase previous one)
		error = FT_Load_Glyph(m_fftFace, // handle to face object
		                      glyph_index, // glyph index
		                      FT_LOAD_DEFAULT );
		if ( error ) {
			EWOL_ERROR("FT_Load_Glyph");
		}
		
		// convert to an anti-aliased bitmap
		error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL );
		if ( error ) {
			EWOL_ERROR("FT_Render_Glyph");
		}
		int32_t tmpWidth=slot->bitmap.width;
		int32_t tmpHeight=slot->bitmap.rows;
		// check if we have enought place on the bitmap to add the current element : 
		if (tmpRowStartPos+tmpWidth >= textureWidth) {
			tmpRowStartPos = 0;
			tmpLineStartPos += CurrentLineHigh;
			CurrentLineHigh = 0;
			EWOL_ASSERT(tmpLineStartPos+tmpHeight < textureHeight, "Texture dimention estimatiuon error for the current Font");
		}
		
		/*
		EWOL_VERBOSE("elem=" << listElement[iii].unicodeCharVal
		              <<" size=(" << tmpWidth << "," << tmpHeight << ")"
		              << " for bitmap (left=" << slot->bitmap_left << ",top=" << slot->bitmap_top << ")");
		EWOL_VERBOSE(" BEARING=(" << (slot->metrics.horiBearingX>>6) << "," << (slot->metrics.vertBearingY>>6) << ")" );
		*/
		for(int32_t j=0; j < tmpHeight;j++) {
			for(int32_t i=0; i < tmpWidth; i++){
				draw::Color tlpppp(0xFF,0xFF,0xFF,slot->bitmap.buffer[i + tmpWidth*j]);
				//EWOL_DEBUG(" plop : " << tlpppp);
				myImage.Set(Vector2D<int32_t>(tmpRowStartPos+i, tmpLineStartPos+j), tlpppp );
			}
		}
		listElement[iii].bearing.x  = slot->metrics.horiBearingX>>6;
		listElement[iii].bearing.y  = slot->metrics.horiBearingY>>6;
		listElement[iii].size.x     = tmpWidth;
		listElement[iii].size.y     = tmpHeight;
		listElement[iii].advance    = slot->metrics.horiAdvance>>6;
		listElement[iii].posStart.u = (float)(tmpRowStartPos) / (float)textureWidth;
		listElement[iii].posStart.v = (float)(tmpLineStartPos) / (float)textureHeight;
		listElement[iii].posStop.u  = (float)(tmpRowStartPos + tmpWidth) / (float)textureWidth;
		listElement[iii].posStop.v  = (float)(tmpLineStartPos + tmpHeight) / (float)textureHeight;
		
		// update the maximum of the line hight : 
		if (CurrentLineHigh<tmpHeight) {
			// note : +1 is for the overlapping of the glyph (Part 2)
			CurrentLineHigh = tmpHeight+1;
		}
		// note : +1 is for the overlapping of the glyph (Part 3)
		// update the Bitmap position drawing : 
		tmpRowStartPos += tmpWidth+1;
	}
	EWOL_DEBUG("End generation of the Fond bitmap, start adding texture");
	texture.Flush();
	EWOL_DEBUG("end load remode tmp data");

	return false;
}

void ewol::FontFreeType::Display(void)
{
	if(false==m_init) {
		return;
	}
	EWOL_INFO("    nuber of glyph       = " << (int)m_fftFace->num_glyphs);
	if ((FT_FACE_FLAG_SCALABLE & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_SCALABLE (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_SCALABLE (disable)");
	}
	if ((FT_FACE_FLAG_FIXED_SIZES & m_fftFace->face_flags) != 0) {
			EWOL_INFO("    flags                = FT_FACE_FLAG_FIXED_SIZES (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_FIXED_SIZES (disable)");
	}
	if ((FT_FACE_FLAG_FIXED_WIDTH & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_FIXED_WIDTH (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_FIXED_WIDTH (disable)");
	}
	if ((FT_FACE_FLAG_SFNT & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_SFNT (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_SFNT (disable)");
	}
	if ((FT_FACE_FLAG_HORIZONTAL & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_HORIZONTAL (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_HORIZONTAL (disable)");
	}
	if ((FT_FACE_FLAG_VERTICAL & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_VERTICAL (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_VERTICAL (disable)");
	}
	if ((FT_FACE_FLAG_KERNING & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_KERNING (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_KERNING (disable)");
	}
	/* Deprecated flag
	if ((FT_FACE_FLAG_FAST_GLYPHS & face->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_FAST_GLYPHS (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_FAST_GLYPHS (disable)");
	}
	*/
	if ((FT_FACE_FLAG_MULTIPLE_MASTERS & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_MULTIPLE_MASTERS (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_MULTIPLE_MASTERS (disable)");
	}
	if ((FT_FACE_FLAG_GLYPH_NAMES & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_GLYPH_NAMES (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_GLYPH_NAMES (disable)");
	}
	if ((FT_FACE_FLAG_EXTERNAL_STREAM & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_EXTERNAL_STREAM (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_EXTERNAL_STREAM (disable)");
	}
	if ((FT_FACE_FLAG_HINTER & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_HINTER (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_HINTER (disable)");
	}
	if ((FT_FACE_FLAG_CID_KEYED & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_CID_KEYED (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_CID_KEYED (disable)");
	}
	/*
	if ((FT_FACE_FLAG_TRICKY & m_fftFace->face_flags) != 0) {
		EWOL_INFO("    flags                = FT_FACE_FLAG_TRICKY (enable)");
	} else {
		EWOL_DEBUG("    flags                = FT_FACE_FLAG_TRICKY (disable)");
	}
	*/
	EWOL_INFO("    unit per EM          = " << m_fftFace->units_per_EM);
	EWOL_INFO("    num of fixed sizes   = " << m_fftFace->num_fixed_sizes);
	//EWOL_INFO("    Availlable sizes     = " << (int)m_fftFace->available_sizes);
	
	//EWOL_INFO("    Current size         = " << (int)m_fftFace->size);
}




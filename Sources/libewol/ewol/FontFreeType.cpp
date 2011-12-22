/**
 *******************************************************************************
 * @file ewol/FontFreeType.cpp
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
#ifdef EWOL_USE_FREE_TYPE

#include <ewol/Font.h>
#include <ewol/Texture.h>
#include <etk/VectorType.h>

/*
#include <GL/glx.h>
#include <GL/glut.h>
#if defined(EWOL_X11_MODE__XF86V)
#	include <X11/extensions/xf86vmode.h>
#elif defined(EWOL_X11_MODE__XRENDER)
#	include <X11/extensions/Xrender.h>
#endif
*/
#include <ewol/importgl.h>
#if defined(__PLATFORM__Linux)
#	include <ft2build.h>
#else
#	include <freetype/ft2build.h>
#endif

#include FT_FREETYPE_H

#undef __class__
#define __class__	"ewol::FontFreeType"



extern "C"
{
	typedef struct {
		uniChar_t   unicodeCharVal;
		int32_t     width;
		texCoord_ts posStart;
		texCoord_ts posStop;
		etkFloat_t ratio;
	}freeTypeFontElement_ts;
};

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

void ewol::InitFont(void)
{
	int32_t error = FT_Init_FreeType( &library );
	if(0 != error) {
		EWOL_CRITICAL(" when loading FreeType Librairy ...");
	}
}

// keep only one instance of every font in freetype
class FTFontInternal
{
	private:
		void Display(void)
		{
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
			if ((FT_FACE_FLAG_TRICKY & m_fftFace->face_flags) != 0) {
				EWOL_INFO("    flags                = FT_FACE_FLAG_TRICKY (enable)");
			} else {
				EWOL_DEBUG("    flags                = FT_FACE_FLAG_TRICKY (disable)");
			}
			EWOL_INFO("    unit per EM          = " << m_fftFace->units_per_EM);
			EWOL_INFO("    num of fixed sizes   = " << m_fftFace->num_fixed_sizes);
			//EWOL_INFO("    Availlable sizes     = " << (int)m_fftFace->available_sizes);
			
			//EWOL_INFO("    Current size         = " << (int)m_fftFace->size);
		}
	public:
		FTFontInternal(etk::File fontFileName, etk::String fontName)
		{
			m_fontName = fontName;
			m_fileName = fontFileName;
			m_FileBuffer = NULL;
			m_FileSize = 0;
			#if 0
				int32_t error = FT_New_Face( library, m_fileName.GetCompleateName().c_str(), 0, &m_fftFace );
			#else
				if (false == m_fileName.Exist()) {
					EWOL_ERROR("File Does not exist : " << m_fileName);
					return;
				}
				m_FileSize = m_fileName.Size();
				if (0==m_FileSize) {
					EWOL_ERROR("This file is empty : " << m_fileName);
					return;
				}
				if (false == m_fileName.fOpenRead()) {
					EWOL_ERROR("Can not open the file : " << m_fileName);
					return;
				}
				// allocate data
				m_FileBuffer = new FT_Byte[m_FileSize];
				if (NULL == m_FileBuffer) {
					EWOL_ERROR("Error Memory allocation size=" << m_FileSize);
					return;
				}
				// load data from the file :
				m_fileName.fRead(m_FileBuffer, 1, m_FileSize);
				// close the file:
				m_fileName.fClose();
				// load Face ...
				int32_t error = FT_New_Memory_Face( library, m_FileBuffer, m_FileSize, 0, &m_fftFace );
			#endif
			if( FT_Err_Unknown_File_Format == error) {
				EWOL_ERROR("... the font file could be opened and read, but it appears ... that its font format is unsupported");
			} else if (0 != error) {
				EWOL_ERROR("... another error code means that the font file could not ... be opened or read, or simply that it is broken...");
			} else {
				// all OK
				EWOL_INFO("load font : \"" << m_fileName << "\" ");
				Display();
			}
		}
		~FTFontInternal(void)
		{
			// clean the tmp memory
			if (NULL != m_FileBuffer) {
				delete[] m_FileBuffer;
				m_FileBuffer = NULL;
			}
		}
	public:
		etk::String GetFontName(void) {return m_fontName;};
		bool GenerateBitmapFont(int32_t size, int32_t &height, int32_t textureId, etk::VectorType<freeTypeFontElement_ts> & listElement)
		{
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
			
			EWOL_DEBUG("Max size for ths glyph size=" << size << " is (" << m_fftFace->max_advance_width << "," << m_fftFace->max_advance_height << ")");
			
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
			int32_t glyphMaxWidth = slot->metrics.horiAdvance>>6;
			int32_t glyphMaxHeight = slot->metrics.vertAdvance>>6;
			int32_t textureWidth = nextP2(coter*glyphMaxWidth);
			int32_t nbRaws = textureWidth / glyphMaxWidth;
			int32_t nbLine = (nbElement / nbRaws) + 1;
			int32_t textureHeight = nextP2(nbLine*glyphMaxHeight);
			EWOL_DEBUG("Generate a text texture for char(" << nbRaws << "," << nbLine << ") with size=(" << textureWidth << "," << textureHeight << ")");
			
			// Allocate Memory For The Texture Data.
			GLubyte* expanded_data = new GLubyte[ 2 * textureWidth * textureHeight];
			// clean the data : 
			for(int j=0; j <textureHeight;j++) {
				for(int i=0; i < textureWidth; i++){
					expanded_data[2*(i+j*textureWidth)+0] = 0;
					expanded_data[2*(i+j*textureWidth)+1] = 0;
				}
			}
			
			int32_t tmpRowId = 0;
			int32_t tmpLineId = 0;
			// Generate for All Elements :
			for (int32_t iii=0; iii<listElement.Size(); iii++) {
				// increment the position of the texture
				if (iii!=0) {
					tmpRowId++;
					if (tmpRowId>=nbRaws) {
						tmpRowId = 0;
						tmpLineId++;
					}
				}
				// retrieve glyph index from character code 
				glyph_index = FT_Get_Char_Index(m_fftFace, listElement[iii].unicodeCharVal );
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
				/*
				EWOL_DEBUG("elem=" << listElement[iii].unicodeCharVal
				           <<" size=(" << tmpWidth << "," << tmpHeight << ")"
				           << " for bitmap (left=" << slot->bitmap_left << ",top=" << slot->bitmap_top << ")");
				EWOL_DEBUG(" BEARING=(" << (slot->metrics.horiBearingX>>6) << "," << (slot->metrics.vertBearingY>>6) << ")" );
				*/
				for(int32_t j=0; j < tmpHeight;j++) {
					for(int32_t i=0; i < tmpWidth; i++){
						int32_t position = 2*(   (tmpRowId *glyphMaxWidth  + i /*+ (slot->metrics.horiBearingX>>6)*/ )
						                       + (tmpLineId*glyphMaxHeight + j + (size-(slot->metrics.horiBearingY>>6)) ) * textureWidth);
						//EWOL_DEBUG(" BEARING=(" << i << "," << j << ") pos=" << position);
						expanded_data[position+0] = slot->bitmap.buffer[i + tmpWidth*j];
						expanded_data[position+1] = slot->bitmap.buffer[i + tmpWidth*j];
					}
				}
				listElement[iii].width = glyphMaxWidth;
				listElement[iii].posStart.u = (etkFloat_t)(tmpRowId *glyphMaxWidth) / (etkFloat_t)textureWidth;
				listElement[iii].posStart.v = (etkFloat_t)(tmpLineId*glyphMaxHeight) / (etkFloat_t)textureHeight;
				listElement[iii].posStop.u = (etkFloat_t)(tmpRowId *glyphMaxWidth + glyphMaxWidth) / (etkFloat_t)textureWidth;
				listElement[iii].posStop.v = (etkFloat_t)(tmpLineId*glyphMaxHeight + glyphMaxHeight + 1) / (etkFloat_t)textureHeight;
			}
			// Now We Just Setup Some Texture Parameters.
			glBindTexture( GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			
			// Here We Actually Create The Texture Itself, Notice That We Are Using GL_LUMINANCE_ALPHA To Indicate That we Are Using 2 Channel Data.
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
			
			// With The Texture Created, We Don't Need The Expanded Data Anymore.
			delete [] expanded_data;
		
			return false;
		}
	private:
		etk::String  m_fontName;
		etk::File    m_fileName;
		FT_Byte *    m_FileBuffer;
		int32_t      m_FileSize;
		FT_Face      m_fftFace;
};

static etk::VectorType<FTFontInternal*> m_listLoadedTTFont;


static etk::String s_currentFolderName = "";
static etk::String s_currentDefaultFontName = "";
static int32_t s_currentDefaultFontId = -1;

class FTFont{
	public:
		FTFont(etk::File fontfileName, etk::String fontName, int32_t size)
		{
			m_trueTypeFontId = -1;
			for (int32_t iii=0; iii < m_listLoadedTTFont.Size(); iii++) {
				if (m_listLoadedTTFont[iii]->GetFontName() == fontName) {
					m_trueTypeFontId = iii;
				}
			}
			if (-1==m_trueTypeFontId) {
				// load a new one ...
				FTFontInternal * tmpElement = new FTFontInternal(fontfileName, fontName);
				m_listLoadedTTFont.PushBack(tmpElement);
				m_trueTypeFontId = m_listLoadedTTFont.Size() -1;
			}
			// set the bassic charset:
			m_elements.Clear();
			freeTypeFontElement_ts tmpchar1;
			tmpchar1.unicodeCharVal = 0;
			m_elements.PushBack(tmpchar1);
			for (int32_t iii=0x20; iii<127; iii++) {
				freeTypeFontElement_ts tmpchar;
				tmpchar.unicodeCharVal = iii;
				m_elements.PushBack(tmpchar);
			}
			m_size = size;
			//generate font
			glGenTextures(1, &m_textureId);
			m_listLoadedTTFont[m_trueTypeFontId]->GenerateBitmapFont(m_size, m_lineHeight, m_textureId, m_elements);
		}
		~FTFont(void)
		{
			
		}
		bool Check(etk::String fontName, int32_t size)
		{
			if (m_trueTypeFontId == -1) {
				return false;
			}
			if(    m_listLoadedTTFont[m_trueTypeFontId]->GetFontName() == fontName
			    && m_size == size)
			{
				return true;
			}
			return false;
		};
		
		etk::VectorType<freeTypeFontElement_ts> & GetRefOnElement(void)
		{
			return m_elements;
		};
		
		uint32_t GetOglId(void)
		{
			return m_textureId;
		};
		
		int32_t GetSize(void)
		{
			return m_size;
		};
		
		int32_t GetHeight(void)
		{
			return m_lineHeight;
		};
		
	private:
		int32_t                                   m_trueTypeFontId;
		uint32_t                                  m_textureId;   // internal texture ID
		int32_t                                   m_size;        // nb pixel height
		int32_t                                   m_lineHeight;  // nb pixel height
		int32_t                                   m_interline;   // nb pixel between 2 lines
		etk::VectorType<freeTypeFontElement_ts>   m_elements;    // 
		
};

static etk::VectorType<FTFont*> m_listLoadedFont;

#undef __class__
#define __class__	"ewol::FontFreeType"

void ewol::SetFontFolder(etk::String folderName)
{
	if (s_currentFolderName != "") {
		EWOL_WARNING("Change the FontFolder, old=\"" << s_currentFolderName << "\"");
	}
	EWOL_TODO("Check if folder exist");
	s_currentFolderName = folderName;
	EWOL_INFO("New default font folder name=\"" << s_currentFolderName << "\"");
}


void ewol::UnInitFont(void)
{
	EWOL_TODO("later");
}

void ewol::SetDefaultFont(etk::String fontName, int32_t size)
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

int32_t ewol::LoadFont(etk::String fontName, int32_t size)
{
	// check if folder file
	etk::String tmpFileName = s_currentFolderName + "/" + fontName + ".ttf";
	etk::File fileName(tmpFileName);
	if (false == fileName.Exist()) {
		EWOL_ERROR("Font does not exist: \"" << fileName.GetCompleateName() << "\"");
		return -1;
	}
	for (int32_t iii=0; iii < m_listLoadedFont.Size(); iii++) {
		if (true == m_listLoadedFont[iii]->Check(fontName, size)) {
			return iii;
		}
	}
	FTFont * tmpFont = new FTFont(fileName, fontName, size);
	m_listLoadedFont.PushBack(tmpFont);
	return m_listLoadedFont.Size()-1;
}

void ewol::UnloadFont(int32_t id)
{
	EWOL_TODO("I do not think it was a good idea... will be done later");
}





void ewol::DrawText(int32_t                        fontID,
                    coord2D_ts &                   drawPosition,
                    const uniChar_t *              unicodeString,
                    uint32_t &                     fontTextureId,
                    etk::VectorType<coord2D_ts> &  coord,
                    etk::VectorType<texCoord_ts> & coordTex)
{
	if(fontID>=m_listLoadedFont.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return;
	}
	etk::VectorType<freeTypeFontElement_ts> & listOfElement = m_listLoadedFont[fontID]->GetRefOnElement();
	
	fontTextureId = m_listLoadedFont[fontID]->GetOglId();
	int32_t size = m_listLoadedFont[fontID]->GetHeight();
	
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


void ewol::DrawText(int32_t                        fontID,
                    coord2D_ts &                   drawPosition,
                    const char *                   utf8String,
                    uint32_t &                     fontTextureId,
                    etk::VectorType<coord2D_ts> &  coord,
                    etk::VectorType<texCoord_ts> & coordTex)
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
	if(fontID>=m_listLoadedFont.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return;
	}
	etk::VectorType<freeTypeFontElement_ts> & listOfElement = m_listLoadedFont[fontID]->GetRefOnElement();
	char * tmpVal = (char*)utf8String;
	
	fontTextureId = m_listLoadedFont[fontID]->GetOglId();
	int32_t size = m_listLoadedFont[fontID]->GetHeight();
	
	etkFloat_t posDrawX = drawPosition.x;
	while(*tmpVal != 0) {
		int32_t tmpChar = *tmpVal++;
		int32_t charIndex;
		if (tmpChar >= 0x80) {
			charIndex = 0;
		} else if (tmpChar < 0x20) {
			charIndex = 0;
		} else if (tmpChar < 0x80) {
			charIndex = tmpChar - 0x1F;
		} else {
			for (int32_t iii=0x80-0x20; iii < listOfElement.Size(); iii++) {
				if (listOfElement[iii].unicodeCharVal == tmpChar) {
					charIndex = iii;
					break;
				}
			}
			// TODO : Update if possible the mapping
			charIndex = 0;
		}
		etkFloat_t sizeWidth = listOfElement[charIndex].width;
		// 0x01 == 0x20 == ' ';
		if (tmpChar != 0x01) {
			// set texture coordonates :
			coordTex.PushBack(listOfElement[charIndex].posStart);
			texCoord_ts tmpTex;
			tmpTex.u = listOfElement[charIndex].posStop.u;
			tmpTex.v = listOfElement[charIndex].posStart.v;
			coordTex.PushBack(tmpTex);
			coordTex.PushBack(listOfElement[charIndex].posStop);
			tmpTex.u = listOfElement[charIndex].posStart.u;
			tmpTex.v = listOfElement[charIndex].posStop.v;
			coordTex.PushBack(tmpTex);
			// set display positions :
			/*int32_t xxxx = posDrawX;
			int32_t yyyy = drawPosition.y;*/
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

int32_t ewol::GetWidth(int32_t fontID, const char * utf8String)
{
	if(fontID>=m_listLoadedFont.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 0;
	}
	etk::VectorType<freeTypeFontElement_ts> & listOfElement = m_listLoadedFont[fontID]->GetRefOnElement();
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
			charIndex = tmpChar - 0x1F;
		} else {
			for (int32_t iii=0x80-0x20; iii < listOfElement.Size(); iii++) {
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
	if(fontID>=m_listLoadedFont.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 10;
	}
	return m_listLoadedFont[fontID]->GetHeight();
}


#endif


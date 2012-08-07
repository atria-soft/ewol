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

#include <ewol/Font.h>
#include <ewol/Texture.h>
#include <etk/unicode.h>
#include <vector>

#include <ewol/importgl.h>
extern "C" {
	#include <freetype/ft2build.h>
}
#include FT_FREETYPE_H

#undef __class__
#define __class__	"ewol::FontFreeType"



extern "C"
{
	// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
	typedef struct {
		uniChar_t        unicodeCharVal;
		texCoord_ts      posStart;
		texCoord_ts      posStop;
		Vector2D<float>  bearing;
		Vector2D<float>  size;
		int32_t          advance;
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
	public:
		FTFontInternal(etk::File fontFileName, etk::UString fontName)
		{
			m_fontName = fontName;
			m_fileName = fontFileName;
			m_FileBuffer = NULL;
			m_FileSize = 0;
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
			if( FT_Err_Unknown_File_Format == error) {
				EWOL_ERROR("... the font file could be opened and read, but it appears ... that its font format is unsupported");
			} else if (0 != error) {
				EWOL_ERROR("... another error code means that the font file could not ... be opened or read, or simply that it is broken...");
			} else {
				// all OK
				EWOL_INFO("load font : \"" << m_fileName << "\" ");
				//Display();
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
		etk::UString GetFontName(void) {return m_fontName;};
		bool GenerateBitmapFont(int32_t size, int32_t &height, int32_t & textureId, std::vector<freeTypeFontElement_ts> & listElement)
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
			
			int32_t nbElement = listElement.size();
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
			
			// Allocate Memory For The Texture Data.
			int32_t byfferDataSize = textureWidth * textureHeight;
			GLubyte* expanded_data = new GLubyte[byfferDataSize];
			if (NULL == expanded_data) {
				EWOL_ERROR("Allocation tmp data ERROR");
				return false;
			}
			// clean the data : 
			for(int j=0; j <textureHeight;j++) {
				for(int i=0; i < textureWidth; i++){
					expanded_data[(i+j*textureWidth)] = 0;
				}
			}
			int32_t tmpRowStartPos = 0;
			int32_t tmpLineStartPos = 0;
			int32_t CurrentLineHigh = 0;
			// Generate for All Elements :
			for (int32_t iii=0; iii<listElement.size(); iii++) {
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
						int32_t position =   (tmpRowStartPos  + i )
						                   + (tmpLineStartPos + j ) * textureWidth;
						expanded_data[position] = slot->bitmap.buffer[i + tmpWidth*j];
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
			// use the texture manager to have the texture availlable every restart of the screen
			//textureId = LoadTexture(GL_TEXTURE_2D, 0, GL_ALPHA8, textureWidth, textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, expanded_data, byfferDataSize * sizeof(GLubyte), "---FreeFont---" );
			textureId = ewol::texture::Load(GL_TEXTURE_2D, 0, GL_ALPHA, textureWidth, textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, expanded_data, byfferDataSize * sizeof(GLubyte), "---FreeFont---" );
			
			EWOL_DEBUG("end load texture font");
			// With The Texture Created, We Don't Need The Expanded Data Anymore.
			delete [] expanded_data;
			EWOL_DEBUG("end load remode tmp data");
		
			return false;
		}
	private:
		etk::UString  m_fontName;
		etk::File    m_fileName;
		FT_Byte *    m_FileBuffer;
		int32_t      m_FileSize;
		FT_Face      m_fftFace;
};

static std::vector<FTFontInternal*> m_listLoadedTTFont;


static etk::UString s_currentFolderName = "";
static etk::UString s_currentDefaultFontName = "";
static int32_t s_currentDefaultFontId = -1;

class FTFont{
	public:
		FTFont(etk::File fontfileName, etk::UString fontName, int32_t size)
		{
			m_trueTypeFontId = -1;
			for (int32_t iii=0; iii < m_listLoadedTTFont.size(); iii++) {
				if (m_listLoadedTTFont[iii]->GetFontName() == fontName) {
					m_trueTypeFontId = iii;
				}
			}
			if (-1==m_trueTypeFontId) {
				// load a new one ...
				FTFontInternal * tmpElement = new FTFontInternal(fontfileName, fontName);
				m_listLoadedTTFont.push_back(tmpElement);
				m_trueTypeFontId = m_listLoadedTTFont.size() -1;
			}
			// set the bassic charset:
			m_elements.clear();
			freeTypeFontElement_ts tmpchar1;
			tmpchar1.unicodeCharVal = 0;
			m_elements.push_back(tmpchar1);
			// TODO : dynamic generation of this : expected minimum of 0x20 => 0x7F ....
			for (int32_t iii=0x20; iii<0xFF; iii++) {
				freeTypeFontElement_ts tmpchar;
				tmpchar.unicodeCharVal = iii;
				m_elements.push_back(tmpchar);
				if (0x7F == iii) {
					iii = 0x9F;
				}
			}
			m_size = size;
			m_listLoadedTTFont[m_trueTypeFontId]->GenerateBitmapFont(m_size, m_lineHeight, m_textureId, m_elements);
			EWOL_DEBUG("plop");
		}
		~FTFont(void)
		{
			
		}
		bool Check(etk::UString fontName, int32_t size)
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
		
		std::vector<freeTypeFontElement_ts> & GetRefOnElement(void)
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
		int32_t                                   m_textureId;   // internal texture ID
		int32_t                                   m_size;        // nb pixel height
		int32_t                                   m_lineHeight;  // nb pixel height
		int32_t                                   m_interline;   // nb pixel between 2 lines
		std::vector<freeTypeFontElement_ts>   m_elements;    // 
};

static std::vector<FTFont*> m_listLoadedFont;

#undef __class__
#define __class__	"ewol::FontFreeType"

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
	EWOL_DEBUG("==> Init Font-Manager");
	int32_t error = FT_Init_FreeType( &library );
	if(0 != error) {
		EWOL_CRITICAL(" when loading FreeType Librairy ...");
	}
	// prevent android error ==> can create memory leak but I prefer
	s_currentFolderName = "";
	s_currentDefaultFontName = "";
	m_listLoadedTTFont.clear();
	s_currentDefaultFontId = -1;
}

void ewol::UnInitFont(void)
{
	EWOL_DEBUG("==> Un-Init Font-Manager");
	/*int32_t error = FT_Done_FreeType( library );
	library = NULL;
	if(0 != error) {
		EWOL_CRITICAL(" when Un-loading FreeType Librairy ...");
	}
	*/
	s_currentFolderName = "";
	s_currentDefaultFontName = "";
	s_currentDefaultFontId = -1;
	
	// unload global font
	for(int32_t iii=0; iii<m_listLoadedFont.size(); iii++) {
		if (NULL != m_listLoadedFont[iii]) {
			delete(m_listLoadedFont[iii]);
		}
		m_listLoadedFont[iii] = NULL;
	}
	m_listLoadedFont.clear();
	// unload TFT font ...
	for(int32_t iii=0; iii<m_listLoadedTTFont.size(); iii++) {
		if (NULL != m_listLoadedTTFont[iii]) {
			delete(m_listLoadedTTFont[iii]);
		}
		m_listLoadedTTFont[iii] = NULL;
	}
	m_listLoadedTTFont.clear();
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
	etk::UString tmpFileName = s_currentFolderName + "/" + fontName;
	etk::File fileName(tmpFileName, etk::FILE_TYPE_DATA);
	if (false == fileName.Exist()) {
		EWOL_ERROR("Font does not exist: \"" << fileName.GetCompleateName() << "\"");
		return -1;
	}
	for (int32_t iii=0; iii < m_listLoadedFont.size(); iii++) {
		if (true == m_listLoadedFont[iii]->Check(fontName, size)) {
			return iii;
		}
	}
	FTFont * tmpFont = new FTFont(fileName, fontName, size);
	m_listLoadedFont.push_back(tmpFont);
	return m_listLoadedFont.size()-1;
}

void ewol::UnloadFont(int32_t id)
{
	EWOL_TODO("I do not think it was a good idea... will be done later");
}

int32_t ewol::DrawText(int32_t                          fontID,
                       Vector2D<float>                  textPos,
                       clipping_ts &                    drawClipping,
                       const etk::UString&              unicodeString,
                       int32_t &                        fontTextureId,
                       std::vector<Vector2D<float> > &  coord,
                       std::vector<texCoord_ts> &       coordTex)
{
	if(fontID>=m_listLoadedFont.size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 0;
	}
	std::vector<freeTypeFontElement_ts> & listOfElement = m_listLoadedFont[fontID]->GetRefOnElement();

	fontTextureId = m_listLoadedFont[fontID]->GetOglId();
	int32_t fontSize = m_listLoadedFont[fontID]->GetSize();
	int32_t fontHeight = m_listLoadedFont[fontID]->GetHeight();

	float posDrawX = textPos.x;
	
	for(int32_t iii=0; iii<unicodeString.Size(); iii++) {
		uniChar_t tmpChar = unicodeString[iii];
		int32_t charIndex;
		if (tmpChar < 0x20) {
			charIndex = 0;
		} else if (tmpChar < 0x80) {
			charIndex = tmpChar - 0x1F;
		} else {
			charIndex = 0;
			for (int32_t iii=0x80-0x20; iii < listOfElement.size(); iii++) {
				if (listOfElement[iii].unicodeCharVal == tmpChar) {
					charIndex = iii;
					break;
				}
			}
		}
		// 0x01 == 0x20 == ' ';
		if (tmpChar != 0x01) {
			/* Bitmap position
			 *      xA     xB
			 *   yC *------*
			 *      |      |
			 *      |      |
			 *   yD *------*
			 */
			float dxA = posDrawX + listOfElement[charIndex].bearing.x;
			float dxB = posDrawX + listOfElement[charIndex].bearing.x + listOfElement[charIndex].size.x;
			float dyC = textPos.y + listOfElement[charIndex].bearing.y + fontHeight - fontSize;
			float dyD = dyC - listOfElement[charIndex].size.y;
			
			float tuA = listOfElement[charIndex].posStart.u;
			float tuB = listOfElement[charIndex].posStop.u;
			float tvC = listOfElement[charIndex].posStart.v;
			float tvD = listOfElement[charIndex].posStop.v;
			
			
			// Clipping and drawing area
			// TODO : clipping in Y too ...
			if(    dxB < drawClipping.x
			    || dxA > drawClipping.x + drawClipping.w)
			{
				// Nothing to diplay ...
			} else {
				/*
				// generata positions...
				float TexSizeX = tuB - tuA;
				if (dxA < drawClipping.x) {
					// clip display
					float drawSize = drawClipping.x - dxA;
					// Update element start display
					dxA = drawClipping.x;
					float addElement = TexSizeX * drawSize / listOfElement[charIndex].size.x;
					// update texture start X Pos
					tuA += addElement;
				}
				if (dxB > drawClipping.x + drawClipping.w) {
					// clip display
					float drawSize = dxB - (drawClipping.x + drawClipping.w);
					// Update element start display
					dxB = drawClipping.x + drawClipping.w;
					float addElement = TexSizeX * drawSize / listOfElement[charIndex].size.x;
					// update texture start X Pos
					tuB -= addElement;
				}
				float TexSizeY = tvD - tvC;
				if (dyD < drawClipping.y) {
					// clip display
					float drawSize = drawClipping.y - dyD;
					// Update element start display
					dyD = drawClipping.y;
					float addElement = TexSizeY * drawSize / listOfElement[charIndex].size.y;
					// update texture start X Pos
					tvD += addElement;
				}
				if (dyC > drawClipping.y + drawClipping.h) {
					// clip display
					float drawSize = dyC - (drawClipping.y + drawClipping.h);
					// Update element start display
					dyC = drawClipping.y + drawClipping.h;
					float addElement = TexSizeX * drawSize / listOfElement[charIndex].size.y;
					// update texture start X Pos
					tvC -= addElement;
				}
				*/
				if(    dxB <= dxA
				    /*|| dyD >= dyC*/) {
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
					coordTex.push_back(texturePos[0]);
					coordTex.push_back(texturePos[1]);
					coordTex.push_back(texturePos[2]);
					// set display positions :
					coord.push_back(bitmapDrawPos[0]);
					coord.push_back(bitmapDrawPos[1]);
					coord.push_back(bitmapDrawPos[2]);
					
					/* Step 2 : 
					 *              
					 *   **         
					 *   ****       
					 *   ******     
					 *   ********   
					 */
					// set texture coordonates :
					coordTex.push_back(texturePos[0]);
					coordTex.push_back(texturePos[2]);
					coordTex.push_back(texturePos[3]);
					// set display positions :
					coord.push_back(bitmapDrawPos[0]);
					coord.push_back(bitmapDrawPos[2]);
					coord.push_back(bitmapDrawPos[3]);
				}
			}
		}
		posDrawX += listOfElement[charIndex].advance;
	}
	int32_t sizeOut = posDrawX - textPos.x;
	textPos.x = posDrawX;
	return sizeOut;
}

// TODO : Simplify this ...
int32_t ewol::DrawText(int32_t                          fontID,
                       Vector2D<float>                  textPos,
                       clipping_ts &                    drawClipping,
                       const uniChar_t                  unicodeChar,
                       int32_t &                        fontTextureId,
                       std::vector<Vector2D<float> > &  coord,
                       std::vector<texCoord_ts> &       coordTex)
{
#if 0
	if(fontID>=m_listLoadedFont.Size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 0;
	}
	std::vector<freeTypeFontElement_ts> & listOfElement = m_listLoadedFont[fontID]->GetRefOnElement();

	fontTextureId = m_listLoadedFont[fontID]->GetOglId();
	int32_t fontSize = m_listLoadedFont[fontID]->GetSize();

	float posDrawX = textPos.x;
	int32_t charIndex;
	
	if (unicodeChar < 0x20) {
		charIndex = 0;
	} else if (unicodeChar < 0x80) {
		charIndex = unicodeChar - 0x1F;
	} else {
		charIndex = 0;
		for (int32_t iii=0x80-0x20; iii < listOfElement.Size(); iii++) {
			if (listOfElement[iii].unicodeCharVal == unicodeChar) {
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
		float dxA = posDrawX + listOfElement[charIndex].bearing.x;
		float dxB = posDrawX + listOfElement[charIndex].bearing.x + listOfElement[charIndex].size.x;
		float dyC = textPos.y + listOfElement[charIndex].bearing.y - fontSize;
		float dyD = dyC + listOfElement[charIndex].size.y;
		
		float tuA = listOfElement[charIndex].posStart.u;
		float tuB = listOfElement[charIndex].posStop.u;
		float tvC = listOfElement[charIndex].posStart.v;
		float tvD = listOfElement[charIndex].posStop.v;
		
		
		// Clipping and drawing area
		// TODO : clipping in Y too ...
		if(    dxB < drawClipping.x
		    || dxA > drawClipping.x + drawClipping.w)
		{
			// Nothing to diplay ...
		} else {
			// generata positions...
			float TexSizeX = tuB - tuA;
			if (dxA < drawClipping.x) {
				// clip display
				float drawSize = drawClipping.x - dxA;
				// Update element start display
				dxA = drawClipping.x;
				float addElement = TexSizeX * drawSize / listOfElement[charIndex].size.x;
				// update texture start X Pos
				tuA += addElement;
			}
			if (dxB > drawClipping.x + drawClipping.w) {
				// clip display
				float drawSize = dxB - (drawClipping.x + drawClipping.w);
				// Update element start display
				dxB = drawClipping.x + drawClipping.w;
				float addElement = TexSizeX * drawSize / listOfElement[charIndex].size.x;
				// update texture start X Pos
				tuB -= addElement;
			}
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
	posDrawX += listOfElement[charIndex].advance;
	int32_t sizeOut = posDrawX - textPos.x;
	textPos.x = posDrawX;
	return sizeOut;
#else
	return 10;
#endif
}


int32_t ewol::DrawText(int32_t                          fontID,
                       Vector2D<float>                  textPos,
                       const etk::UString&              unicodeString,
                       int32_t &                        fontTextureId,
                       std::vector<Vector2D<float> > &  coord,
                       std::vector<texCoord_ts> &       coordTex)
{
	if(fontID>=m_listLoadedFont.size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 0;
	}
	std::vector<freeTypeFontElement_ts> & listOfElement = m_listLoadedFont[fontID]->GetRefOnElement();

	fontTextureId = m_listLoadedFont[fontID]->GetOglId();
	int32_t fontSize = m_listLoadedFont[fontID]->GetSize();
	int32_t fontHeight = m_listLoadedFont[fontID]->GetHeight();

	float posDrawX = textPos.x;
	
	for(int32_t iii=0; iii<unicodeString.Size(); iii++) {
		uniChar_t tmpChar = unicodeString[iii];
		int32_t charIndex;
		if (tmpChar < 0x20) {
			charIndex = 0;
		} else if (tmpChar < 0x80) {
			charIndex = tmpChar - 0x1F;
		} else {
			charIndex = 0;
			for (int32_t iii=0x80-0x20; iii < listOfElement.size(); iii++) {
				if (listOfElement[iii].unicodeCharVal == tmpChar) {
					charIndex = iii;
					break;
				}
			}
		}
		// 0x01 == 0x20 == ' ';
		if (tmpChar != 0x01) {
			/* Bitmap position
			 *      xA     xB
			 *   yC *------*
			 *      |      |
			 *      |      |
			 *   yD *------*
			 */
			float dxA = posDrawX + listOfElement[charIndex].bearing.x;
			float dxB = posDrawX + listOfElement[charIndex].bearing.x + listOfElement[charIndex].size.x;
			float dyC = textPos.y + listOfElement[charIndex].bearing.y + fontHeight - fontSize;
			float dyD = dyC - listOfElement[charIndex].size.y;
			
			float tuA = listOfElement[charIndex].posStart.u;
			float tuB = listOfElement[charIndex].posStop.u;
			float tvC = listOfElement[charIndex].posStart.v;
			float tvD = listOfElement[charIndex].posStop.v;
			
			
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
				coordTex.push_back(texturePos[0]);
				coordTex.push_back(texturePos[1]);
				coordTex.push_back(texturePos[2]);
				// set display positions :
				coord.push_back(bitmapDrawPos[0]);
				coord.push_back(bitmapDrawPos[1]);
				coord.push_back(bitmapDrawPos[2]);
				
				/* Step 2 : 
				 *              
				 *   **         
				 *   ****       
				 *   ******     
				 *   ********   
				 */
				// set texture coordonates :
				coordTex.push_back(texturePos[0]);
				coordTex.push_back(texturePos[2]);
				coordTex.push_back(texturePos[3]);
				// set display positions :
				coord.push_back(bitmapDrawPos[0]);
				coord.push_back(bitmapDrawPos[2]);
				coord.push_back(bitmapDrawPos[3]);
				
			}
		}
		posDrawX += listOfElement[charIndex].advance;
	}
	int32_t sizeOut = posDrawX - textPos.x;
	textPos.x = posDrawX;
	return sizeOut;
}

// TODO : Simplify this ...
int32_t ewol::DrawText(int32_t                         fontID,
                       Vector2D<float>                 textPos,
                       const uniChar_t                 unicodeChar,
                       int32_t &                       fontTextureId,
                       std::vector<Vector2D<float> > & coord,
                       std::vector<texCoord_ts> &      coordTex)
{
	if(fontID>=m_listLoadedFont.size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 0;
	}
	std::vector<freeTypeFontElement_ts> & listOfElement = m_listLoadedFont[fontID]->GetRefOnElement();

	fontTextureId = m_listLoadedFont[fontID]->GetOglId();
	int32_t fontSize = m_listLoadedFont[fontID]->GetSize();

	float posDrawX = textPos.x;
	int32_t charIndex;
	
	if (unicodeChar < 0x20) {
		charIndex = 0;
	} else if (unicodeChar < 0x80) {
		charIndex = unicodeChar - 0x1F;
	} else {
		charIndex = 0;
		for (int32_t iii=0x80-0x20; iii < listOfElement.size(); iii++) {
			if (listOfElement[iii].unicodeCharVal == unicodeChar) {
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
		float dxA = posDrawX + listOfElement[charIndex].bearing.x;
		float dxB = posDrawX + listOfElement[charIndex].bearing.x + listOfElement[charIndex].size.x;
		float dyC = textPos.y + fontSize - listOfElement[charIndex].bearing.y;
		float dyD = textPos.y + fontSize - listOfElement[charIndex].bearing.y + listOfElement[charIndex].size.y;
		
		float tuA = listOfElement[charIndex].posStart.u;
		float tuB = listOfElement[charIndex].posStop.u;
		float tvC = listOfElement[charIndex].posStart.v;
		float tvD = listOfElement[charIndex].posStop.v;
		
		if(    dxB <= dxA
		    || dyD <= dyC) {
			// nothing to do ...
		} else {
			/* Bitmap position
			 *   0------1
			 *   |      |
			 *   |      |
			 *   3------2
			 */
			Vector2D<float> bitmapDrawPos[4];
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
			coordTex.push_back(texturePos[0]);
			coordTex.push_back(texturePos[1]);
			coordTex.push_back(texturePos[2]);
			// set display positions :
			coord.push_back(bitmapDrawPos[0]);
			coord.push_back(bitmapDrawPos[1]);
			coord.push_back(bitmapDrawPos[2]);
			
			/* Step 2 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			// set texture coordonates :
			coordTex.push_back(texturePos[0]);
			coordTex.push_back(texturePos[2]);
			coordTex.push_back(texturePos[3]);
			// set display positions :
			coord.push_back(bitmapDrawPos[0]);
			coord.push_back(bitmapDrawPos[2]);
			coord.push_back(bitmapDrawPos[3]);
		}
	}
	posDrawX += listOfElement[charIndex].advance;
	int32_t sizeOut = posDrawX - textPos.x;
	textPos.x = posDrawX;
	return sizeOut;
}


int32_t ewol::GetWidth(int32_t fontID, const etk::UString& unicodeString)
{
	if(fontID>=m_listLoadedFont.size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 0;
	}
	std::vector<freeTypeFontElement_ts> & listOfElement = m_listLoadedFont[fontID]->GetRefOnElement();
	
	float posDrawX = 0.0;
	for(int32_t iii=0; iii<unicodeString.Size(); iii++) {
		uniChar_t tmpChar = unicodeString[iii];
		int32_t charIndex;
		if (tmpChar >= 0x80) {
			charIndex = 0;
		} else if (tmpChar < 0x20) {
			charIndex = 0;
		} else if (tmpChar < 0x80) {
			charIndex = tmpChar - 0x1F;
		} else {
			for (int32_t iii=0x80-0x20; iii < listOfElement.size(); iii++) {
				if (listOfElement[iii].unicodeCharVal == tmpChar) {
					charIndex = iii;
					break;
				}
			}
			// TODO : Update if possible the mapping
			charIndex = 0;
		}
		posDrawX += listOfElement[charIndex].advance;
	}
	return posDrawX;
}


int32_t ewol::GetHeight(int32_t fontID)
{
	if(fontID>=m_listLoadedFont.size() || fontID < 0) {
		EWOL_WARNING("try to display text with an fontID that does not existed " << fontID);
		return 10;
	}
	return m_listLoadedFont[fontID]->GetHeight();
}


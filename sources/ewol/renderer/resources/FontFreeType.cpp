/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/unicode.h>
#include <etk/Vector.h>
#include <etk/os/FSNode.h>


#include <ewol/renderer/openGL.h>

#include <ewol/renderer/resources/Texture.h>
#include <ewol/renderer/resources/FontFreeType.h>
#include <ewol/renderer/resources/font/FontBase.h>


#undef __class__
#define __class__	"ewol::FontFreeType"


// free Font hnadle of librairies ... entry for acces ...
static FT_Library library;

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
	int32_t error = FT_Done_FreeType( library );
	library = NULL;
	if(0 != error) {
		EWOL_CRITICAL(" when Un-loading FreeType Librairy ...");
	}
}



ewol::FontFreeType::FontFreeType(etk::UString fontName) :
	FontBase(fontName)
{
	m_init = false;
	m_FileBuffer = NULL;
	m_FileSize = 0;
	
	etk::FSNode myfile(fontName);
	if (false == myfile.Exist()) {
		EWOL_ERROR("File Does not exist : " << myfile);
		return;
	}
	m_FileSize = myfile.FileSize();
	if (0==m_FileSize) {
		EWOL_ERROR("This file is empty : " << myfile);
		return;
	}
	if (false == myfile.FileOpenRead()) {
		EWOL_ERROR("Can not open the file : " << myfile);
		return;
	}
	// allocate data
	m_FileBuffer = new FT_Byte[m_FileSize];
	if (NULL == m_FileBuffer) {
		EWOL_ERROR("Error Memory allocation size=" << fontName);
		return;
	}
	// load data from the file :
	myfile.FileRead(m_FileBuffer, 1, m_FileSize);
	// close the file:
	myfile.FileClose();
	// load Face ...
	int32_t error = FT_New_Memory_Face( library, m_FileBuffer, m_FileSize, 0, &m_fftFace );
	if( FT_Err_Unknown_File_Format == error) {
		EWOL_ERROR("... the font file could be opened and read, but it appears ... that its font format is unsupported");
	} else if (0 != error) {
		EWOL_ERROR("... another error code means that the font file could not ... be opened or read, or simply that it is broken...");
	} else {
		// all OK
		EWOL_INFO("load font : \"" << fontName << "\" ");
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
	FT_Done_Face( m_fftFace );
}


vec2 ewol::FontFreeType::GetSize(int32_t fontSize, const etk::UString & unicodeString)
{
	if(false==m_init) {
		return vec2(0,0);
	}
	// TODO : ...
	vec2 outputSize(0,0);
	return outputSize;
}

int32_t ewol::FontFreeType::GetHeight(int32_t fontSize)
{
	return fontSize*1.43f; // this is a really "magic" number ...
}

bool ewol::FontFreeType::GetGlyphProperty(int32_t              fontSize,
                                          ewol::GlyphProperty& property)
{
	if(false==m_init) {
		return false;
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int32_t fontQuality = 96;
	// Select Size ...
	// note tha <<6==*64 corespond with the 1/64th of points calculation of freetype
	int32_t error = FT_Set_Char_Size(m_fftFace, fontSize<<6, fontSize<<6, fontQuality, fontQuality);
	if (0!=error ) {
		EWOL_ERROR("FT_Set_Char_Size ==> error in settings ...");
		return false;
	}
	// a small shortcut
	FT_GlyphSlot slot = m_fftFace->glyph;
	// retrieve glyph index from character code 
	int32_t glyph_index = FT_Get_Char_Index(m_fftFace, property.m_UVal);
	// load glyph image into the slot (erase previous one)
	error = FT_Load_Glyph(m_fftFace, // handle to face object
	                      glyph_index, // glyph index
	                      FT_LOAD_DEFAULT );
	if (0!=error ) {
		EWOL_ERROR("FT_Load_Glyph specify Glyph");
		return false;
	}
	// convert to an anti-aliased bitmap
	error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL );
	if (0!=error) {
		EWOL_ERROR("FT_Render_Glyph");
		return false;
	}
	// set properties :
	property.m_glyphIndex = glyph_index;
	property.m_sizeTexture.x = slot->bitmap.width;
	property.m_sizeTexture.y = slot->bitmap.rows;
	property.m_bearing.x = slot->metrics.horiBearingX>>6;
	property.m_bearing.y = slot->metrics.horiBearingY>>6;
	property.m_advance.x = slot->metrics.horiAdvance>>6;
	property.m_advance.y = slot->metrics.vertAdvance>>6;
	
	return true;
}

bool ewol::FontFreeType::DrawGlyph(draw::Image&           imageOut,
                                   int32_t                fontSize,
                                   ivec2 glyphPosition,
                                   ewol::GlyphProperty&   property,
                                   int8_t                 posInImage)
{

	if(false==m_init) {
		return false;
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int32_t fontQuality = 96;
	// Select Size ...
	// note tha <<6==*64 corespond with the 1/64th of points calculation of freetype
	int32_t error = FT_Set_Char_Size(m_fftFace, fontSize<<6, fontSize<<6, fontQuality, fontQuality);
	if (0!=error ) {
		EWOL_ERROR("FT_Set_Char_Size ==> error in settings ...");
		return false;
	}
	// a small shortcut
	FT_GlyphSlot slot = m_fftFace->glyph;
	// load glyph image into the slot (erase previous one)
	error = FT_Load_Glyph(m_fftFace, // handle to face object
	                      property.m_glyphIndex, // glyph index
	                      FT_LOAD_DEFAULT );
	if (0!=error ) {
		EWOL_ERROR("FT_Load_Glyph specify Glyph");
		return false;
	}
	// convert to an anti-aliased bitmap
	error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL );
	if (0!=error) {
		EWOL_ERROR("FT_Render_Glyph");
		return false;
	}
	// draw it on the output Image :
	draw::Color tlpppp(0xFF,0xFF,0xFF,0x00);
	for(int32_t jjj=0; jjj < slot->bitmap.rows;jjj++) {
		for(int32_t iii=0; iii < slot->bitmap.width; iii++){
			tlpppp = imageOut.Get(ivec2(glyphPosition.x+iii, glyphPosition.y+jjj));
			uint8_t valueColor = slot->bitmap.buffer[iii + slot->bitmap.width*jjj];
			// set only alpha :
			switch(posInImage)
			{
				default:
				case 0:
					tlpppp.a = valueColor;
					break;
				case 1:
					tlpppp.r = valueColor;
					break;
				case 2:
					tlpppp.g = valueColor;
					break;
				case 3:
					tlpppp.b = valueColor;
					break;
			}
			// real set of color
			imageOut.Set(ivec2(glyphPosition.x+iii, glyphPosition.y+jjj), tlpppp );
		}
	}
	return true;
}


void ewol::FontFreeType::GenerateKerning(int32_t fontSize, etk::Vector<ewol::GlyphProperty>& listGlyph)
{
	if(false==m_init) {
		return;
	}
	if ((FT_FACE_FLAG_KERNING & m_fftFace->face_flags) == 0) {
		EWOL_INFO("No kerning generation (Disable) in the font");
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int32_t fontQuality = 96;
	// Select Size ...
	// note tha <<6==*64 corespond with the 1/64th of points calculation of freetype
	int32_t error = FT_Set_Char_Size(m_fftFace, fontSize<<6, fontSize<<6, fontQuality, fontQuality);
	if (0!=error ) {
		EWOL_ERROR("FT_Set_Char_Size ==> error in settings ...");
		return;
	}
	// For all the kerning element we get the kerning value :
	for(int32_t iii=0; iii<listGlyph.Size(); iii++) {
		listGlyph[iii].KerningClear();
		for(int32_t kkk=0; kkk<listGlyph.Size(); kkk++) {
			FT_Vector kerning;
			FT_Get_Kerning(m_fftFace, listGlyph[kkk].m_glyphIndex, listGlyph[iii].m_glyphIndex, FT_KERNING_UNFITTED, &kerning );
			// add the kerning only if != 0 ... 
			if (kerning.x != 0) {
				listGlyph[iii].KerningAdd(listGlyph[kkk].m_UVal,
				                          kerning.x/32.0f );
				//EWOL_DEBUG("Kerning between : '" << (char)listGlyph[iii].m_UVal << "'&'" << (char)listGlyph[kkk].m_UVal << "' value : " << kerning.x << " => " << (kerning.x/64.0f));
			}
		}
	}
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




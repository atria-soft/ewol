/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <etk/Vector.hpp>


#include <gale/renderer/openGL/openGL.hpp>

#include <ewol/resource/Texture.hpp>
#include <ewol/resource/FontFreeType.hpp>
#include <ewol/resource/font/FontBase.hpp>
#include <gale/resource/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::resource::FontFreeType);

// free Font hnadle of librairies ... entry for acces ...
static int32_t l_countLoaded=0;
static FT_Library library;

void ewol::resource::freeTypeInit() {
	EWOL_DEBUG(" == > init Font-Manager");
	l_countLoaded++;
	if (l_countLoaded>1) {
		// already loaded ...
		return;
	}
	int32_t error = FT_Init_FreeType( &library );
	if(0 != error) {
		EWOL_CRITICAL(" when loading FreeType Librairy ...");
	}
}

void ewol::resource::freeTypeUnInit() {
	EWOL_DEBUG(" == > Un-Init Font-Manager");
	l_countLoaded--;
	if (l_countLoaded>0) {
		// already needed ...
		return;
	}
	int32_t error = FT_Done_FreeType( library );
	library = null;
	if(0 != error) {
		EWOL_CRITICAL(" when Un-loading FreeType Librairy ...");
	}
}

ewol::resource::FontFreeType::FontFreeType() {
	addResourceType("ewol::FontFreeType");
	m_init = false;
	m_FileSize = 0;
}

void ewol::resource::FontFreeType::init(const etk::Uri& _uri) {
	ethread::RecursiveLock lock(m_mutex);
	ewol::resource::FontBase::init(_uri);
	auto fileIO = etk::uri::get(_uri);
	if (fileIO == null) {
		EWOL_ERROR("File Does not exist : " << _uri);
		return;
	}
	if (fileIO->open(etk::io::OpenMode::Read) == false) {
		EWOL_ERROR("Can not open the file : " << _uri);
		return;
	}
	m_FileBuffer = fileIO->readAll<FT_Byte>();
	// close the file:
	fileIO->close();
	// load Face ...
	int32_t error = FT_New_Memory_Face(library, &m_FileBuffer[0], m_FileBuffer.size(), 0, &m_fftFace );
	if( FT_Err_Unknown_File_Format == error) {
		EWOL_ERROR("... the font file could be opened and read, but it appears ... that its font format is unsupported");
	} else if (0 != error) {
		EWOL_ERROR("... another error code means that the font file could not ... be opened or read, or simply that it is broken...");
	} else {
		// all OK
		EWOL_DEBUG("load font : \"" << _uri << "\" glyph count = " << (int)m_fftFace->num_glyphs);
		m_init = true;
		//display();
	}
}

ewol::resource::FontFreeType::~FontFreeType() {
	ethread::RecursiveLock lock(m_mutex);
	// clean the tmp memory
	m_FileBuffer.clear();
	// must be deleted fftFace
	FT_Done_Face(m_fftFace);
}

vec2 ewol::resource::FontFreeType::getSize(int32_t _fontSize, const etk::String& _unicodeString) {
	ethread::RecursiveLock lock(m_mutex);
	if (m_init == false) {
		return vec2(0,0);
	}
	// TODO : ...
	vec2 outputSize(0,0);
	return outputSize;
}

int32_t ewol::resource::FontFreeType::getHeight(int32_t _fontSize) {
	ethread::RecursiveLock lock(m_mutex);
	return _fontSize*1.43f; // this is a really "magic" number ...
}
float ewol::resource::FontFreeType::getSizeWithHeight(float _fontHeight) {
	ethread::RecursiveLock lock(m_mutex);
	return _fontHeight*0.6993f; // this is a really "magic" number ...
}

bool ewol::resource::FontFreeType::getGlyphProperty(int32_t _fontSize, ewol::GlyphProperty& _property) {
	ethread::RecursiveLock lock(m_mutex);
	if(false == m_init) {
		return false;
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int32_t fontQuality = 96;
	// Select size ...
	// note tha <<6 == *64 corespond with the 1/64th of points calculation of freetype
	int32_t error = FT_Set_Char_Size(m_fftFace, _fontSize<<6, _fontSize<<6, fontQuality, fontQuality);
	if (0!=error ) {
		EWOL_ERROR("FT_Set_Char_Size  == > error in settings ...");
		return false;
	}
	// a small shortcut
	FT_GlyphSlot slot = m_fftFace->glyph;
	// retrieve glyph index from character code 
	int32_t glyph_index = FT_Get_Char_Index(m_fftFace, _property.m_UVal);
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
	_property.m_glyphIndex = glyph_index;
	_property.m_sizeTexture.setValue(slot->bitmap.width, slot->bitmap.rows);
	_property.m_bearing.setValue( slot->metrics.horiBearingX>>6 , slot->metrics.horiBearingY>>6 );
	_property.m_advance.setValue( slot->metrics.horiAdvance>>6 , slot->metrics.vertAdvance>>6 );
	
	return true;
}

bool ewol::resource::FontFreeType::drawGlyph(egami::Image& _imageOut,
                                             int32_t _fontSize,
                                             ivec2 _glyphPosition,
                                             ewol::GlyphProperty& _property,
                                             int8_t _posInImage) {
	ethread::RecursiveLock lock(m_mutex);
	if(m_init == false) {
		return false;
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int32_t fontQuality = 96;
	// Select size ...
	// note tha <<6 == *64 corespond with the 1/64th of points calculation of freetype
	int32_t error = FT_Set_Char_Size(m_fftFace, _fontSize<<6, _fontSize<<6, fontQuality, fontQuality);
	if (0!=error ) {
		EWOL_ERROR("FT_Set_Char_Size  == > error in settings ...");
		return false;
	}
	// a small shortcut
	FT_GlyphSlot slot = m_fftFace->glyph;
	// load glyph image into the slot (erase previous one)
	error = FT_Load_Glyph(m_fftFace, // handle to face object
	                      _property.m_glyphIndex, // glyph index
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
	etk::Color<> tlpppp(0xFF, 0xFF, 0xFF, 0x00);
	for(size_t jjj=0; jjj < slot->bitmap.rows;jjj++) {
		for(size_t iii=0; iii < slot->bitmap.width; iii++){
			tlpppp = _imageOut.get(ivec2(_glyphPosition.x()+iii, _glyphPosition.y()+jjj));
			uint8_t valueColor = slot->bitmap.buffer[iii + slot->bitmap.width*jjj];
			// set only alpha :
			switch(_posInImage) {
				default:
				case 0:
					tlpppp.setA(valueColor);
					break;
				case 1:
					tlpppp.setR(valueColor);
					break;
				case 2:
					tlpppp.setG(valueColor);
					break;
				case 3:
					tlpppp.setB(valueColor);
					break;
			}
			// real set of color
			_imageOut.set(ivec2(_glyphPosition.x()+iii, _glyphPosition.y()+jjj), tlpppp );
		}
	}
	return true;
}

bool ewol::resource::FontFreeType::drawGlyph(egami::ImageMono& _imageOut,
                                             int32_t _fontSize,
                                             ewol::GlyphProperty& _property,
                                             int32_t _borderSize) {
	ethread::RecursiveLock lock(m_mutex);
	if(false == m_init) {
		return false;
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int32_t fontQuality = 96;
	// Select size ...
	// note tha <<6 == *64 corespond with the 1/64th of points calculation of freetype
	int32_t error = FT_Set_Char_Size(m_fftFace, _fontSize<<6, _fontSize<<6, fontQuality, fontQuality);
	if (0!=error ) {
		EWOL_ERROR("FT_Set_Char_Size  == > error in settings ...");
		return false;
	}
	// a small shortcut
	FT_GlyphSlot slot = m_fftFace->glyph;
	// load glyph image into the slot (erase previous one)
	error = FT_Load_Glyph(m_fftFace, // handle to face object
	                      _property.m_glyphIndex, // glyph index
	                      FT_LOAD_DEFAULT );
	if (0!=error ) {
		EWOL_ERROR("FT_Load_Glyph specify Glyph");
		return false;
	}
	// convert to an anti-aliased bitmap
	error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL ); // TODO : set FT_RENDER_MODE_MONO ==> 1 bit value ==> faster generation ...
	if (0!=error) {
		EWOL_ERROR("FT_Render_Glyph");
		return false;
	}
	// resize output image :
	_imageOut.resize(ivec2(slot->bitmap.width+2*_borderSize, slot->bitmap.rows+2*_borderSize), 0);
	
	for(size_t jjj=0; jjj < slot->bitmap.rows;jjj++) {
		for(size_t iii=0; iii < slot->bitmap.width; iii++){
			uint8_t valueColor = slot->bitmap.buffer[iii + slot->bitmap.width*jjj];
			// real set of color
			_imageOut.set(ivec2(_borderSize+iii, _borderSize+jjj), valueColor );
		}
	}
	return true;
}


void ewol::resource::FontFreeType::generateKerning(int32_t fontSize, etk::Vector<ewol::GlyphProperty>& listGlyph) {
	ethread::RecursiveLock lock(m_mutex);
	if(m_init == false) {
		return;
	}
	if ((FT_FACE_FLAG_KERNING & m_fftFace->face_flags) == 0) {
		EWOL_INFO("No kerning generation (disable) in the font");
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int32_t fontQuality = 96;
	// Select size ...
	// note tha <<6 == *64 corespond with the 1/64th of points calculation of freetype
	int32_t error = FT_Set_Char_Size(m_fftFace, fontSize<<6, fontSize<<6, fontQuality, fontQuality);
	if (0!=error ) {
		EWOL_ERROR("FT_Set_Char_Size  == > error in settings ...");
		return;
	}
	// For all the kerning element we get the kerning value :
	for(size_t iii=0; iii<listGlyph.size(); iii++) {
		listGlyph[iii].kerningClear();
		for(size_t kkk=0; kkk<listGlyph.size(); kkk++) {
			FT_Vector kerning;
			FT_Get_Kerning(m_fftFace, listGlyph[kkk].m_glyphIndex, listGlyph[iii].m_glyphIndex, FT_KERNING_UNFITTED, &kerning );
			// add the kerning only if != 0 ... 
			if (kerning.x != 0) {
				listGlyph[iii].kerningAdd(listGlyph[kkk].m_UVal,
				                          kerning.x/32.0f );
				//EWOL_DEBUG("Kerning between : '" << (char)listGlyph[iii].m_UVal << "'&'" << (char)listGlyph[kkk].m_UVal << "' value : " << kerning.x << " => " << (kerning.x/64.0f));
			}
		}
	}
}


void ewol::resource::FontFreeType::display() {
	ethread::RecursiveLock lock(m_mutex);
	if(m_init == false) {
		return;
	}
	EWOL_INFO("    number of glyph       = " << (int)m_fftFace->num_glyphs);
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

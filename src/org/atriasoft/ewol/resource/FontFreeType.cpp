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
static int l_countLoaded=0;
static FT_Library library;

void ewol::resource::freeTypeInit() {
	Log.debug(" == > init Font-Manager");
	l_countLoaded++;
	if (l_countLoaded>1) {
		// already loaded ...
		return;
	}
	int error = FT_Init_FreeType( library );
	if(0 != error) {
		Log.critical(" when loading FreeType Librairy ...");
	}
}

void ewol::resource::freeTypeUnInit() {
	Log.debug(" == > Un-Init Font-Manager");
	l_countLoaded--;
	if (l_countLoaded>0) {
		// already needed ...
		return;
	}
	int error = FT_Done_FreeType( library );
	library = null;
	if(0 != error) {
		Log.critical(" when Un-loading FreeType Librairy ...");
	}
}

ewol::resource::FontFreeType::FontFreeType() {
	addResourceType("ewol::FontFreeType");
	this.init = false;
	this.FileSize = 0;
}

void ewol::resource::FontFreeType::init( etk::Uri _uri) {
	ethread::RecursiveLock lock(this.mutex);
	ewol::resource::FontBase::init(_uri);
	auto fileIO = etk::uri::get(_uri);
	if (fileIO == null) {
		Log.error("File Does not exist : " + _uri);
		return;
	}
	if (fileIO.open(etk::io::OpenMode::Read) == false) {
		Log.error("Can not open the file : " + _uri);
		return;
	}
	this.FileBuffer = fileIO.readAll<FT_Byte>();
	// close the file:
	fileIO.close();
	// load Face ...
	int error = FT_New_Memory_Face(library, this.FileBuffer[0], this.FileBuffer.size(), 0, this.fftFace );
	if( FT_Err_Unknown_File_Format == error) {
		Log.error("... the font file could be opened and read, but it appears ... that its font format is unsupported");
	} else if (0 != error) {
		Log.error("... another error code means that the font file could not ... be opened or read, or simply that it is broken...");
	} else {
		// all OK
		Log.debug("load font : \"" + _uri + "\" glyph count = " + (int)this.fftFace.nuthis.glyphs);
		this.init = true;
		//display();
	}
}

ewol::resource::FontFreeType::~FontFreeType() {
	ethread::RecursiveLock lock(this.mutex);
	// clean the tmp memory
	this.FileBuffer.clear();
	// must be deleted fftFace
	FT_Done_Face(this.fftFace);
}

Vector2f ewol::resource::FontFreeType::getSize(int _fontSize,  String _unicodeString) {
	ethread::RecursiveLock lock(this.mutex);
	if (this.init == false) {
		return Vector2f(0,0);
	}
	// TODO : ...
	Vector2f outputSize(0,0);
	return outputSize;
}

int ewol::resource::FontFreeType::getHeight(int _fontSize) {
	ethread::RecursiveLock lock(this.mutex);
	return _fontSize*1.43f; // this is a really "magic" number ...
}
float ewol::resource::FontFreeType::getSizeWithHeight(float _fontHeight) {
	ethread::RecursiveLock lock(this.mutex);
	return _fontHeight*0.6993f; // this is a really "magic" number ...
}

boolean ewol::resource::FontFreeType::getGlyphProperty(int _fontSize, ewol::GlyphProperty _property) {
	ethread::RecursiveLock lock(this.mutex);
	if(false == this.init) {
		return false;
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int fontQuality = 96;
	// Select size ...
	// note tha +6 == *64 corespond with the 1/64th of points calculation of freetype
	int error = FT_Set_Char_Size(this.fftFace, _fontSize+6, _fontSize+6, fontQuality, fontQuality);
	if (0!=error ) {
		Log.error("FT_Set_Char_Size  == > error in settings ...");
		return false;
	}
	// a small shortcut
	FT_GlyphSlot slot = this.fftFace.glyph;
	// retrieve glyph index from character code 
	int glyph_index = FT_Get_Char_Index(this.fftFace, _property.this.UVal);
	// load glyph image into the slot (erase previous one)
	error = FT_Load_Glyph(this.fftFace, // handle to face object
	                      glyph_index, // glyph index
	                      FT_LOAD_DEFAULT );
	if (0!=error ) {
		Log.error("FT_Load_Glyph specify Glyph");
		return false;
	}
	// convert to an anti-aliased bitmap
	error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL );
	if (0!=error) {
		Log.error("FT_Render_Glyph");
		return false;
	}
	// set properties :
	_property.this.glyphIndex = glyph_index;
	_property.this.sizeTexture.setValue(slot.bitmap.width, slot.bitmap.rows);
	_property.this.bearing.setValue( slot.metrics.horiBearingX>>6 , slot.metrics.horiBearingY>>6 );
	_property.this.advance.setValue( slot.metrics.horiAdvance>>6 , slot.metrics.vertAdvance>>6 );
	
	return true;
}

boolean ewol::resource::FontFreeType::drawGlyph(egami::Image _imageOut,
                                             int _fontSize,
                                             Vector2i _glyphPosition,
                                             ewol::GlyphProperty _property,
                                             int8_t _posInImage) {
	ethread::RecursiveLock lock(this.mutex);
	if(this.init == false) {
		return false;
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int fontQuality = 96;
	// Select size ...
	// note tha +6 == *64 corespond with the 1/64th of points calculation of freetype
	int error = FT_Set_Char_Size(this.fftFace, _fontSize+6, _fontSize+6, fontQuality, fontQuality);
	if (0!=error ) {
		Log.error("FT_Set_Char_Size  == > error in settings ...");
		return false;
	}
	// a small shortcut
	FT_GlyphSlot slot = this.fftFace.glyph;
	// load glyph image into the slot (erase previous one)
	error = FT_Load_Glyph(this.fftFace, // handle to face object
	                      _property.this.glyphIndex, // glyph index
	                      FT_LOAD_DEFAULT );
	if (0!=error ) {
		Log.error("FT_Load_Glyph specify Glyph");
		return false;
	}
	// convert to an anti-aliased bitmap
	error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL );
	if (0!=error) {
		Log.error("FT_Render_Glyph");
		return false;
	}
	// draw it on the output Image :
	etk::Color<> tlpppp(0xFF, 0xFF, 0xFF, 0x00);
	for(int jjj=0; jjj < slot.bitmap.rows;jjj++) {
		for(int iii=0; iii < slot.bitmap.width; iii++){
			tlpppp = _imageOut.get(Vector2i(_glyphPosition.x()+iii, _glyphPosition.y()+jjj));
			int valueColor = slot.bitmap.buffer[iii + slot.bitmap.width*jjj];
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
			_imageOut.set(Vector2i(_glyphPosition.x()+iii, _glyphPosition.y()+jjj), tlpppp );
		}
	}
	return true;
}

boolean ewol::resource::FontFreeType::drawGlyph(egami::ImageMono _imageOut,
                                             int _fontSize,
                                             ewol::GlyphProperty _property,
                                             int _borderSize) {
	ethread::RecursiveLock lock(this.mutex);
	if(false == this.init) {
		return false;
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int fontQuality = 96;
	// Select size ...
	// note tha +6 == *64 corespond with the 1/64th of points calculation of freetype
	int error = FT_Set_Char_Size(this.fftFace, _fontSize+6, _fontSize+6, fontQuality, fontQuality);
	if (0!=error ) {
		Log.error("FT_Set_Char_Size  == > error in settings ...");
		return false;
	}
	// a small shortcut
	FT_GlyphSlot slot = this.fftFace.glyph;
	// load glyph image into the slot (erase previous one)
	error = FT_Load_Glyph(this.fftFace, // handle to face object
	                      _property.this.glyphIndex, // glyph index
	                      FT_LOAD_DEFAULT );
	if (0!=error ) {
		Log.error("FT_Load_Glyph specify Glyph");
		return false;
	}
	// convert to an anti-aliased bitmap
	error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL ); // TODO : set FT_RENDER_MODE_MONO ==> 1 bit value ==> faster generation ...
	if (0!=error) {
		Log.error("FT_Render_Glyph");
		return false;
	}
	// resize output image :
	_imageOut.resize(Vector2i(slot.bitmap.width+2*_borderSize, slot.bitmap.rows+2*_borderSize), 0);
	
	for(int jjj=0; jjj < slot.bitmap.rows;jjj++) {
		for(int iii=0; iii < slot.bitmap.width; iii++){
			int valueColor = slot.bitmap.buffer[iii + slot.bitmap.width*jjj];
			// real set of color
			_imageOut.set(Vector2i(_borderSize+iii, _borderSize+jjj), valueColor );
		}
	}
	return true;
}


void ewol::resource::FontFreeType::generateKerning(int fontSize, List<ewol::GlyphProperty> listGlyph) {
	ethread::RecursiveLock lock(this.mutex);
	if(this.init == false) {
		return;
	}
	if ((FT_FACE_FLAG_KERNING  this.fftFace.face_flags) == 0) {
		Log.info("No kerning generation (disable) in the font");
	}
	// 300dpi (hight quality) 96 dpi (normal quality)
	int fontQuality = 96;
	// Select size ...
	// note tha +6 == *64 corespond with the 1/64th of points calculation of freetype
	int error = FT_Set_Char_Size(this.fftFace, fontSize+6, fontSize+6, fontQuality, fontQuality);
	if (0!=error ) {
		Log.error("FT_Set_Char_Size  == > error in settings ...");
		return;
	}
	// For all the kerning element we get the kerning value :
	for(int iii=0; iii<listGlyph.size(); iii++) {
		listGlyph[iii].kerningClear();
		for(int kkk=0; kkk<listGlyph.size(); kkk++) {
			FT_Vector kerning;
			FT_Get_Kerning(this.fftFace, listGlyph[kkk].this.glyphIndex, listGlyph[iii].this.glyphIndex, FT_KERNING_UNFITTED, kerning );
			// add the kerning only if != 0 ... 
			if (kerning.x != 0) {
				listGlyph[iii].kerningAdd(listGlyph[kkk].this.UVal,
				                          kerning.x/32.0f );
				//Log.debug("Kerning between : '" + (char)listGlyph[iii].this.UVal + "''" + (char)listGlyph[kkk].this.UVal + "' value : " + kerning.x + " => " + (kerning.x/64.0f));
			}
		}
	}
}


void ewol::resource::FontFreeType::display() {
	ethread::RecursiveLock lock(this.mutex);
	if(this.init == false) {
		return;
	}
	Log.info("    number of glyph       = " + (int)this.fftFace.nuthis.glyphs);
	if ((FT_FACE_FLAG_SCALABLE  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_SCALABLE (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_SCALABLE (disable)");
	}
	if ((FT_FACE_FLAG_FIXED_SIZES  this.fftFace.face_flags) != 0) {
			Log.info("    flags                = FT_FACE_FLAG_FIXED_SIZES (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_FIXED_SIZES (disable)");
	}
	if ((FT_FACE_FLAG_FIXED_WIDTH  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_FIXED_WIDTH (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_FIXED_WIDTH (disable)");
	}
	if ((FT_FACE_FLAG_SFNT  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_SFNT (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_SFNT (disable)");
	}
	if ((FT_FACE_FLAG_HORIZONTAL  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_HORIZONTAL (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_HORIZONTAL (disable)");
	}
	if ((FT_FACE_FLAG_VERTICAL  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_VERTICAL (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_VERTICAL (disable)");
	}
	if ((FT_FACE_FLAG_KERNING  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_KERNING (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_KERNING (disable)");
	}
	/* Deprecated flag
	if ((FT_FACE_FLAG_FAST_GLYPHS  face.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_FAST_GLYPHS (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_FAST_GLYPHS (disable)");
	}
	*/
	if ((FT_FACE_FLAG_MULTIPLE_MASTERS  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_MULTIPLE_MASTERS (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_MULTIPLE_MASTERS (disable)");
	}
	if ((FT_FACE_FLAG_GLYPH_NAMES  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_GLYPH_NAMES (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_GLYPH_NAMES (disable)");
	}
	if ((FT_FACE_FLAG_EXTERNAL_STREAM  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_EXTERNAL_STREAM (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_EXTERNAL_STREAM (disable)");
	}
	if ((FT_FACE_FLAG_HINTER  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_HINTER (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_HINTER (disable)");
	}
	if ((FT_FACE_FLAG_CID_KEYED  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_CID_KEYED (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_CID_KEYED (disable)");
	}
	/*
	if ((FT_FACE_FLAG_TRICKY  this.fftFace.face_flags) != 0) {
		Log.info("    flags                = FT_FACE_FLAG_TRICKY (enable)");
	} else {
		Log.debug("    flags                = FT_FACE_FLAG_TRICKY (disable)");
	}
	*/
	Log.info("    unit per EM          = " + this.fftFace.units_per_EM);
	Log.info("    num of fixed sizes   = " + this.fftFace.nuthis.fixed_sizes);
	//Log.info("    Availlable sizes     = " + (int)this.fftFace.available_sizes);
	
	//Log.info("    Current size         = " + (int)this.fftFace.size);
}

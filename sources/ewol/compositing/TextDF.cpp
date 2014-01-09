/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/compositing/TextDF.h>
#include <ewol/context/Context.h>
#include <etk/types.h>

#undef __class__
#define __class__	"ewol::compositing::TextDF"


ewol::compositing::TextDF::TextDF(const std::string& _fontName, int32_t _fontSize) :
  ewol::compositing::Text::Text(_fontName, _fontSize, false),
  m_fontDF(NULL),
  m_size(9.0) {
	setFont(_fontName, _fontSize);
	loadProgram("DATA:fontDistanceField/font1.prog");
}


ewol::compositing::TextDF::~TextDF(void) {
	ewol::resource::DistanceFieldFont::release(m_fontDF);
}

void ewol::compositing::TextDF::drawMT(const mat4& _transformationMatrix, bool _enableDepthTest) {
	
	// draw BG in any case:
	m_vectorialDraw.draw();
	
	EWOL_WARNING("draw DF...");
	if (m_coord.size() <= 0 || m_fontDF == NULL) {
		// TODO : a remÃštre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_fontDF == NULL) {
		EWOL_WARNING("no font...");
		return;
	}
	if (m_GLprogram == NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (_enableDepthTest == true) {
		ewol::openGL::enable(ewol::openGL::FLAG_DEPTH_TEST);
	}
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::getMatrix();
	mat4 camMatrix = ewol::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	m_GLprogram->use(); 
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->setTexture0(m_GLtexID, m_fontDF->getId());
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
	// Texture :
	m_GLprogram->sendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
	// color :
	m_GLprogram->sendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	ewol::openGL::drawArrays(GL_TRIANGLES, 0, m_coord.size());
	m_GLprogram->unUse();
	if (_enableDepthTest == true) {
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
}


void ewol::compositing::TextDF::drawD(bool _disableDepthTest) {
	EWOL_WARNING("draw DF.2.");
	// draw BG in any case:
	m_vectorialDraw.draw();
	
	if (m_coord.size() <= 0 || m_fontDF == NULL) {
		// TODO : a remètre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_fontDF == NULL) {
		EWOL_WARNING("no font...");
		return;
	}
	if (m_GLprogram == NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = ewol::openGL::getMatrix()*m_matrixApply;
	m_GLprogram->use(); 
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->setTexture0(m_GLtexID, m_fontDF->getId());
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
	// Texture :
	m_GLprogram->sendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
	// color :
	m_GLprogram->sendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	ewol::openGL::drawArrays(GL_TRIANGLES, 0, m_coord.size());
	m_GLprogram->unUse();
}

float ewol::compositing::TextDF::getHeight(void) {
	if (m_fontDF == NULL) {
		EWOL_WARNING("no font...");
		return 1;
	}
	return m_fontDF->getHeight(m_size);
}

ewol::GlyphProperty * ewol::compositing::TextDF::getGlyphPointer(char32_t _charcode) {
	if (m_fontDF == NULL) {
		EWOL_WARNING("no font...");
		return NULL;
	}
	return m_fontDF->getGlyphPointer(_charcode);
}

void ewol::compositing::TextDF::setFontSize(int32_t _fontSize) {
	clear();
	EWOL_VERBOSE("Set font Size: " << _fontSize);
	if (_fontSize <= 1) {
		m_size = ewol::getContext().getFontDefault().getSize();
	} else {
		m_size = _fontSize;
	}
}

void ewol::compositing::TextDF::setFontName(const std::string& _fontName) {
	clear();
	// remove old one
	ewol::resource::DistanceFieldFont* previousFont = m_fontDF;
	std::string fontName;
	if (_fontName == "") {
		fontName = ewol::getContext().getFontDefault().getName();
	} else {
		fontName = _fontName;
	}
	EWOL_VERBOSE("Set font name: '" << fontName << "'");
	// link to new one
	m_fontDF = ewol::resource::DistanceFieldFont::keep(fontName);
	if (m_fontDF == NULL) {
		EWOL_ERROR("Can not get find resource");
		m_fontDF = previousFont;
	} else {
		ewol::resource::DistanceFieldFont::release(previousFont);
	}
}

void ewol::compositing::TextDF::setFont(std::string _fontName, int32_t _fontSize) {
	setFontSize(_fontSize);
	setFontName(_fontName);
}

void ewol::compositing::TextDF::setFontMode(enum ewol::font::mode _mode) {
	m_mode = _mode;
}

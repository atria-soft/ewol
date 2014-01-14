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
  ewol::compositing::TextBase("DATA:fontDistanceField/font1.prog"),
  m_fontDF(NULL),
  m_size(12.0) {
	setFont(_fontName, _fontSize);
	// TODO : Reset size .... when reset ...
}


ewol::compositing::TextDF::~TextDF(void) {
	ewol::resource::DistanceFieldFont::release(m_fontDF);
}

void ewol::compositing::TextDF::drawMT(const mat4& _transformationMatrix, bool _enableDepthTest) {
	
	// draw BG in any case:
	m_vectorialDraw.draw();
	
	EWOL_WARNING("draw DF...");
	if (m_coord.size() <= 0 || m_fontDF == NULL) {
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
	m_GLprogram->sendAttribute(m_GLPosition, m_coord);
	// Texture :
	m_GLprogram->sendAttribute(m_GLtexture, m_coordTex);
	// color :
	m_GLprogram->sendAttribute(m_GLColor, m_coordColor);
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
	m_GLprogram->sendAttribute(m_GLPosition, m_coord);
	// Texture :
	m_GLprogram->sendAttribute(m_GLtexture, m_coordTex);
	// color :
	m_GLprogram->sendAttribute(m_GLColor, m_coordColor);
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


void ewol::compositing::TextDF::printChar(const char32_t& _charcode) {
	// get a pointer on the glyph property : 
	ewol::GlyphProperty* myGlyph = getGlyphPointer(_charcode);
	if (NULL == myGlyph) {
		EWOL_ERROR(" font does not really existed ...");
		return;
	}
	float fontSize = getSize();
	float fontHeigh = getHeight();
	
	float factorDisplay = m_fontDF->getDisplayRatio(fontSize);
	
	// get the kerning ofset :
	float kerningOffset = 0;
	if (true == m_kerning) {
		kerningOffset = myGlyph->kerningGet(m_previousCharcode);
		if (kerningOffset != 0) {
			//EWOL_DEBUG("Kerning between : '" << m_previousCharcode << "'&'" << myGlyph->m_UVal << "' value : " << kerningOffset);
		}
	}
	// 0x01 == 0x20 == ' ';
	if (_charcode != 0x01) {
		/* Bitmap position
		 *      xA     xB
		 *   yC *------*
		 *      |      |
		 *      |      |
		 *   yD *------*
		 */
		#if 0
		float dxA = m_position.x() + (myGlyph->m_bearing.x() + kerningOffset) * factorDisplay;
		float dxB = dxA + myGlyph->m_sizeTexture.x() * factorDisplay;
		float dyC = m_position.y() + (myGlyph->m_bearing.y() + fontHeigh - fontSize) * factorDisplay;
		float dyD = dyC - myGlyph->m_sizeTexture.y() * factorDisplay;
		#else
		float dxA = m_position.x() + (myGlyph->m_bearing.x() + kerningOffset - (float)m_fontDF->getPixelBorderSize()*0.5f) * factorDisplay;
		float dxB = dxA + (myGlyph->m_sizeTexture.x() + (float)m_fontDF->getPixelBorderSize()) * factorDisplay;
		float dyC = m_position.y() + (myGlyph->m_bearing.y() + fontHeigh - fontSize + (float)m_fontDF->getPixelBorderSize()*0.5f) * factorDisplay;
		float dyD = dyC - (myGlyph->m_sizeTexture.y() + (float)m_fontDF->getPixelBorderSize()) * factorDisplay;
		#endif
		
		float tuA = myGlyph->m_texturePosStart.x();
		float tuB = tuA + myGlyph->m_texturePosSize.x();
		float tvC = myGlyph->m_texturePosStart.y();
		float tvD = tvC + myGlyph->m_texturePosSize.y();
		
		
		// Clipping and drawing area
		if(    m_clippingEnable == true
		    && (    dxB < m_clippingPosStart.x()
		         || dxA > m_clippingPosStop.x()
		         || dyC < m_clippingPosStart.y()
		         || dyD > m_clippingPosStop.y() ) ) {
			// Nothing to diplay ...
		} else {
			if (m_clippingEnable == true) {
				// generata positions...
				float TexSizeX = tuB - tuA;
				if (dxA < m_clippingPosStart.x()) {
					// clip display
					float drawSize = m_clippingPosStart.x() - dxA;
					// update element start display
					dxA = m_clippingPosStart.x();
					float addElement = TexSizeX * drawSize / ((float)myGlyph->m_sizeTexture.x() * factorDisplay);
					// update texture start X Pos
					tuA += addElement;
				}
				if (dxB > m_clippingPosStop.x()) {
					// clip display
					float drawSize = dxB - m_clippingPosStop.x();
					// update element start display
					dxB = m_clippingPosStop.x();
					float addElement = TexSizeX * drawSize / ((float)myGlyph->m_sizeTexture.x() * factorDisplay);
					// update texture start X Pos
					tuB -= addElement;
				}
				float TexSizeY = tvC - tvD;
				if (dyC > m_clippingPosStop.y()) {
					// clip display
					float drawSize = dyC - m_clippingPosStop.y();
					// update element start display
					dyC = m_clippingPosStop.y();
					float addElement = TexSizeY * drawSize / ((float)myGlyph->m_sizeTexture.y() * factorDisplay);
					// update texture start X Pos
					tvC -= addElement;
				}
				if (dyD < m_clippingPosStart.y()) {
					// clip display
					float drawSize = m_clippingPosStart.y() - dyD;
					// update element start display
					dyD = m_clippingPosStart.y();
					float addElement = TexSizeY * drawSize / ((float)myGlyph->m_sizeTexture.y() * factorDisplay);
					// update texture start X Pos
					tvD += addElement;
				}
			}
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
				if (m_needDisplay == true) {
					vec3 bitmapDrawPos[4];
					bitmapDrawPos[0].setValue(dxA, dyC, 0);
					bitmapDrawPos[1].setValue(dxB, dyC, 0);
					bitmapDrawPos[2].setValue(dxB, dyD, 0);
					bitmapDrawPos[3].setValue(dxA, dyD, 0);
					/* texture Position : 
					 *   0------1
					 *   |      |
					 *   |      |
					 *   3------2
					 */
					vec2 texturePos[4];
					texturePos[0].setValue(tuA+m_mode, tvC);
					texturePos[1].setValue(tuB+m_mode, tvC);
					texturePos[2].setValue(tuB+m_mode, tvD);
					texturePos[3].setValue(tuA+m_mode, tvD);
					
					// NOTE : Android does not support the Quads elements ...
					/* Step 1 : 
					 *   ********     
					 *     ******     
					 *       ****     
					 *         **     
					 *                
					 */
					// set texture coordonates :
					m_coordTex.push_back(texturePos[0]);
					m_coordTex.push_back(texturePos[1]);
					m_coordTex.push_back(texturePos[2]);
					// set display positions :
					m_coord.push_back(bitmapDrawPos[0]);
					m_coord.push_back(bitmapDrawPos[1]);
					m_coord.push_back(bitmapDrawPos[2]);
					// set the color
					m_coordColor.push_back(m_color);
					m_coordColor.push_back(m_color);
					m_coordColor.push_back(m_color);
					/* Step 2 : 
					 *              
					 *   **         
					 *   ****       
					 *   ******     
					 *   ********   
					 */
					// set texture coordonates :
					m_coordTex.push_back(texturePos[0]);
					m_coordTex.push_back(texturePos[2]);
					m_coordTex.push_back(texturePos[3]);
					// set display positions :
					m_coord.push_back(bitmapDrawPos[0]);
					m_coord.push_back(bitmapDrawPos[2]);
					m_coord.push_back(bitmapDrawPos[3]);
					// set the color
					m_coordColor.push_back(m_color);
					m_coordColor.push_back(m_color);
					m_coordColor.push_back(m_color);
				}
			}
		}
	}
	// move the position :
	//EWOL_DEBUG(" 5 pos=" << m_position << " advance=" << myGlyph->m_advance.x() << " kerningOffset=" << kerningOffset);
	m_position.setX(m_position.x() + (myGlyph->m_advance.x() + kerningOffset) * factorDisplay);
	//EWOL_DEBUG(" 6 print '" << charcode << "' : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
	// Register the previous character
	m_previousCharcode = _charcode;
	return;
}
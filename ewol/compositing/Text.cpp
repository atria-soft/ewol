/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/context/Context.hpp>
#include <etk/types.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::compositing::Text);

ewol::compositing::Text::Text(const etk::String& _fontName, int32_t _fontSize) :
  m_font(null) {
	setFont(_fontName, _fontSize);
}

ewol::compositing::Text::~Text() {
	
}

void ewol::compositing::Text::drawMT(const mat4& _transformationMatrix, bool _enableDepthTest) {
	
	// draw BG in any case:
	m_vectorialDraw.draw();
	
	if (    m_VBO->bufferSize(m_vboIdCoord) <= 0
	     || m_font == null) {
		// TODO : set it back ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_font == null) {
		EWOL_WARNING("no font...");
		return;
	}
	if (m_GLprogram == null) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (_enableDepthTest == true) {
		gale::openGL::enable(gale::openGL::flag_depthTest);
	}
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	m_GLprogram->use(); 
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	// Texture:
	m_GLprogram->setTexture0(m_GLtexID, m_font->getRendererId());
	m_GLprogram->uniform1i(m_GLtextWidth, m_font->getOpenGlSize().x());
	m_GLprogram->uniform1i(m_GLtextHeight, m_font->getOpenGlSize().x());
	// position:
	m_GLprogram->sendAttributePointer(m_GLPosition, m_VBO, m_vboIdCoord);
	// Texture:
	m_GLprogram->sendAttributePointer(m_GLtexture, m_VBO, m_vboIdCoordText);
	// color:
	m_GLprogram->sendAttributePointer(m_GLColor, m_VBO, m_vboIdColor);
	// Request the draw od the elements:
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, m_VBO->bufferSize(m_vboIdCoord));
	m_GLprogram->unUse();
	if (_enableDepthTest == true) {
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
}

void ewol::compositing::Text::drawD(bool _disableDepthTest) {
	// draw BG in any case:
	m_vectorialDraw.draw(_disableDepthTest);
	
	if (    m_VBO->bufferSize(m_vboIdCoord) <= 0
	     || m_font == null) {
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_font == null) {
		EWOL_WARNING("no font...");
		return;
	}
	if (m_GLprogram == null) {
		EWOL_ERROR("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix()*m_matrixApply;
	m_GLprogram->use(); 
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	// Texture :
	m_GLprogram->setTexture0(m_GLtexID, m_font->getRendererId());
	m_GLprogram->uniform1i(m_GLtextWidth, m_font->getOpenGlSize().x());
	m_GLprogram->uniform1i(m_GLtextHeight, m_font->getOpenGlSize().x());
	// position:
	m_GLprogram->sendAttributePointer(m_GLPosition, m_VBO, m_vboIdCoord);
	// Texture:
	m_GLprogram->sendAttributePointer(m_GLtexture, m_VBO, m_vboIdCoordText);
	// color:
	m_GLprogram->sendAttributePointer(m_GLColor, m_VBO, m_vboIdColor);
	// Request the draw od the elements : 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, m_VBO->bufferSize(m_vboIdCoord));
	m_GLprogram->unUse();
}

float ewol::compositing::Text::getSize() {
	if (m_font == null) {
		EWOL_WARNING("no font...");
		return 1.0f;
	}
	return m_font->getFontSize();
}
float ewol::compositing::Text::getHeight() {
	if (m_font == null) {
		EWOL_WARNING("no font...");
		return 10.0f;
	}
	return m_font->getHeight(m_mode);
}
ewol::GlyphProperty * ewol::compositing::Text::getGlyphPointer(char32_t _charcode) {
	if (m_font == null) {
		EWOL_WARNING("no font...");
		return null;
	}
	return m_font->getGlyphPointer(_charcode, m_mode);
}

void ewol::compositing::Text::setFontSize(int32_t _fontSize) {
	// get old size
	etk::String fontName = "";
	if (m_font != null) {
		fontName = m_font->getName();
		// Remove the :XX for the size ...
		size_t pos = fontName.rfind(':');
		fontName.erase(pos, fontName.size()-pos);
	}
	setFont(fontName, _fontSize);
}

void ewol::compositing::Text::setFontName(const etk::String& _fontName) {
	// get old size
	int32_t fontSize = -1;
	if (m_font != null) {
		fontSize = m_font->getFontSize();
	}
	setFont(_fontName, fontSize);
}

void ewol::compositing::Text::setFont(etk::String _fontName, int32_t _fontSize) {
	clear();
	// remove old one
	ememory::SharedPtr<ewol::resource::TexturedFont> previousFont = m_font;
	if (_fontSize <= 0) {
		_fontSize = ewol::getContext().getFontDefault().getSize();
	}
	if (_fontName == "") {
		_fontName = ewol::getContext().getFontDefault().getName();
	}
	_fontName += ":";
	_fontName += etk::toString(_fontSize);
	EWOL_VERBOSE("plop : " << _fontName << " size=" << _fontSize << " result :" << _fontName);
	// link to new one
	m_font = ewol::resource::TexturedFont::create(_fontName);
	if (m_font == null) {
		EWOL_ERROR("Can not get font resource");
		m_font = previousFont;
	}
}

void ewol::compositing::Text::setFontMode(enum ewol::font::mode _mode) {
	if (m_font != null) {
		m_mode = m_font->getWrappingMode(_mode);
	}
}

void ewol::compositing::Text::printChar(const char32_t& _charcode) {
	// get a pointer on the glyph property : 
	ewol::GlyphProperty* myGlyph = getGlyphPointer(_charcode);
	if (null == myGlyph) {
		EWOL_ERROR(" font does not really existed ...");
		return;
	}
	int32_t fontSize = getSize();
	int32_t fontHeigh = getHeight();
	
	// get the kerning ofset :
	float kerningOffset = 0;
	if (m_kerning == true) {
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
		float dxA = m_position.x() + myGlyph->m_bearing.x() + kerningOffset;
		float dxB = dxA + myGlyph->m_sizeTexture.x();
		float dyC = m_position.y() + myGlyph->m_bearing.y() + fontHeigh - fontSize;
		float dyD = dyC - myGlyph->m_sizeTexture.y();
		
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
					float addElement = TexSizeX * drawSize / (float)myGlyph->m_sizeTexture.x();
					// update texture start X Pos
					tuA += addElement;
				}
				if (dxB > m_clippingPosStop.x()) {
					// clip display
					float drawSize = dxB - m_clippingPosStop.x();
					// update element start display
					dxB = m_clippingPosStop.x();
					float addElement = TexSizeX * drawSize / (float)myGlyph->m_sizeTexture.x();
					// update texture start X Pos
					tuB -= addElement;
				}
				float TexSizeY = tvC - tvD;
				if (dyC > m_clippingPosStop.y()) {
					// clip display
					float drawSize = dyC - m_clippingPosStop.y();
					// update element start display
					dyC = m_clippingPosStop.y();
					float addElement = TexSizeY * drawSize / (float)myGlyph->m_sizeTexture.y();
					// update texture start X Pos
					tvC -= addElement;
				}
				if (dyD < m_clippingPosStart.y()) {
					// clip display
					float drawSize = m_clippingPosStart.y() - dyD;
					// update element start display
					dyD = m_clippingPosStart.y();
					float addElement = TexSizeY * drawSize / (float)myGlyph->m_sizeTexture.y();
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
					bitmapDrawPos[0].setValue((int32_t)dxA, (int32_t)dyC, 0);
					bitmapDrawPos[1].setValue((int32_t)dxB, (int32_t)dyC, 0);
					bitmapDrawPos[2].setValue((int32_t)dxB, (int32_t)dyD, 0);
					bitmapDrawPos[3].setValue((int32_t)dxA, (int32_t)dyD, 0);
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
					m_VBO->pushOnBuffer(m_vboIdCoordText, texturePos[0]);
					m_VBO->pushOnBuffer(m_vboIdCoordText, texturePos[1]);
					m_VBO->pushOnBuffer(m_vboIdCoordText, texturePos[2]);
					// set display positions :
					m_VBO->pushOnBuffer(m_vboIdCoord, bitmapDrawPos[0]);
					m_VBO->pushOnBuffer(m_vboIdCoord, bitmapDrawPos[1]);
					m_VBO->pushOnBuffer(m_vboIdCoord, bitmapDrawPos[2]);
					// set the color
					m_VBO->pushOnBuffer(m_vboIdColor, m_color);
					m_VBO->pushOnBuffer(m_vboIdColor, m_color);
					m_VBO->pushOnBuffer(m_vboIdColor, m_color);
					/* Step 2 : 
					 *              
					 *   **         
					 *   ****       
					 *   ******     
					 *   ********   
					 */
					// set texture coordonates :
					m_VBO->pushOnBuffer(m_vboIdCoordText, texturePos[0]);
					m_VBO->pushOnBuffer(m_vboIdCoordText, texturePos[2]);
					m_VBO->pushOnBuffer(m_vboIdCoordText, texturePos[3]);
					// set display positions :
					m_VBO->pushOnBuffer(m_vboIdCoord, bitmapDrawPos[0]);
					m_VBO->pushOnBuffer(m_vboIdCoord, bitmapDrawPos[2]);
					m_VBO->pushOnBuffer(m_vboIdCoord, bitmapDrawPos[3]);
					// set the color
					m_VBO->pushOnBuffer(m_vboIdColor, m_color);
					m_VBO->pushOnBuffer(m_vboIdColor, m_color);
					m_VBO->pushOnBuffer(m_vboIdColor, m_color);
				}
			}
		}
	}
	// move the position :
	//EWOL_DEBUG(" 5 pos=" << m_position << " advance=" << myGlyph->m_advance.x() << " kerningOffset=" << kerningOffset);
	m_position.setX(m_position.x() + myGlyph->m_advance.x() + kerningOffset);
	//EWOL_DEBUG(" 6 print '" << charcode << "' : start=" << m_sizeDisplayStart << " stop=" << m_sizeDisplayStop << " pos=" << m_position);
	// Register the previous character
	m_previousCharcode = _charcode;
	m_VBO->flush();
	return;
}


vec3 ewol::compositing::Text::calculateSizeChar(const char32_t& _charcode) {
	// get a pointer on the glyph property : 
	ewol::GlyphProperty * myGlyph = getGlyphPointer(_charcode);
	int32_t fontHeigh = getHeight();
	if (myGlyph == null) {
		if (m_font == null) {
			EWOL_WARNING("no Glyph... in no font");
		} else {
			EWOL_WARNING("no Glyph... in font : " << m_font->getName());
		}
		return vec3((float)(0.2),
		            (float)(fontHeigh),
		            (float)(0.0));
	}
	// get the kerning ofset :
	float kerningOffset = 0.0;
	if (m_kerning == true) {
		kerningOffset = myGlyph->kerningGet(m_previousCharcode);
	}
	
	vec3 outputSize((float)(myGlyph->m_advance.x() + kerningOffset),
	                (float)(fontHeigh),
	                (float)(0.0));
	// Register the previous character
	m_previousCharcode = _charcode;
	return outputSize;
}


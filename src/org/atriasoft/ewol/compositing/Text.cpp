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

ewol::compositing::Text::Text( String _fontName, int _fontSize) :
  this.font(null) {
	setFont(_fontName, _fontSize);
}

ewol::compositing::Text::~Text() {
	
}

void ewol::compositing::Text::drawMT( mat4 _transformationMatrix, boolean _enableDepthTest) {
	
	// draw BG in any case:
	this.vectorialDraw.draw();
	
	if (    this.VBO.bufferSize(this.vboIdCoord) <= 0
	     || this.font == null) {
		// TODO : set it back ...
		//Log.warning("Nothink to draw...");
		return;
	}
	if (this.font == null) {
		Log.warning("no font...");
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	if (_enableDepthTest == true) {
		gale::openGL::enable(gale::openGL::flag_depthTest);
	}
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	this.GLprogram.use(); 
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// Texture:
	this.GLprogram.setTexture0(this.GLtexID, this.font.getRendererId());
	this.GLprogram.uniform1i(this.GLtextWidth, this.font.getOpenGlSize().x());
	this.GLprogram.uniform1i(this.GLtextHeight, this.font.getOpenGlSize().x());
	// position:
	this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, this.vboIdCoord);
	// Texture:
	this.GLprogram.sendAttributePointer(this.GLtexture, this.VBO, this.vboIdCoordText);
	// color:
	this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, this.vboIdColor);
	// Request the draw od the elements:
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, this.VBO.bufferSize(this.vboIdCoord));
	this.GLprogram.unUse();
	if (_enableDepthTest == true) {
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
}

void ewol::compositing::Text::drawD(boolean _disableDepthTest) {
	// draw BG in any case:
	this.vectorialDraw.draw(_disableDepthTest);
	
	if (    this.VBO.bufferSize(this.vboIdCoord) <= 0
	     || this.font == null) {
		//Log.warning("Nothink to draw...");
		return;
	}
	if (this.font == null) {
		Log.warning("no font...");
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix()*this.matrixApply;
	this.GLprogram.use(); 
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// Texture :
	this.GLprogram.setTexture0(this.GLtexID, this.font.getRendererId());
	this.GLprogram.uniform1i(this.GLtextWidth, this.font.getOpenGlSize().x());
	this.GLprogram.uniform1i(this.GLtextHeight, this.font.getOpenGlSize().x());
	// position:
	this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, this.vboIdCoord);
	// Texture:
	this.GLprogram.sendAttributePointer(this.GLtexture, this.VBO, this.vboIdCoordText);
	// color:
	this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, this.vboIdColor);
	// Request the draw od the elements : 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, this.VBO.bufferSize(this.vboIdCoord));
	this.GLprogram.unUse();
}

float ewol::compositing::Text::getSize() {
	if (this.font == null) {
		Log.warning("no font...");
		return 1.0f;
	}
	return this.font.getFontSize();
}
float ewol::compositing::Text::getHeight() {
	if (this.font == null) {
		Log.warning("no font...");
		return 10.0f;
	}
	return this.font.getHeight(this.mode);
}
ewol::GlyphProperty * ewol::compositing::Text::getGlyphPointer(Character _charcode) {
	if (this.font == null) {
		Log.warning("no font...");
		return null;
	}
	return this.font.getGlyphPointer(_charcode, this.mode);
}

void ewol::compositing::Text::setFontSize(int _fontSize) {
	// get old size
	String fontName = "";
	if (this.font != null) {
		fontName = this.font.getName();
		// Remove the :XX for the size ...
		int pos = fontName.rfind(':');
		fontName.erase(pos, fontName.size()-pos);
	}
	setFont(fontName, _fontSize);
}

void ewol::compositing::Text::setFontName( String _fontName) {
	// get old size
	int fontSize = -1;
	if (this.font != null) {
		fontSize = this.font.getFontSize();
	}
	setFont(_fontName, fontSize);
}

void ewol::compositing::Text::setFont(String _fontName, int _fontSize) {
	clear();
	// remove old one
	ememory::Ptr<ewol::resource::TexturedFont> previousFont = this.font;
	if (_fontSize <= 0) {
		_fontSize = ewol::getContext().getFontDefault().getSize();
	}
	if (_fontName == "") {
		_fontName = ewol::getContext().getFontDefault().getName();
	}
	_fontName += ":";
	_fontName += etk::toString(_fontSize);
	Log.verbose("plop : " + _fontName + " size=" + _fontSize + " result :" + _fontName);
	// link to new one
	this.font = ewol::resource::TexturedFont::create(_fontName);
	if (this.font == null) {
		Log.error("Can not get font resource");
		this.font = previousFont;
	}
}

void ewol::compositing::Text::setFontMode(enum ewol::font::mode _mode) {
	if (this.font != null) {
		this.mode = this.font.getWrappingMode(_mode);
	}
}

void ewol::compositing::Text::printChar( Character _charcode) {
	// get a pointer on the glyph property : 
	ewol::GlyphProperty* myGlyph = getGlyphPointer(_charcode);
	if (null == myGlyph) {
		Log.error(" font does not really existed ...");
		return;
	}
	int fontSize = getSize();
	int fontHeigh = getHeight();
	
	// get the kerning ofset :
	float kerningOffset = 0;
	if (this.kerning == true) {
		kerningOffset = myGlyph.kerningGet(this.previousCharcode);
		if (kerningOffset != 0) {
			//Log.debug("Kerning between : '" + this.previousCharcode + "''" + myGlyph.this.UVal + "' value : " + kerningOffset);
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
		float dxA = this.position.x() + myGlyph.this.bearing.x() + kerningOffset;
		float dxB = dxA + myGlyph.this.sizeTexture.x();
		float dyC = this.position.y() + myGlyph.this.bearing.y() + fontHeigh - fontSize;
		float dyD = dyC - myGlyph.this.sizeTexture.y();
		
		float tuA = myGlyph.this.texturePosStart.x();
		float tuB = tuA + myGlyph.this.texturePosSize.x();
		float tvC = myGlyph.this.texturePosStart.y();
		float tvD = tvC + myGlyph.this.texturePosSize.y();
		
		
		// Clipping and drawing area
		if(    this.clippingEnable == true
		    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (    dxB < this.clippingPosStart.x()
		         || dxA > this.clippingPosStop.x()
		         || dyC < this.clippingPosStart.y()
		         || dyD > this.clippingPosStop.y() ) ) {
			// Nothing to diplay ...
		} else {
			if (this.clippingEnable == true) {
				// generata positions...
				float TexSizeX = tuB - tuA;
				if (dxA < this.clippingPosStart.x()) {
					// clip display
					float drawSize = this.clippingPosStart.x() - dxA;
					// update element start display
					dxA = this.clippingPosStart.x();
					float addElement = TexSizeX * drawSize / (float)myGlyph.this.sizeTexture.x();
					// update texture start X Pos
					tuA += addElement;
				}
				if (dxB > this.clippingPosStop.x()) {
					// clip display
					float drawSize = dxB - this.clippingPosStop.x();
					// update element start display
					dxB = this.clippingPosStop.x();
					float addElement = TexSizeX * drawSize / (float)myGlyph.this.sizeTexture.x();
					// update texture start X Pos
					tuB -= addElement;
				}
				float TexSizeY = tvC - tvD;
				if (dyC > this.clippingPosStop.y()) {
					// clip display
					float drawSize = dyC - this.clippingPosStop.y();
					// update element start display
					dyC = this.clippingPosStop.y();
					float addElement = TexSizeY * drawSize / (float)myGlyph.this.sizeTexture.y();
					// update texture start X Pos
					tvC -= addElement;
				}
				if (dyD < this.clippingPosStart.y()) {
					// clip display
					float drawSize = this.clippingPosStart.y() - dyD;
					// update element start display
					dyD = this.clippingPosStart.y();
					float addElement = TexSizeY * drawSize / (float)myGlyph.this.sizeTexture.y();
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
				if (this.needDisplay == true) {
					Vector3f bitmapDrawPos[4];
					bitmapDrawPos[0].setValue((int)dxA, (int)dyC, 0);
					bitmapDrawPos[1].setValue((int)dxB, (int)dyC, 0);
					bitmapDrawPos[2].setValue((int)dxB, (int)dyD, 0);
					bitmapDrawPos[3].setValue((int)dxA, (int)dyD, 0);
					/* texture Position : 
					 *   0------1
					 *   |      |
					 *   |      |
					 *   3------2
					 */
					Vector2f texturePos[4];
					texturePos[0].setValue(tuA+this.mode, tvC);
					texturePos[1].setValue(tuB+this.mode, tvC);
					texturePos[2].setValue(tuB+this.mode, tvD);
					texturePos[3].setValue(tuA+this.mode, tvD);
					
					// NOTE : Android does not support the Quads elements ...
					/* Step 1 : 
					 *   ********     
					 *     ******     
					 *       ****     
					 *         **     
					 *                
					 */
					// set texture coordonates :
					this.VBO.pushOnBuffer(this.vboIdCoordText, texturePos[0]);
					this.VBO.pushOnBuffer(this.vboIdCoordText, texturePos[1]);
					this.VBO.pushOnBuffer(this.vboIdCoordText, texturePos[2]);
					// set display positions :
					this.VBO.pushOnBuffer(this.vboIdCoord, bitmapDrawPos[0]);
					this.VBO.pushOnBuffer(this.vboIdCoord, bitmapDrawPos[1]);
					this.VBO.pushOnBuffer(this.vboIdCoord, bitmapDrawPos[2]);
					// set the color
					this.VBO.pushOnBuffer(this.vboIdColor, this.color);
					this.VBO.pushOnBuffer(this.vboIdColor, this.color);
					this.VBO.pushOnBuffer(this.vboIdColor, this.color);
					/* Step 2 : 
					 *              
					 *   **         
					 *   ****       
					 *   ******     
					 *   ********   
					 */
					// set texture coordonates :
					this.VBO.pushOnBuffer(this.vboIdCoordText, texturePos[0]);
					this.VBO.pushOnBuffer(this.vboIdCoordText, texturePos[2]);
					this.VBO.pushOnBuffer(this.vboIdCoordText, texturePos[3]);
					// set display positions :
					this.VBO.pushOnBuffer(this.vboIdCoord, bitmapDrawPos[0]);
					this.VBO.pushOnBuffer(this.vboIdCoord, bitmapDrawPos[2]);
					this.VBO.pushOnBuffer(this.vboIdCoord, bitmapDrawPos[3]);
					// set the color
					this.VBO.pushOnBuffer(this.vboIdColor, this.color);
					this.VBO.pushOnBuffer(this.vboIdColor, this.color);
					this.VBO.pushOnBuffer(this.vboIdColor, this.color);
				}
			}
		}
	}
	// move the position :
	//Log.debug(" 5 pos=" + this.position + " advance=" + myGlyph.this.advance.x() + " kerningOffset=" + kerningOffset);
	this.position.setX(this.position.x() + myGlyph.this.advance.x() + kerningOffset);
	//Log.debug(" 6 print '" + charcode + "' : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
	// Register the previous character
	this.previousCharcode = _charcode;
	this.VBO.flush();
	return;
}


Vector3f ewol::compositing::Text::calculateSizeChar( Character _charcode) {
	// get a pointer on the glyph property : 
	ewol::GlyphProperty * myGlyph = getGlyphPointer(_charcode);
	int fontHeigh = getHeight();
	if (myGlyph == null) {
		if (this.font == null) {
			Log.warning("no Glyph... in no font");
		} else {
			Log.warning("no Glyph... in font : " + this.font.getName());
		}
		return Vector3f((float)(0.2),
		            (float)(fontHeigh),
		            (float)(0.0));
	}
	// get the kerning ofset :
	float kerningOffset = 0.0;
	if (this.kerning == true) {
		kerningOffset = myGlyph.kerningGet(this.previousCharcode);
	}
	
	Vector3f outputSize((float)(myGlyph.this.advance.x() + kerningOffset),
	                (float)(fontHeigh),
	                (float)(0.0));
	// Register the previous character
	this.previousCharcode = _charcode;
	return outputSize;
}


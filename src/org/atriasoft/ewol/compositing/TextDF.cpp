/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/compositing/TextDF.hpp>
#include <ewol/context/Context.hpp>
#include <etk/types.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::compositing::TextDF);

ewol::compositing::TextDF::TextDF( String _fontName, int _fontSize) :
  ewol::compositing::TextBase("", false),
  this.fontDF(null),
  this.GLglyphLevel(-1),
  this.size(12.0) {
	setFont(_fontName, _fontSize);
	loadProgram("DATA:///fontDistanceField/font1.prog?lib=ewol");
}

ewol::compositing::TextDF::~TextDF() {
	
}

void ewol::compositing::TextDF::updateSizeToRender( Vector2f _size) {
	float minSize = etk::min(_size.x(), _size.y());
	if (this.fontDF != null) {
		setFontSize(this.fontDF.getSize(minSize));
	}
}

void ewol::compositing::TextDF::drawMT( mat4 _transformationMatrix, boolean _enableDepthTest) {
	// draw BG in any case:
	this.vectorialDraw.draw();
	if (    this.VBO.bufferSize(this.vboIdCoord) <= 0
	     || this.fontDF == null) {
		//Log.warning("Nothink to draw...");
		return;
	}
	if (this.fontDF == null) {
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
	// set Matrix: translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	this.GLprogram.use(); 
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// Texture:
	this.GLprogram.setTexture0(this.GLtexID, this.fontDF.getRendererId());
	this.GLprogram.uniform1i(this.GLtextWidth, this.fontDF.getOpenGlSize().x());
	this.GLprogram.uniform1i(this.GLtextHeight, this.fontDF.getOpenGlSize().x());
	this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, this.vboIdCoord);
	this.GLprogram.sendAttributePointer(this.GLtexture, this.VBO, this.vboIdCoordText);
	this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, this.vboIdColor);
	this.GLprogram.sendAttributePointer(this.GLglyphLevel, this.VBO, this.vboIdGlyphLevel);
	// Request the draw od the elements:
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, this.VBO.bufferSize(this.vboIdCoord));
	this.GLprogram.unUse();
	if (_enableDepthTest == true) {
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
}


void ewol::compositing::TextDF::drawD(boolean _disableDepthTest) {
	// draw BG in any case:
	this.vectorialDraw.draw();
	
	if (    this.VBO.bufferSize(this.vboIdCoord) <= 0
	     || this.fontDF == null) {
		// TODO : Set it back
		//Log.warning("Nothink to draw...");
		return;
	}
	if (this.fontDF == null) {
		Log.warning("no font...");
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	// set Matrix: translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix()*this.matrixApply;
	this.GLprogram.use(); 
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// Texture:
	this.GLprogram.setTexture0(this.GLtexID, this.fontDF.getRendererId());
	this.GLprogram.uniform1i(this.GLtextWidth, this.fontDF.getOpenGlSize().x());
	this.GLprogram.uniform1i(this.GLtextHeight, this.fontDF.getOpenGlSize().x());
	this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, this.vboIdCoord);
	this.GLprogram.sendAttributePointer(this.GLtexture, this.VBO, this.vboIdCoordText);
	this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, this.vboIdColor);
	this.GLprogram.sendAttributePointer(this.GLglyphLevel, this.VBO, this.vboIdGlyphLevel);
	// Request the draw od the elements:
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, this.VBO.bufferSize(this.vboIdCoord));
	this.GLprogram.unUse();
}

void ewol::compositing::TextDF::loadProgram( String _shaderName) {
	ewol::compositing::TextBase::loadProgram(_shaderName);
	if (this.GLprogram != null) {
		this.GLglyphLevel = this.GLprogram.getAttribute("EW_glyphLevel");
	}
}


float ewol::compositing::TextDF::getHeight() {
	if (this.fontDF == null) {
		Log.warning("no font...");
		return 1;
	}
	return this.fontDF.getHeight(this.size);
}

ewol::GlyphProperty * ewol::compositing::TextDF::getGlyphPointer(Character _charcode) {
	if (this.fontDF == null) {
		Log.warning("no font...");
		return null;
	}
	return this.fontDF.getGlyphPointer(_charcode);
}

void ewol::compositing::TextDF::setFontSize(int _fontSize) {
	clear();
	Log.verbose("Set font Size: " + _fontSize);
	if (_fontSize <= 1) {
		this.size = ewol::getContext().getFontDefault().getSize();
	} else {
		this.size = _fontSize;
	}
}

void ewol::compositing::TextDF::setFontName( String _fontName) {
	clear();
	// remove old one
	ememory::Ptr<ewol::resource::DistanceFieldFont> previousFont = this.fontDF;
	String fontName;
	if (_fontName == "") {
		fontName = ewol::getContext().getFontDefault().getName();
	} else {
		fontName = _fontName;
	}
	Log.verbose("Set font name: '" + fontName + "'");
	// link to new one
	this.fontDF = ewol::resource::DistanceFieldFont::create(fontName);
	if (this.fontDF == null) {
		Log.error("Can not get find resource");
		this.fontDF = previousFont;
	}
}

void ewol::compositing::TextDF::setFont(String _fontName, int _fontSize) {
	setFontSize(_fontSize);
	setFontName(_fontName);
}

void ewol::compositing::TextDF::setFontMode(enum ewol::font::mode _mode) {
	this.mode = _mode;
}

//#define ANGLE_OF_ITALIC (tan(0.4))
#define ANGLE_OF_ITALIC (0.00698143f)


void ewol::compositing::TextDF::printChar( Character _charcode) {
	// get a pointer on the glyph property : 
	ewol::GlyphProperty* myGlyph = getGlyphPointer(_charcode);
	if (null == myGlyph) {
		Log.error(" font does not really existed ...");
		return;
	}
	float fontSize = getSize();
	float fontHeigh = getHeight();
	
	float factorDisplay = this.fontDF.getDisplayRatio(fontSize);
	
	// get the kerning ofset :
	float kerningOffset = 0;
	if (true == this.kerning) {
		kerningOffset = myGlyph.kerningGet(this.previousCharcode);
		if (kerningOffset != 0) {
			//Log.debug("Kerning between : '" + this.previousCharcode + "''" + myGlyph.this.UVal + "' value : " + kerningOffset);
		}
	}
	// 0x01 == 0x20 == ' ';
	if (    _charcode != 0x01
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _charcode != 0x20) {
		float glyphLevel = 0.5f;
		if (    this.mode == ewol::font::BoldItalic
		     || this.mode == ewol::font::Bold) {
			glyphLevel = 0.41f;
		}
		float italicMove = 0.0f;
		if (    this.mode == ewol::font::BoldItalic
		     || this.mode == ewol::font::Italic) {
			// This is a simple version of Italic mode, in theory we need to move the up and the down...
			italicMove = (float)myGlyph.this.sizeTexture.y() * factorDisplay * ANGLE_OF_ITALIC;
			// TODO : pb on the clipper...
		}
		
		/* Bitmap position
		 *      xA     xB
		 *   yC *------*
		 *      |      |
		 *      |      |
		 *   yD *------*
		 */
		#if 0
		float dxA = this.position.x() + (myGlyph.this.bearing.x() + kerningOffset) * factorDisplay;
		float dxB = dxA + myGlyph.this.sizeTexture.x() * factorDisplay;
		float dyC = this.position.y() + (myGlyph.this.bearing.y() + fontHeigh - fontSize) * factorDisplay;
		float dyD = dyC - myGlyph.this.sizeTexture.y() * factorDisplay;
		#else
		//Log.debug(" plop : fontHeigh" + fontHeigh + " fontSize=" + fontSize);
		float dxA = this.position.x() + ((float)myGlyph.this.bearing.x() + kerningOffset - (float)this.fontDF.getPixelBorderSize()*0.5f) * factorDisplay;
		float dxB = dxA + ((float)myGlyph.this.sizeTexture.x() + (float)this.fontDF.getPixelBorderSize()) * factorDisplay;
		float dyC = this.position.y() + (fontHeigh - fontSize + ((float)myGlyph.this.bearing.y() + (float)this.fontDF.getPixelBorderSize()*0.5f) * factorDisplay);
		float dyD = dyC - ((float)myGlyph.this.sizeTexture.y() + (float)this.fontDF.getPixelBorderSize()) * factorDisplay;
		#endif
		
		float tuA = myGlyph.this.texturePosStart.x();
		float tuB = tuA + myGlyph.this.texturePosSize.x();
		float tvC = myGlyph.this.texturePosStart.y();
		float tvD = tvC + myGlyph.this.texturePosSize.y();
		/*
		Vector3f drawingPos = this.vectorialDraw.getPos();
		etk::Color<> backColor = this.vectorialDraw.getColor();
		
		this.vectorialDraw.setPos(Vector2f(dxA, dyC));
		
		this.vectorialDraw.setColor(etk::Color<>(0.0,1.0,0.0,1.0));
		this.vectorialDraw.rectangle(Vector2f(dxB, dyD));
		
		this.vectorialDraw.setPos(drawingPos);
		this.vectorialDraw.setColor(backColor);
		*/
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
					float addElement = TexSizeX * drawSize / ((float)myGlyph.this.sizeTexture.x() * factorDisplay);
					// update texture start X Pos
					tuA += addElement;
				}
				if (dxB > this.clippingPosStop.x()) {
					// clip display
					float drawSize = dxB - this.clippingPosStop.x();
					// update element start display
					dxB = this.clippingPosStop.x();
					float addElement = TexSizeX * drawSize / ((float)myGlyph.this.sizeTexture.x() * factorDisplay);
					// update texture start X Pos
					tuB -= addElement;
				}
				float TexSizeY = tvC - tvD;
				if (dyC > this.clippingPosStop.y()) {
					// clip display
					float drawSize = dyC - this.clippingPosStop.y();
					// update element start display
					dyC = this.clippingPosStop.y();
					float addElement = TexSizeY * drawSize / ((float)myGlyph.this.sizeTexture.y() * factorDisplay);
					// update texture start X Pos
					tvC -= addElement;
				}
				if (dyD < this.clippingPosStart.y()) {
					// clip display
					float drawSize = this.clippingPosStart.y() - dyD;
					// update element start display
					dyD = this.clippingPosStart.y();
					float addElement = TexSizeY * drawSize / ((float)myGlyph.this.sizeTexture.y() * factorDisplay);
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
					bitmapDrawPos[0].setValue(dxA+italicMove, dyC, 0);
					bitmapDrawPos[1].setValue(dxB+italicMove, dyC, 0);
					bitmapDrawPos[2].setValue(dxB, dyD, 0);
					bitmapDrawPos[3].setValue(dxA, dyD, 0);
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
					// set the bliph level
					this.VBO.pushOnBuffer(this.vboIdGlyphLevel, glyphLevel);
					this.VBO.pushOnBuffer(this.vboIdGlyphLevel, glyphLevel);
					this.VBO.pushOnBuffer(this.vboIdGlyphLevel, glyphLevel);
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
					// set the bliph level
					this.VBO.pushOnBuffer(this.vboIdGlyphLevel, glyphLevel);
					this.VBO.pushOnBuffer(this.vboIdGlyphLevel, glyphLevel);
					this.VBO.pushOnBuffer(this.vboIdGlyphLevel, glyphLevel);
				}
			}
		}
	}
	// move the position :
	//Log.debug(" 5 pos=" + this.position + " advance=" + myGlyph.this.advance.x() + " kerningOffset=" + kerningOffset);
	this.position.setX(this.position.x() + (myGlyph.this.advance.x() + kerningOffset) * factorDisplay);
	//Log.debug(" 6 print '" + charcode + "' : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
	// Register the previous character
	this.previousCharcode = _charcode;
	this.VBO.flush();
	return;
}


Vector3f ewol::compositing::TextDF::calculateSizeChar( Character _charcode) {
	// get a pointer on the glyph property : 
	ewol::GlyphProperty * myGlyph = getGlyphPointer(_charcode);
	int fontHeigh = getHeight();
	
	// get the kerning ofset :
	float kerningOffset = 0.0;
	if (true == this.kerning) {
		kerningOffset = myGlyph.kerningGet(this.previousCharcode);
	}
	
	Vector3f outputSize((float)(myGlyph.this.advance.x() + kerningOffset)*this.fontDF.getDisplayRatio(getSize()),
	                (float)(fontHeigh),
	                (float)(0.0));
	// Register the previous character
	this.previousCharcode = _charcode;
	return outputSize;
}



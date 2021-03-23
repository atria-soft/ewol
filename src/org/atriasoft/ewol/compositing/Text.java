
/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.ResourceTexturedFont;
import org.atriasoft.ewol.resource.font.FontMode;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.gale.backend3d.OpenGL;

class Text extends TextBase {
	protected ResourceTexturedFont font; // !< Font resources

	List<Vector3f> pointPositions = new ArrayList<>();
	List<Vector2f> texturePositions = new ArrayList<>();
	List<Color> colors = new ArrayList<>();

	protected float size;

	/**
	 * generic ructor
	 * @param _fontName Name of the font that might be loaded
	 * @param _fontSize size of the font that might be loaded
	 */
	public Text() {
		this("");
	}

	public Text(final String _fontName) {
		this(_fontName, -1);
	}

	public Text(final String _fontName, final int _fontSize) {
		setFont(_fontName, _fontSize);
	}

	@Override
	public Vector3f calculateSizeChar(final Character _charcode) {
		// get a pointer on the glyph property :
		GlyphProperty myGlyph = getGlyphPointer(_charcode);
		int fontHeigh = (int) getHeight();
		if (myGlyph == null) {
			if (this.font == null) {
				Log.warning("no Glyph... in no font");
			} else {
				Log.warning("no Glyph... in font : " + this.font.getName());
			}
			return new Vector3f((float) (0.2), (fontHeigh), (float) (0.0));
		}
		// get the kerning ofset :
		float kerningOffset = 0.0f;
		if (this.kerning == true) {
			kerningOffset = myGlyph.kerningGet(this.previousCharcode);
		}

		Vector3f outputSize = new Vector3f(myGlyph.advance.x() + kerningOffset, (fontHeigh), 0.0f);
		// Register the previous character
		this.previousCharcode = _charcode;
		return outputSize;
	}

	@Override
	public void clear() {
		// call upper class
		super.clear();
		this.texturePositions.clear();
		// set display positions :
		this.pointPositions.clear();
		// set the color
		this.colors.clear();
	}

	@Override
	public void drawD(final boolean _disableDepthTest) {
		// draw BG in any case:
		// TODO this.vectorialDraw.draw(_disableDepthTest);

		// TODO : do it only one time (when needed ...)
		// set texture coordonates :
		this.VBO.setVboData(vboIdCoordText, this.texturePositions.toArray(Vector3f[]::new));
		// set display positions :
		this.VBO.setVboData(vboIdCoord, this.pointPositions.toArray(Vector2f[]::new));
		// set the color
		this.VBO.setVboData(vboIdColor, this.colors.toArray(Color[]::new));
		// TODO : do it only one time (when needed ...) --------- end -------

		if (this.VBO.bufferSize(TextBase.vboIdCoord) <= 0 || this.font == null) {
			// Log.warning("Nothink to draw...");
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
		Matrix4f tmpMatrix = OpenGL.getMatrix().multiply(this.matrixApply);
		this.GLprogram.use();
		this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
		// Texture :
		this.GLprogram.setTexture0(this.GLtexID, this.font.getRendererId());
		this.GLprogram.uniformInt(this.GLtextWidth, this.font.getOpenGlSize().x());
		this.GLprogram.uniformInt(this.GLtextHeight, this.font.getOpenGlSize().x());
		// position:
		this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, TextBase.vboIdCoord);
		// Texture:
		this.GLprogram.sendAttributePointer(this.GLtexture, this.VBO, TextBase.vboIdCoordText);
		// color:
		this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, TextBase.vboIdColor);
		// Request the draw od the elements :
		OpenGL.drawArrays(OpenGL.RenderMode.triangle, 0, this.VBO.bufferSize(TextBase.vboIdCoord));
		this.GLprogram.unUse();
	}

	@Override
	public void drawMT(final Matrix4f _transformationMatrix, final boolean _enableDepthTest) {

		// draw BG in any case:
		// TODO this.vectorialDraw.draw();

		// TODO : do it only one time (when needed ...)
		// set texture coordonates :
		this.VBO.setVboData(vboIdCoordText, this.texturePositions.toArray(Vector3f[]::new));
		// set display positions :
		this.VBO.setVboData(vboIdCoord, this.pointPositions.toArray(Vector2f[]::new));
		// set the color
		this.VBO.setVboData(vboIdColor, this.colors.toArray(Color[]::new));
		// TODO : do it only one time (when needed ...) --------- end -------

		if (this.VBO.bufferSize(TextBase.vboIdCoord) <= 0 || this.font == null) {
			// TODO : set it back ...
			// Log.warning("Nothink to draw...");
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
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
		}
		// set Matrix : translation/positionMatrix
		Matrix4f projMatrix = OpenGL.getMatrix();
		Matrix4f camMatrix = OpenGL.getCameraMatrix();
		Matrix4f tmpMatrix = projMatrix.multiply(camMatrix).multiply(_transformationMatrix);
		this.GLprogram.use();
		this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
		// Texture:
		this.GLprogram.setTexture0(this.GLtexID, this.font.getRendererId());
		this.GLprogram.uniformInt(this.GLtextWidth, this.font.getOpenGlSize().x());
		this.GLprogram.uniformInt(this.GLtextHeight, this.font.getOpenGlSize().x());
		// position:
		this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, TextBase.vboIdCoord);
		// Texture:
		this.GLprogram.sendAttributePointer(this.GLtexture, this.VBO, TextBase.vboIdCoordText);
		// color:
		this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, TextBase.vboIdColor);
		// Request the draw od the elements:
		OpenGL.drawArrays(OpenGL.RenderMode.triangle, 0, this.VBO.bufferSize(TextBase.vboIdCoord));
		this.GLprogram.unUse();
		if (_enableDepthTest == true) {
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		}
	}

	@Override
	public GlyphProperty getGlyphPointer(final Character _charcode) {
		if (this.font == null) {
			Log.warning("no font...");
			return null;
		}
		return this.font.getGlyph(_charcode, this.mode);
	}

	@Override
	public float getHeight() {
		if (this.font == null) {
			Log.warning("no font...");
			return 10.0f;
		}
		return this.font.getHeight(this.mode);
	}

	@Override
	public float getSize() {
		if (this.font == null) {
			Log.warning("no font...");
			return 1.0f;
		}
		return this.font.getFontSize();
	}

	@Override
	public void printChar(final Character _charcode) {
		// get a pointer on the glyph property :
		GlyphProperty myGlyph = getGlyphPointer(_charcode);
		if (myGlyph == null) {
			Log.error(" font does not really existed ...");
			return;
		}
		int fontSize = (int) getSize();
		int fontHeigh = (int) getHeight();

		// get the kerning ofset :
		float kerningOffset = 0;
		if (this.kerning == true) {
			kerningOffset = myGlyph.kerningGet(this.previousCharcode);
			if (kerningOffset != 0) {
				// Log.debug("Kerning between : '" + this.previousCharcode + "''" + myGlyph.UVal
				// + "' value : " + kerningOffset);
			}
		}
		// 0x01 == 0x20 == ' ';
		if (_charcode != 0x01) {
			/*
			 * Bitmap position xA xB yC *------* | | | | yD *------*
			 */
			float dxA = this.position.x() + myGlyph.bearing.x() + kerningOffset;
			float dxB = dxA + myGlyph.sizeTexture.x();
			float dyC = this.position.y() + myGlyph.bearing.y() + fontHeigh - fontSize;
			float dyD = dyC - myGlyph.sizeTexture.y();

			float tuA = myGlyph.texturePosStart.x();
			float tuB = tuA + myGlyph.texturePosSize.x();
			float tvC = myGlyph.texturePosStart.y();
			float tvD = tvC + myGlyph.texturePosSize.y();

			// Clipping and drawing area
			if (this.clippingEnable == true && (dxB < this.clippingPosStart.x() || dxA > this.clippingPosStop.x()
					|| dyC < this.clippingPosStart.y() || dyD > this.clippingPosStop.y())) {
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
						float addElement = TexSizeX * drawSize / myGlyph.sizeTexture.x();
						// update texture start X Pos
						tuA += addElement;
					}
					if (dxB > this.clippingPosStop.x()) {
						// clip display
						float drawSize = dxB - this.clippingPosStop.x();
						// update element start display
						dxB = this.clippingPosStop.x();
						float addElement = TexSizeX * drawSize / myGlyph.sizeTexture.x();
						// update texture start X Pos
						tuB -= addElement;
					}
					float TexSizeY = tvC - tvD;
					if (dyC > this.clippingPosStop.y()) {
						// clip display
						float drawSize = dyC - this.clippingPosStop.y();
						// update element start display
						dyC = this.clippingPosStop.y();
						float addElement = TexSizeY * drawSize / myGlyph.sizeTexture.y();
						// update texture start X Pos
						tvC -= addElement;
					}
					if (dyD < this.clippingPosStart.y()) {
						// clip display
						float drawSize = this.clippingPosStart.y() - dyD;
						// update element start display
						dyD = this.clippingPosStart.y();
						float addElement = TexSizeY * drawSize / myGlyph.sizeTexture.y();
						// update texture start X Pos
						tvD += addElement;
					}
				}
				if (dxB <= dxA || dyD >= dyC) {
					// nothing to do ...
				} else {
					/*
					 * Bitmap position 0------1 | | | | 3------2
					 */
					if (this.needDisplay == true) {
						Vector3f drawPosition0 = new Vector3f((int) dxA, (int) dyC, 0);
						Vector3f drawPosition1 = new Vector3f((int) dxB, (int) dyC, 0);
						Vector3f drawPosition2 = new Vector3f((int) dxB, (int) dyD, 0);
						Vector3f drawPosition3 = new Vector3f((int) dxA, (int) dyD, 0);
						/*
						 * texture Position : 0------1 | | | | 3------2
						 */
						Vector2f texturePos0 = new Vector2f(tuA + this.mode.getValue(), tvC);
						Vector2f texturePos1 = new Vector2f(tuB + this.mode.getValue(), tvC);
						Vector2f texturePos2 = new Vector2f(tuB + this.mode.getValue(), tvD);
						Vector2f texturePos3 = new Vector2f(tuA + this.mode.getValue(), tvD);

						// NOTE : Android does not support the Quads elements ...
						/*
						 * Step 1 : ******** ****** **** **
						 * 
						 */
						// set texture coordonates :
						this.texturePositions.add(texturePos0);
						this.texturePositions.add(texturePos1);
						this.texturePositions.add(texturePos2);
						// set display positions :
						this.pointPositions.add(drawPosition0);
						this.pointPositions.add(drawPosition1);
						this.pointPositions.add(drawPosition2);
						// set the color
						this.colors.add(this.color);
						this.colors.add(this.color);
						this.colors.add(this.color);
						/*
						 * Step 2 :
						 * 
						 * ** **** ****** ********
						 */
						// set texture coordonates :
						this.texturePositions.add(texturePos0);
						this.texturePositions.add(texturePos2);
						this.texturePositions.add(texturePos3);
						// set display positions :
						this.pointPositions.add(drawPosition0);
						this.pointPositions.add(drawPosition2);
						this.pointPositions.add(drawPosition3);
						// set the color
						this.colors.add(this.color);
						this.colors.add(this.color);
						this.colors.add(this.color);
					}
				}
			}
		}
		// move the position :
		// Log.debug(" 5 pos=" + this.position + " advance=" + myGlyph.advance.x() + "
		// kerningOffset=" + kerningOffset);
		this.position = this.position.withX(this.position.x() + myGlyph.advance.x() + kerningOffset);
		// Log.debug(" 6 print '" + charcode + "' : start=" + this.sizeDisplayStart + "
		// stop=" + this.sizeDisplayStop + " pos=" + this.position);
		// Register the previous character
		this.previousCharcode = _charcode;
		this.VBO.flush();
		return;
	}

	@Override
	public void setFont(String _fontName, int _fontSize) {
		clear();
		// remove old one
		ResourceTexturedFont previousFont = this.font;
		if (_fontSize <= 0) {
			_fontSize = Ewol.getContext().getFontDefault().getSize();
		}
		if (_fontName == "") {
			_fontName = Ewol.getContext().getFontDefault().getName();
		}
		_fontName += ":";
		_fontName += _fontSize;
		Log.verbose("plop : " + _fontName + " size=" + _fontSize + " result :" + _fontName);
		// link to new one
		this.font = ResourceTexturedFont.create(_fontName);
		if (this.font == null) {
			Log.error("Can not get font resource");
			this.font = previousFont;
		}
	}

	@Override
	public void setFontMode(final FontMode _mode) {
		if (this.font != null) {
			this.mode = this.font.getWrappingMode(_mode);
		}
	}

	@Override
	public void setFontName(final String _fontName) {
		// get old size
		int fontSize = -1;
		if (this.font != null) {
			fontSize = this.font.getFontSize();
		}
		setFont(_fontName, fontSize);
	}

	@Override
	public void setFontSize(final int _fontSize) {
		// get old size
		String fontName = "";
		if (this.font != null) {
			fontName = this.font.getName();
			// Remove the :XX for the size ...
			int pos = fontName.lastIndexOf(':');
			fontName = fontName.substring(0, pos);
		}
		setFont(fontName, _fontSize);
	}

};

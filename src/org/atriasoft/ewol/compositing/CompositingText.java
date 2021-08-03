
/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Configs;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.ResourceTexturedFont;
import org.atriasoft.ewol.resource.font.FontMode;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.gale.backend3d.OpenGL;

public class CompositingText extends TextBase {
	protected List<Color> colors = new ArrayList<>();
	protected ResourceTexturedFont font; // !< Font resources
	protected List<Vector3f> pointPositions = new ArrayList<>();
	protected float size;
	
	protected List<Vector2f> texturePositions = new ArrayList<>();
	
	public CompositingText() {
		this("");
	}
	
	public CompositingText(final String fontName) {
		this(fontName, -1);
	}
	
	/**
	 * generic constructor
	 * @param fontName Name of the font that might be loaded
	 * @param fontSize size of the font that might be loaded
	 */
	public CompositingText(final String fontName, final int fontSize) {
		setFont(fontName, fontSize);
	}
	
	@Override
	public Vector3f calculateSizeChar(final Character charcode) {
		// get a pointer on the glyph property :
		final GlyphProperty myGlyphProperty = getGlyphPointer(charcode);
		final int fontHeigh = (int) getHeight();
		if (myGlyphProperty == null) {
			if (this.font == null) {
				Log.warning("no Glyph... in no font");
			} else {
				Log.warning("no Glyph... in font : " + this.font.getName());
			}
			return new Vector3f((float) (0.2), (fontHeigh), (float) (0.0));
		}
		// get the kerning ofset :
		float kerningOffset = 0.0f;
		if (this.kerning) {
			kerningOffset = myGlyphProperty.kerningGet(this.previousCharcode);
		}
		
		final Vector3f outputSize = new Vector3f(myGlyphProperty.getAdvenceX() + kerningOffset, (fontHeigh), 0.0f);
		// Register the previous character
		this.previousCharcode = charcode;
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
	public void drawD(final boolean disableDepthTest) {
		// draw BG in any case:
		this.vectorialDraw.draw(disableDepthTest);
		
		if (this.vbo.getVertexCount() <= 0 || this.font == null) {
			// Log.warning("Nothink to draw...");
			return;
		}
		if (this.font == null) {
			Log.warning("no font...");
			return;
		}
		if (this.oGLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		// set Matrix : translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		this.oGLprogram.use();
		this.vbo.bindForRendering();
		this.oGLprogram.uniformMatrix(this.oGLMatrixProjection, projMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrixTransformation, this.matrixApply);
		this.oGLprogram.uniformMatrix(this.oGLMatrixView, camMatrix);
		// Texture :
		this.oGLprogram.setTexture0(this.oGLtexID, this.font.getRendererId());
		this.oGLprogram.uniformInt(this.oGLtextWidth, this.font.getOpenGlSize().x());
		this.oGLprogram.uniformInt(this.oGLtextHeight, this.font.getOpenGlSize().x());
		// Request the draw of the elements:
		this.vbo.renderArrays(OpenGL.RenderMode.TRIANGLE);
		
		this.vbo.unBindForRendering();
		this.oGLprogram.unUse();
	}
	
	@Override
	public void drawMT(final Matrix4f transformationMatrix, final boolean enableDepthTest) {
		
		// draw BG in any case:
		this.vectorialDraw.draw();
		
		if (this.vbo.getVertexCount() <= 0 || this.font == null) {
			// TODO : set it back ...
			// Log.warning("Nothink to draw...");
			return;
		}
		if (this.oGLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		if (enableDepthTest) {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
		}
		// set Matrix : translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		this.oGLprogram.use();
		this.vbo.bindForRendering();
		this.oGLprogram.uniformMatrix(this.oGLMatrixProjection, projMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrixTransformation, transformationMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrixView, camMatrix);
		// Texture:
		this.oGLprogram.setTexture0(this.oGLtexID, this.font.getRendererId());
		this.oGLprogram.uniformInt(this.oGLtextWidth, this.font.getOpenGlSize().x());
		this.oGLprogram.uniformInt(this.oGLtextHeight, this.font.getOpenGlSize().x());
		// Request the draw of the elements:
		this.vbo.renderArrays(OpenGL.RenderMode.TRIANGLE);
		
		this.vbo.unBindForRendering();
		this.oGLprogram.unUse();
		if (enableDepthTest) {
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		}
	}
	
	@Override
	public void flush() {
		super.flush();
		// set texture coordonates :
		this.vbo.setTextureCoordinate(this.texturePositions.toArray(Vector2f[]::new));
		// set display positions :
		this.vbo.setPosition(this.pointPositions.toArray(Vector3f[]::new));
		// set the color
		this.vbo.setColors(this.colors.toArray(Color[]::new));
		this.vbo.setVertexCount(this.pointPositions.size());
		this.vbo.flush();
	}
	
	@Override
	public GlyphProperty getGlyphPointer(final Character charcode) {
		if (this.font == null) {
			Log.warning("no font...");
			return null;
		}
		return this.font.getGlyph(charcode, this.mode);
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
	public void printChar(final Character charcode) {
		// get a pointer on the glyph property :
		final GlyphProperty myGlyphProperty = getGlyphPointer(charcode);
		if (myGlyphProperty == null) {
			Log.error(" font does not really existed ...");
			return;
		}
		final int fontSize = (int) getSize();
		final int fontHeigh = (int) getHeight();
		
		// get the kerning ofset :
		float kerningOffset = 0;
		if (this.kerning) {
			kerningOffset = myGlyphProperty.kerningGet(this.previousCharcode);
			if (kerningOffset != 0) {
				// Log.debug("Kerning between : '" + this.previousCharcode + "''" + myGlyph.UVal
				// + "' value : " + kerningOffset);
			}
		}
		// 0x01 == 0x20 == ' ';
		if (charcode != 0x01) {
			/*
			 * Bitmap position xA xB yC *------* | | | | yD *------*
			 */
			float dxA = this.position.x() + myGlyphProperty.getTextureRenderOffset().x() + kerningOffset;
			float dxB = dxA + myGlyphProperty.sizeTexture.x();
			float dyC = this.position.y() + myGlyphProperty.getTextureRenderOffset().y() + fontHeigh - fontSize;
			float dyD = dyC - myGlyphProperty.sizeTexture.y();
			
			float tuA = myGlyphProperty.texturePosStart.x();
			float tuB = tuA + myGlyphProperty.texturePosSize.x();
			float tvC = myGlyphProperty.texturePosStart.y();
			float tvD = tvC + myGlyphProperty.texturePosSize.y();
			
			// Clipping and drawing area
			if (this.clippingEnable && (dxB < this.clippingPosStart.x() || dxA > this.clippingPosStop.x() || dyC < this.clippingPosStart.y() || dyD > this.clippingPosStop.y())) {
				// Nothing to display ...
			} else {
				if (this.clippingEnable) {
					// generate a positions...
					final float texSizeX = tuB - tuA;
					if (dxA < this.clippingPosStart.x()) {
						// clip display
						final float drawSize = this.clippingPosStart.x() - dxA;
						// update element start display
						dxA = this.clippingPosStart.x();
						final float addElement = texSizeX * drawSize / myGlyphProperty.sizeTexture.x();
						// update texture start X Pos
						tuA += addElement;
					}
					if (dxB > this.clippingPosStop.x()) {
						// clip display
						final float drawSize = dxB - this.clippingPosStop.x();
						// update element start display
						dxB = this.clippingPosStop.x();
						final float addElement = texSizeX * drawSize / myGlyphProperty.sizeTexture.x();
						// update texture start X Pos
						tuB -= addElement;
					}
					final float texSizeY = tvC - tvD;
					if (dyC > this.clippingPosStop.y()) {
						// clip display
						final float drawSize = dyC - this.clippingPosStop.y();
						// update element start display
						dyC = this.clippingPosStop.y();
						final float addElement = texSizeY * drawSize / myGlyphProperty.sizeTexture.y();
						// update texture start X Pos
						tvC -= addElement;
					}
					if (dyD < this.clippingPosStart.y()) {
						// clip display
						final float drawSize = this.clippingPosStart.y() - dyD;
						// update element start display
						dyD = this.clippingPosStart.y();
						final float addElement = texSizeY * drawSize / myGlyphProperty.sizeTexture.y();
						// update texture start X Pos
						tvD += addElement;
					}
				}
				if (dxB <= dxA || dyD >= dyC) {
					// nothing to do ...
				} else /*
						* Bitmap position 0------1 | | | | 3------2
						*/
				if (this.needDisplay) {
					final Vector3f drawPosition0 = new Vector3f((int) dxA, (int) dyC, 0);
					final Vector3f drawPosition1 = new Vector3f((int) dxB, (int) dyC, 0);
					final Vector3f drawPosition2 = new Vector3f((int) dxB, (int) dyD, 0);
					final Vector3f drawPosition3 = new Vector3f((int) dxA, (int) dyD, 0);
					/*
					 * texture Position : 0------1 | | | | 3------2
					 */
					final Vector2f texturePos0 = new Vector2f(tuA + this.mode.getValue(), tvC);
					final Vector2f texturePos1 = new Vector2f(tuB + this.mode.getValue(), tvC);
					final Vector2f texturePos2 = new Vector2f(tuB + this.mode.getValue(), tvD);
					final Vector2f texturePos3 = new Vector2f(tuA + this.mode.getValue(), tvD);
					
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
		// move the position :
		// Log.debug(" 5 pos=" + this.position + " advance=" + myGlyph.advance.x() + "
		// kerningOffset=" + kerningOffset);
		this.position = this.position.withX(this.position.x() + myGlyphProperty.getAdvenceX() + kerningOffset);
		// Log.debug(" 6 print '" + char-code + "' : start=" + this.sizeDisplayStart + "
		// stop=" + this.sizeDisplayStop + " pos=" + this.position);
		// Register the previous character
		this.previousCharcode = charcode;
		this.vbo.flush();
	}
	
	@Override
	public void setFont(String fontName, int fontSize) {
		clear();
		// remove old one
		final ResourceTexturedFont previousFont = this.font;
		if (fontSize <= 0) {
			fontSize = Configs.getConfigFonts().getSize();
		}
		if (fontName.isEmpty()) {
			fontName = Configs.getConfigFonts().getName();
		}
		Uri fontUri = Configs.getConfigFonts().getFontUri(fontName).clone();
		fontUri.setProperty("size", Integer.toString(fontSize));
		Log.verbose("plop : " + fontName + " size=" + fontSize + " result :" + fontName);
		// link to new one
		this.font = ResourceTexturedFont.create(fontUri);
		if (this.font == null) {
			Log.error("Can not get font resource");
			this.font = previousFont;
		}
	}
	
	@Override
	public void setFontMode(final FontMode mode) {
		if (this.font != null) {
			this.mode = this.font.getWrappingMode(mode);
		}
	}
	
	@Override
	public void setFontName(final String fontName) {
		// get old size
		int fontSize = -1;
		if (this.font != null) {
			fontSize = this.font.getFontSize();
		}
		setFont(fontName, fontSize);
	}
	
	@Override
	public void setFontSize(final int fontSize) {
		// get old size
		String fontName = "";
		if (this.font != null) {
			fontName = this.font.getName();
			// Remove the :XX for the size ...
			final int pos = fontName.lastIndexOf(':');
			fontName = fontName.substring(0, pos);
		}
		setFont(fontName, fontSize);
	}
	
}

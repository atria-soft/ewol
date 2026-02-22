
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
import org.atriasoft.ewol.resource.ResourceTexturedFont;
import org.atriasoft.ewol.resource.font.FontMode;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.gale.backend3d.OpenGL;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class CompositingText extends TextBase {
	private static final Logger LOGGER = LoggerFactory.getLogger(CompositingText.class);
	protected List<Color> colors = new ArrayList<>();
	protected ResourceTexturedFont font; // !< Font resources
	protected List<Vector3f> pointPositions = new ArrayList<>();
	protected float size;
	protected List<Vector2f> texturePositions = new ArrayList<>();

	protected String currentFontName = "";
	protected int currentFontSize = -2; // -1 is to perform first initialization

	public CompositingText() {
		this("");
	}

	public CompositingText(final String fontName) {
		this(fontName, 0);
	}

	/**
	 * generic constructor
	 *
	 * @param fontName Name of the font that might be loaded
	 * @param fontSize Size of the font that might be loaded
	 */
	public CompositingText(final String fontName, final int fontSize) {
		setFont(fontName, fontSize);
	}

	@Override
	public Vector2f calculateSizeChar(final Character charcode) {
		final GlyphProperty myGlyphProperty = getGlyphPointer(charcode);
		final int fontHeight = (int) getHeight();
		if (myGlyphProperty == null) {
			if (this.font == null) {
				LOGGER.warn("no Glyph... in no font");
			} else {
				LOGGER.warn("no Glyph... in font : {}", this.font.getName());
			}
			return new Vector2f(0.2f, fontHeight);
		}
		float kerningOffset = 0F;
		if (this.kerning) {
			kerningOffset = myGlyphProperty.kerningGet(this.previousCharcode);
		}
		final Vector2f outputSize = new Vector2f(myGlyphProperty.getAdvenceX() + kerningOffset, fontHeight);
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
			// LOGGER.warn("Nothink to draw...");
			return;
		}
		if (this.font == null) {
			LOGGER.warn("no font...");
			return;
		}
		if (this.oGLprogram == null) {
			LOGGER.error("No shader ...");
			return;
		}
		// Enable blend for text rendering (text always needs alpha blending)
		OpenGL.enable(OpenGL.Flag.flag_blend);
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
		this.oGLprogram.uniformInt(this.oGLtextHeight, this.font.getOpenGlSize().y());
		// Request the draw of the elements:
		this.vbo.renderArrays(OpenGL.RenderMode.TRIANGLE);

		this.vbo.unBindForRendering();
		this.oGLprogram.unUse();
		// Disable blend after text rendering
		OpenGL.disable(OpenGL.Flag.flag_blend);
	}

	@Override
	public void drawMT(final Matrix4f transformationMatrix, final boolean enableDepthTest) {

		// draw BG in any case:
		this.vectorialDraw.draw();

		if (this.vbo.getVertexCount() <= 0 || this.font == null) {
			// TODO : set it back ...
			// LOGGER.warn("Nothink to draw...");
			return;
		}
		if (this.oGLprogram == null) {
			LOGGER.error("No shader ...");
			return;
		}
		// Enable blend for text rendering (text always needs alpha blending)
		OpenGL.enable(OpenGL.Flag.flag_blend);
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
		this.oGLprogram.uniformInt(this.oGLtextHeight, this.font.getOpenGlSize().y());
		// Request the draw of the elements:
		this.vbo.renderArrays(OpenGL.RenderMode.TRIANGLE);

		this.vbo.unBindForRendering();
		this.oGLprogram.unUse();
		if (enableDepthTest) {
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		}
		// Disable blend after text rendering
		OpenGL.disable(OpenGL.Flag.flag_blend);
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
			LOGGER.warn("no font...");
			return null;
		}
		return this.font.getGlyph(charcode, this.mode);
	}

	@Override
	public float getHeight() {
		if (this.font == null) {
			LOGGER.warn("no font...");
			return 10.0f;
		}
		return this.font.getHeight(this.mode);
	}

	@Override
	public float getSize() {
		if (this.font == null) {
			LOGGER.warn("no font...");
			return 1.0f;
		}
		return this.font.getFontSize();
	}

	@Override
	public void printChar(final Character charcode) {
		final GlyphProperty myGlyphProperty = getGlyphPointer(charcode);
		if (myGlyphProperty == null) {
			LOGGER.error(" font does not really existed ...");
			return;
		}
		final int fontSize = (int) getSize();
		final int fontHeight = (int) getHeight();

		float kerningOffset = 0F;
		if (this.kerning) {
			kerningOffset = myGlyphProperty.kerningGet(this.previousCharcode);
		}
		// 0x01 == 0x20 == ' ';
		if (charcode != 0x01) {
			/*
			 * Bitmap position xA xB yC *------* | | | | yD *------*
			 */
			float dxA = this.position.x() + myGlyphProperty.getTextureRenderOffset().x() + kerningOffset;
			float dxB = dxA + myGlyphProperty.sizeTexture.x();
			float dyC = this.position.y() + myGlyphProperty.getTextureRenderOffset().y() + fontHeight
					- fontSize;
			float dyD = dyC - myGlyphProperty.sizeTexture.y();

			float tuA = myGlyphProperty.texturePosStart.x();
			float tuB = tuA + myGlyphProperty.texturePosSize.x();
			float tvC = myGlyphProperty.texturePosStart.y();
			float tvD = tvC + myGlyphProperty.texturePosSize.y();

			// Clipping and drawing area
			if (this.clippingEnable && (dxB < this.clippingPosStart.x() || dxA > this.clippingPosStop.x()
					|| dyC < this.clippingPosStart.y() || dyD > this.clippingPosStop.y())) {
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
				} else if (this.needDisplay) {
					/*
					 * Bitmap position 0------1 | | | | 3------2
					 */
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
					// set texture coordinates :
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
					// set texture coordinates :
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
		// LOGGER.debug(" 5 pos=" + this.position + " advance=" + myGlyph.advance.x() +
		// "
		// kerningOffset=" + kerningOffset);
		this.position = this.position
				.withX(this.position.x() + myGlyphProperty.getAdvenceX() + kerningOffset);
		// LOGGER.debug(" 6 print '" + char-code + "' : start=" + this.sizeDisplayStart
		// + "
		// stop=" + this.sizeDisplayStop + " pos=" + this.position);
		// Register the previous character
		this.previousCharcode = charcode;
	}

	@Override
	public void setFont(final String inputFontName, final int inputFontSize) {
		if (inputFontName.equals(this.currentFontName) && inputFontSize == this.currentFontSize) {
			return;
		}

		String fontName = inputFontName;
		int fontSize = inputFontSize;
		// remove old one
		final ResourceTexturedFont previousFont = this.font;
		if (fontSize <= 0) {
			fontSize = Configs.getConfigFonts().getSize();
		}
		if (fontName.isEmpty()) {
			fontName = Configs.getConfigFonts().getName();
		}

		clear();
		final Uri fontUri = Configs.getConfigFonts().getFontUri(fontName).clone();
		fontUri.setProperty("size", Integer.toString(fontSize));
		LOGGER.trace("fontName={} fontSize={}", fontName, fontSize);
		this.font = ResourceTexturedFont.create(fontUri);
		if (this.font == null) {
			LOGGER.error("Can not get font resource");
			this.font = previousFont;
		} else {
			this.currentFontName = inputFontName;
			this.currentFontSize = inputFontSize;
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
		setFont(fontName, this.currentFontSize);
	}

	@Override
	public void setFontSize(final int fontSize) {
		setFont(this.currentFontName, fontSize);
	}

}

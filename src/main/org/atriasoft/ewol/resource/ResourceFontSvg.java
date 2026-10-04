/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import java.awt.image.BufferedImage;

import org.atriasoft.esvg.SvgFont;
import org.atriasoft.esvg.font.Glyph;
import org.atriasoft.esvg.raster.GlyphRaster;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.gale.resource.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
public class ResourceFontSvg extends Resource {
	private static final Logger LOGGER = LoggerFactory.getLogger(ResourceFontSvg.class);
	
	/**
	 * Get the font of {@code uri} (its size property apart), shared: the living
	 * one is kept (count of references + 1), otherwise it is loaded. Call
	 * {@link #release()} once when it is not used any more.
	 * @param uri File of the font.
	 * @return the font.
	 * @throws IllegalStateException if a resource of another type has this name.
	 */
	public static ResourceFontSvg create(final Uri uri) {
		LOGGER.trace("KEEP: FontFreeType: {}", uri);
		// Create cache key without size property (it's only for rendering, not loading)
		// Keep other properties like 'lib' that are needed for resource resolution
		final Uri cacheKey = uri.clone();
		cacheKey.getproperties().remove("size");

		final ResourceFontSvg existing = keepExisting(cacheKey.toString(), ResourceFontSvg.class);
		if (existing != null) {
			return existing;
		}
		LOGGER.debug("CREATE: FontFreeType: {}", uri);
		// need to crate a new one ...
		return new ResourceFontSvg(cacheKey);
	}
	
	private final SvgFont font;
	
	private ResourceFontSvg(final Uri uri) {
		super(uri);
		this.font = SvgFont.load(uri);
		if (this.font == null) {
			LOGGER.error(
					"... the font file could be opened and read, but it appears ... that its font format is unsupported");
		} else {
			// all OK
			LOGGER.debug("load font: '{}' glyph count = {}", uri, this.font.getNumGlyphs());
			// display();
		}
	}
	
	@Override
	public void cleanUp() {
		// nothing to do...
	}
	
	public synchronized void display() {
		LOGGER.debug("    number of glyph = {}", this.font.getNumGlyphs());
	}
	
	public synchronized boolean drawGlyph(
			final BufferedImage imageOut,
			final int fontSize,
			final Vector2i glyphPosition,
			final GlyphProperty property,
			final int posInImage) {
		return drawGlyph(imageOut, fontSize, glyphPosition, property, posInImage, false, false);
	}

	public synchronized boolean drawGlyph(
			final BufferedImage imageOut,
			final int fontSize,
			final Vector2i glyphPosition,
			final GlyphProperty property,
			final int posInImage,
			final boolean syntheticBold,
			final boolean syntheticItalic) {
		final GlyphRaster weight = this.font.render(property.glyph.getUnicodeValue(), fontSize,
				syntheticBold, syntheticItalic);
		if (weight == null) {
			return false;
		}
		// Update sizeTexture to match actual raster size (authoritative source of truth)
		if (weight.getWidth() != property.sizeTexture.x() || weight.getHeight() != property.sizeTexture.y()) {
			property.sizeTexture = new Vector2i(
					Math.max(property.sizeTexture.x(), weight.getWidth()),
					Math.max(property.sizeTexture.y(), weight.getHeight()));
		}
		final int maxX = Math.min(weight.getWidth(), imageOut.getWidth() - glyphPosition.x());
		final int maxY = Math.min(weight.getHeight(), imageOut.getHeight() - glyphPosition.y());
		for (int y = 0; y < maxY; y++) {
			for (int x = 0; x < maxX; x++) {
				final float valueColor = weight.get(x, weight.getHeight() - 1 - y);
				final int byteVal = (int) (valueColor * 255.0f) & 0xFF;
				final int px = glyphPosition.x() + x;
				final int py = glyphPosition.y() + y;
				final int argb = imageOut.getRGB(px, py);
				final int updated;
				switch (posInImage) {
					case 1:
						updated = (argb & 0xFF00FFFF) | (byteVal << 16);
						break;
					case 2:
						updated = (argb & 0xFFFF00FF) | (byteVal << 8);
						break;
					case 3:
						updated = (argb & 0xFFFFFF00) | byteVal;
						break;
					default:
						updated = (argb & 0x00FFFFFF) | (byteVal << 24);
						break;
				}
				imageOut.setRGB(px, py, updated);
			}
		}
		return true;
	}

	public synchronized boolean drawGlyph(
			final BufferedImage imageOut,
			final int fontSize,
			final GlyphProperty property,
			final int borderSize) {
		final GlyphRaster weight = this.font.render(property.glyph.getUnicodeValue(), fontSize);
		for (int jjj = 0; jjj < weight.getHeight(); jjj++) {
			for (int iii = 0; iii < weight.getWidth(); iii++) {
				final float valueColor = weight.get(iii, weight.getHeight() - 1 - jjj);
				final int gray = (int) (valueColor * 255.0f) & 0xFF;
				imageOut.getRaster().setSample(borderSize + iii, borderSize + jjj, 0, gray);
			}
		}
		return true;
	}
	
	public synchronized Vector2i calculateRasterSize(final int fontSize, final int unicodeVal,
			final boolean syntheticBold, final boolean syntheticItalic) {
		return this.font.calculateRasterSize(unicodeVal, fontSize, syntheticBold, syntheticItalic);
	}

	public synchronized GlyphProperty getGlyphProperty(final int fontSize, final int uicodeVal) {
		final Glyph glyph = this.font.getGlyphNullIfMissing(uicodeVal);
		GlyphProperty out;
		if (glyph == null) {
			out = new GlyphProperty(this.font, uicodeVal, fontSize);
		} else {
			out = new GlyphProperty(this.font, glyph, fontSize);
		}
		return out;
	}
	
	public synchronized int getHeight(final int fontSize) {
		return this.font.calculateFontRealHeight(fontSize);
	}
	
	public synchronized Vector2f getSize(final int fontSize, final String unicodeString) {
		final float width = this.font.calculateWidth(unicodeString, fontSize, false);
		final float height = this.font.calculateFontRealHeight(fontSize);
		return new Vector2f(width, height);
	}
	
	public synchronized float getSizeWithHeight(final float fontHeight) {
		return this.font.calculateFontSizeWithHeight(fontHeight);
	}
}

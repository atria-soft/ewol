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
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.gale.resource.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
public class ResourceFontSvg extends Resource {
	private static final Logger LOGGER = LoggerFactory.getLogger(ResourceFontSvg.class);
	
	public static ResourceFontSvg create(final Uri uri) {
		LOGGER.trace("KEEP: FontFreeType: {}", uri);
		// Create cache key without size/FORCE_CLIMP properties (they're only for rendering, not loading)
		// Keep other properties like 'lib' that are needed for resource resolution
		final Uri cacheKey = uri.clone();
		cacheKey.getproperties().remove("size");
		cacheKey.getproperties().remove("FORCE_CLIMP");

		ResourceFontSvg object = null;
		final Resource object2 = Resource.getManager().localKeep(cacheKey);
		if (object2 != null) {
			if (!(object2 instanceof ResourceFontSvg)) {
				LOGGER.error("Request resource file: '{}' with the wrong type (dynamic cast error)", uri);
				System.exit(-1);
				return null;
			}
			object = (ResourceFontSvg) object2;
		}
		if (object != null) {
			return object;
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
	
	public boolean drawGlyph(
			final BufferedImage imageOut,
			final int fontSize,
			final Vector2i glyphPosition,
			final GlyphProperty property,
			final int posInImage) {
		return drawGlyph(imageOut, fontSize, glyphPosition, property, posInImage, false);
	}

	// the forceClimp is to generate a forcing of the rendering in small font, this permit to have a correct view of the font, otherwise it will be transparent.
	public synchronized boolean drawGlyph(
			final BufferedImage imageOut,
			final int fontSize,
			final Vector2i glyphPosition,
			final GlyphProperty property,
			final int posInImage,
			final boolean forceClimp) {
		final GlyphRaster weight = this.font.render(property.glyph.getUnicodeValue(), fontSize);
		if (weight == null) {
			return false;
		}
		for (int yyy = 0; yyy < weight.getHeight(); yyy++) {
			for (int xxx = 0; xxx < weight.getWidth(); xxx++) {
				float valueColor = weight.get(xxx, weight.getHeight() - 1 - yyy);
				if (forceClimp) {
					valueColor = FMath.avg(-0.5f, ((valueColor - 0.2f) * 7.0f), 0.5f) + 0.5f;
				}
				final int byteVal = (int) (valueColor * 255.0f) & 0xFF;
				final int px = glyphPosition.x() + xxx;
				final int py = glyphPosition.y() + yyy;
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

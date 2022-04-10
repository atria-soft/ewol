/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import org.atriasoft.egami.ImageByte;
import org.atriasoft.egami.ImageByteMono;
import org.atriasoft.esvg.EsvgFont;
import org.atriasoft.esvg.font.Glyph;
import org.atriasoft.esvg.render.Weight;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.gale.resource.Resource;

// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
public class ResourceFontSvg extends Resource {
	
	public static ResourceFontSvg create(final Uri uri) {
		Log.verbose("KEEP: FontFreeType: " + uri);
		ResourceFontSvg object = null;
		final Resource object2 = Resource.getManager().localKeep(uri);
		if (object2 != null) {
			if (!(object2 instanceof ResourceFontSvg)) {
				Log.critical("Request resource file : '" + uri + "' With the wrong type (dynamic cast error)");
				return null;
			}
			object = (ResourceFontSvg) object2;
		}
		if (object != null) {
			return object;
		}
		Log.debug("CREATE: FontFreeType: " + uri);
		// need to crate a new one ...
		return new ResourceFontSvg(uri);
	}
	
	private final EsvgFont font;
	
	private ResourceFontSvg(final Uri uri) {
		super(uri);
		this.font = EsvgFont.load(uri);
		if (this.font == null) {
			Log.error("... the font file could be opened and read, but it appears ... that its font format is unsupported");
		} else {
			// all OK
			Log.debug("load font : '" + uri + "' glyph count = " + this.font.getNumGlyphs());
			// display();
		}
	}
	
	@Override
	public void cleanUp() {
		// nothing to do...
	}
	
	public synchronized void display() {
		Log.info("    number of glyph       = " + this.font.getNumGlyphs());
	}
	
	public boolean drawGlyph(final ImageByte imageOut, final int fontSize, final Vector2i glyphPosition, final GlyphProperty property, final int posInImage) {
		return drawGlyph(imageOut, fontSize, glyphPosition, property, posInImage, false);
	}
	
	// the forceClimp is to generate a forcing of the rendering in small font, this permit to have a correct view of the font, otherwise it will be transparent.
	public synchronized boolean drawGlyph(final ImageByte imageOut, final int fontSize, final Vector2i glyphPosition, final GlyphProperty property, final int posInImage, final boolean forceClimp) {
		final Weight weight = this.font.render(property.glyph.getUnicodeValue(), fontSize);
		if (weight == null) {
			return false;
		}
		for (int yyy = 0; yyy < weight.getHeight(); yyy++) {
			for (int xxx = 0; xxx < weight.getWidth(); xxx++) {
				float valueColor = weight.get(xxx, weight.getHeight() - 1 - yyy);
				if (forceClimp) {
					valueColor = FMath.avg(-0.5f, ((valueColor - 0.2f) * 7.0f), 0.5f) + 0.5f;
					//valueColor = FMath.avg(-0.5f, (valueColor * 20.0f), 0.5f) + 0.5f;
				}
				// set only alpha :
				switch (posInImage) {
					default:
					case 0:
						imageOut.setAFloat(glyphPosition.x() + xxx, glyphPosition.y() + yyy, valueColor);
						break;
					case 1:
						imageOut.setRFloat(glyphPosition.x() + xxx, glyphPosition.y() + yyy, valueColor);
						break;
					case 2:
						imageOut.setGFloat(glyphPosition.x() + xxx, glyphPosition.y() + yyy, valueColor);
						break;
					case 3:
						imageOut.setBFloat(glyphPosition.x() + xxx, glyphPosition.y() + yyy, valueColor);
						break;
				}
			}
		}
		return true;
	}
	
	public synchronized boolean drawGlyph(final ImageByteMono imageOut, final int fontSize, final GlyphProperty property, final int borderSize) {
		final Weight weight = this.font.render(property.glyph.getUnicodeValue(), fontSize);
		for (int jjj = 0; jjj < weight.getHeight(); jjj++) {
			for (int iii = 0; iii < weight.getWidth(); iii++) {
				final float valueColor = weight.get(iii, weight.getHeight() - 1 - jjj);
				// real set of color
				imageOut.set(borderSize + iii, borderSize + jjj, valueColor);
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

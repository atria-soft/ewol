/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import java.util.List;

import org.atriasoft.egami.Image;
import org.atriasoft.egami.ImageMono;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.internal.LoadPackageStream;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.font.FontBase;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.gale.resource.Resource;

import com.mlomb.freetypejni.Bitmap;
import com.mlomb.freetypejni.Face;
import com.mlomb.freetypejni.FreeType;
import com.mlomb.freetypejni.FreeTypeConstants;
import com.mlomb.freetypejni.FreeTypeConstants.FT_Kerning_Mode;
import com.mlomb.freetypejni.FreeTypeConstants.FT_Render_Mode;
import com.mlomb.freetypejni.GlyphSlot;
import com.mlomb.freetypejni.Kerning;
import com.mlomb.freetypejni.Library;

// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
public class ResourceFontFreeType extends FontBase {
	private static final Library LIBRARY;
	static {
		LIBRARY = FreeType.newLibrary();
	}
	
	public static ResourceFontFreeType create(final Uri uri) {
		Log.verbose("KEEP: FontFreeType: " + uri);
		ResourceFontFreeType object = null;
		final Resource object2 = Resource.getManager().localKeep(uri);
		if (object2 != null) {
			if (!(object2 instanceof ResourceFontFreeType)) {
				Log.critical("Request resource file : '" + uri + "' With the wrong type (dynamic cast error)");
				return null;
			}
			object = (ResourceFontFreeType) object2;
		}
		if (object != null) {
			return object;
		}
		Log.debug("CREATE: FontFreeType: " + uri);
		// need to crate a new one ...
		return new ResourceFontFreeType(uri);
	}
	
	private final Face fftFace;
	private final byte[] fileBuffer;
	
	private boolean init;
	
	private ResourceFontFreeType(final Uri uri) {
		super(uri);
		this.fileBuffer = LoadPackageStream.getAllData(uri.getPath());
		// load Face ...
		this.fftFace = ResourceFontFreeType.LIBRARY.newFace(this.fileBuffer, 0);
		if (this.fftFace == null) {
			Log.error("... the font file could be opened and read, but it appears ... that its font format is unsupported");
		} else {
			// all OK
			Log.debug("load font : \"" + uri + "\" glyph count = " + this.fftFace.getNumGlyphs());
			this.init = true;
			// display();
		}
	}
	
	@Override
	public synchronized void display() {
		if (!this.init) {
			return;
		}
		Log.info("    number of glyph       = " + this.fftFace.getNumGlyphs());
	}
	
	@Override
	public synchronized boolean drawGlyph(final Image imageOut, final int fontSize, final Vector2i glyphPosition, final GlyphProperty property, final int posInImage) {
		if (!this.init) {
			return false;
		}
		// 300dpi (hight quality) 96 dpi (normal quality)
		final int fontQuality = 96;
		// Select size ...
		// note tha +6 == *64 corespond with the 1/64th of points calculation of
		// freetype
		boolean error = this.fftFace.setCharSize(fontSize + 6, fontSize + 6, fontQuality, fontQuality);
		if (!error) {
			Log.error("FTSetCharSize  == > error in settings ...");
			return false;
		}
		// a small shortcut
		final GlyphSlot slot = this.fftFace.getGlyphSlot();
		// load glyph image into the slot (erase previous one)
		error = this.fftFace.loadGlyph(property.glyphIndex, FreeTypeConstants.FT_LOAD_DEFAULT);
		if (!error) {
			Log.error("FTLoadGlyph specify Glyph");
			return false;
		}
		// convert to an anti-aliased bitmap
		error = slot.renderGlyph(FT_Render_Mode.FT_RENDER_MODE_NORMAL);
		if (!error) {
			Log.error("FTRenderGlyph");
			return false;
		}
		// draw it on the output Image :
		final Bitmap bitmap = slot.getBitmap();
		for (int jjj = 0; jjj < bitmap.getRows(); jjj++) {
			for (int iii = 0; iii < bitmap.getWidth(); iii++) {
				final int valueColor = bitmap.getBuffer().get(iii + bitmap.getWidth() * jjj);
				// set only alpha :
				switch (posInImage) {
					default:
					case 0:
						imageOut.setA(glyphPosition.x() + iii, glyphPosition.y() + jjj, valueColor);
						break;
					case 1:
						imageOut.setR(glyphPosition.x() + iii, glyphPosition.y() + jjj, valueColor);
						break;
					case 2:
						imageOut.setG(glyphPosition.x() + iii, glyphPosition.y() + jjj, valueColor);
						break;
					case 3:
						imageOut.setB(glyphPosition.x() + iii, glyphPosition.y() + jjj, valueColor);
						break;
				}
				// real set of color
				
			}
		}
		return true;
	}
	
	@Override
	public synchronized boolean drawGlyph(final ImageMono imageOut, final int fontSize, final GlyphProperty property, final int borderSize) {
		if (!this.init) {
			return false;
		}
		// 300dpi (hight quality) 96 dpi (normal quality)
		final int fontQuality = 96;
		// Select size ...
		// note tha +6 == *64 corespond with the 1/64th of points calculation of
		// freetype
		boolean error = this.fftFace.setCharSize(fontSize + 6, fontSize + 6, fontQuality, fontQuality);
		if (!error) {
			Log.error("FTSetCharSize  == > error in settings ...");
			return false;
		}
		// a small shortcut
		final GlyphSlot slot = this.fftFace.getGlyphSlot();
		// load glyph image into the slot (erase previous one)
		error = this.fftFace.loadGlyph(property.glyphIndex, // glyph index
				FreeTypeConstants.FT_LOAD_DEFAULT);
		if (!error) {
			Log.error("FTLoadGlyph specify Glyph");
			return false;
		}
		// convert to an anti-aliased bitmap
		error = slot.renderGlyph(FT_Render_Mode.FT_RENDER_MODE_NORMAL); // TODO set FT_RENDER_MODE_MONO ==> 1 bit
		// value ==> faster generation ...
		if (!error) {
			Log.error("FTRenderGlyph");
			return false;
		}
		// resize output image :
		final Bitmap bitmap = slot.getBitmap();
		imageOut.resize(bitmap.getWidth() + 2 * borderSize, bitmap.getRows() + 2 * borderSize);
		
		for (int jjj = 0; jjj < bitmap.getRows(); jjj++) {
			for (int iii = 0; iii < bitmap.getWidth(); iii++) {
				final int valueColor = bitmap.getBuffer().get(iii + bitmap.getWidth() * jjj);
				// real set of color
				imageOut.set(borderSize + iii, borderSize + jjj, valueColor);
			}
		}
		return true;
	}
	
	@Override
	public synchronized void generateKerning(final int fontSize, final List<GlyphProperty> listGlyph) {
		if (!this.init) {
			return;
		}
		if (!this.fftFace.hasKerning()) {
			Log.info("No kerning generation (disable) in the font");
		}
		// 300dpi (hight quality) 96 dpi (normal quality)
		final int fontQuality = 96;
		// Select size ...
		// note tha +6 == *64 corespond with the 1/64th of points calculation of
		// freetype
		final boolean error = this.fftFace.setCharSize(fontSize + 6, fontSize + 6, fontQuality, fontQuality);
		if (!error) {
			Log.error("FTSetCharSize  == > error in settings ...");
			return;
		}
		// For all the kerning element we get the kerning value :
		for (int iii = 0; iii < listGlyph.size(); iii++) {
			listGlyph.get(iii).kerningClear();
			for (int kkk = 0; kkk < listGlyph.size(); kkk++) {
				final Kerning kerning = this.fftFace.getKerning(listGlyph.get(kkk).glyphIndex, listGlyph.get(iii).glyphIndex, FT_Kerning_Mode.FT_KERNING_UNFITTED);
				// add the kerning only if != 0 ...
				if (kerning.x != 0) {
					listGlyph.get(iii).kerningAdd(listGlyph.get(kkk).uVal, kerning.x / 32.0f);
					// Log.debug("Kerning between : '" + (char)listGlyph[iii].this.UVal + "''" +
					// (char)listGlyph[kkk].this.UVal + "' value : " + kerning.x + " => " +
					// (kerning.x/64.0f));
				}
			}
		}
	}
	
	@Override
	public synchronized boolean getGlyphProperty(final int fontSize, final GlyphProperty property) {
		if (!this.init) {
			return false;
		}
		// 300dpi (hight quality) 96 dpi (normal quality)
		final int fontQuality = 96;
		// Select size ...
		// note tha +6 == *64 corespond with the 1/64th of points calculation of
		// freetype
		boolean error = this.fftFace.setCharSize(fontSize + 6, fontSize + 6, fontQuality, fontQuality);
		if (!error) {
			Log.error("FTSetCharSize  == > error in settings ...");
			return false;
		}
		// a small shortcut
		final GlyphSlot slot = this.fftFace.getGlyphSlot();
		// retrieve glyph index from character code
		final int glyphindex = this.fftFace.getCharIndex(property.uVal);
		// load glyph image into the slot (erase previous one)
		error = this.fftFace.loadGlyph(glyphindex, // glyph index
				FreeTypeConstants.FT_LOAD_DEFAULT);
		if (!error) {
			Log.error("FTLoadGlyph specify Glyph");
			return false;
		}
		// convert to an anti-aliased bitmap
		error = slot.renderGlyph(FT_Render_Mode.FT_RENDER_MODE_NORMAL);
		if (!error) {
			Log.error("FTRenderGlyph");
			return false;
		}
		// set properties :
		property.glyphIndex = glyphindex;
		final Bitmap bitmap = slot.getBitmap();
		property.sizeTexture = new Vector2i(bitmap.getWidth(), bitmap.getRows());
		property.bearing = new Vector2i(slot.getMetrics().getHoriBearingX() >> 6, slot.getMetrics().getHoriBearingY() >> 6);
		property.advance = new Vector2i(slot.getMetrics().getHoriAdvance() >> 6, slot.getMetrics().getVertAdvance() >> 6);
		return true;
	}
	
	@Override
	public synchronized int getHeight(final int fontSize) {
		return (int) (fontSize * 1.43f); // this is a really "magic" number ...
	}
	
	@Override
	public synchronized Vector2f getSize(final int fontSize, final String unicodeString) {
		if (!this.init) {
			return new Vector2f(0, 0);
		}
		// TODO ...
		return new Vector2f(0, 0);
	}
	
	@Override
	public synchronized float getSizeWithHeight(final float fontHeight) {
		return fontHeight * 0.6993f; // this is a really "magic" number ...
	}
	
}

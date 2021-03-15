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
	private static final Library library;
	static {
		library = FreeType.newLibrary();
	}
	
	public static ResourceFontFreeType create(final Uri uri) {
		Log.verbose("KEEP: FontFreeType: " + uri);
		ResourceFontFreeType object = null;
		final Resource object2 = getManager().localKeep(uri);
		if (object2 != null) {
			object = (ResourceFontFreeType) object2;
			if (object == null) {
				Log.critical("Request resource file : '" + uri + "' With the wrong type (dynamic cast error)");
				return null;
			}
		}
		if (object != null) {
			return object;
		}
		Log.debug("CREATE: FontFreeType: " + uri);
		// need to crate a new one ...
		return new ResourceFontFreeType(uri);
	}
	
	private final byte[] FileBuffer;
	private final Face fftFace;
	
	private boolean init;
	
	private ResourceFontFreeType(final Uri _uri) {
		super(_uri);
		this.FileBuffer = LoadPackageStream.getAllData(_uri.getPath());
		// load Face ...
		this.fftFace = library.newFace(this.FileBuffer, 0);
		if (this.fftFace == null) {
			Log.error("... the font file could be opened and read, but it appears ... that its font format is unsupported");
		} else {
			// all OK
			Log.debug("load font : \"" + _uri + "\" glyph count = " + this.fftFace.getNumGlyphs());
			this.init = true;
			// display();
		}
	}
	
	@Override
	public synchronized void display() {
		if (this.init == false) {
			return;
		}
		Log.info("    number of glyph       = " + this.fftFace.getNumGlyphs());
	}
	
	@Override
	public synchronized boolean drawGlyph(final Image _imageOut, final int _fontSize, final Vector2i _glyphPosition, final GlyphProperty _property, final int _posInImage) {
		if (this.init == false) {
			return false;
		}
		// 300dpi (hight quality) 96 dpi (normal quality)
		final int fontQuality = 96;
		// Select size ...
		// note tha +6 == *64 corespond with the 1/64th of points calculation of freetype
		boolean error = this.fftFace.setCharSize(_fontSize + 6, _fontSize + 6, fontQuality, fontQuality);
		if (error == false) {
			Log.error("FT_Set_Char_Size  == > error in settings ...");
			return false;
		}
		// a small shortcut
		final GlyphSlot slot = this.fftFace.getGlyphSlot();
		// load glyph image into the slot (erase previous one)
		error = this.fftFace.loadGlyph(_property.glyphIndex, FreeTypeConstants.FT_LOAD_DEFAULT);
		if (error == false) {
			Log.error("FT_Load_Glyph specify Glyph");
			return false;
		}
		// convert to an anti-aliased bitmap
		error = slot.renderGlyph(FT_Render_Mode.FT_RENDER_MODE_NORMAL);
		if (error == false) {
			Log.error("FT_Render_Glyph");
			return false;
		}
		// draw it on the output Image :
		final Bitmap bitmap = slot.getBitmap();
		for (int jjj = 0; jjj < bitmap.getRows(); jjj++) {
			for (int iii = 0; iii < bitmap.getWidth(); iii++) {
				final int valueColor = bitmap.getBuffer().get(iii + bitmap.getWidth() * jjj);
				// set only alpha :
				switch (_posInImage) {
					default:
					case 0:
						_imageOut.setA(_glyphPosition.x + iii, _glyphPosition.y + jjj, valueColor);
						break;
					case 1:
						_imageOut.setR(_glyphPosition.x + iii, _glyphPosition.y + jjj, valueColor);
						break;
					case 2:
						_imageOut.setG(_glyphPosition.x + iii, _glyphPosition.y + jjj, valueColor);
						break;
					case 3:
						_imageOut.setB(_glyphPosition.x + iii, _glyphPosition.y + jjj, valueColor);
						break;
				}
				// real set of color
				
			}
		}
		return true;
	}
	
	@Override
	public synchronized boolean drawGlyph(final ImageMono _imageOut, final int _fontSize, final GlyphProperty _property, final int _borderSize) {
		if (false == this.init) {
			return false;
		}
		// 300dpi (hight quality) 96 dpi (normal quality)
		final int fontQuality = 96;
		// Select size ...
		// note tha +6 == *64 corespond with the 1/64th of points calculation of freetype
		boolean error = this.fftFace.setCharSize(_fontSize + 6, _fontSize + 6, fontQuality, fontQuality);
		if (error == false) {
			Log.error("FT_Set_Char_Size  == > error in settings ...");
			return false;
		}
		// a small shortcut
		final GlyphSlot slot = this.fftFace.getGlyphSlot();
		// load glyph image into the slot (erase previous one)
		error = this.fftFace.loadGlyph(_property.glyphIndex, // glyph index
				FreeTypeConstants.FT_LOAD_DEFAULT);
		if (error == false) {
			Log.error("FT_Load_Glyph specify Glyph");
			return false;
		}
		// convert to an anti-aliased bitmap
		error = slot.renderGlyph(FT_Render_Mode.FT_RENDER_MODE_NORMAL); // TODO : set FT_RENDER_MODE_MONO ==> 1 bit value ==> faster generation ...
		if (error == false) {
			Log.error("FT_Render_Glyph");
			return false;
		}
		// resize output image :
		final Bitmap bitmap = slot.getBitmap();
		_imageOut.resize(bitmap.getWidth() + 2 * _borderSize, bitmap.getRows() + 2 * _borderSize);
		
		for (int jjj = 0; jjj < bitmap.getRows(); jjj++) {
			for (int iii = 0; iii < bitmap.getWidth(); iii++) {
				final int valueColor = bitmap.getBuffer().get(iii + bitmap.getWidth() * jjj);
				// real set of color
				_imageOut.set(_borderSize + iii, _borderSize + jjj, valueColor);
			}
		}
		return true;
	}
	
	@Override
	public synchronized void generateKerning(final int fontSize, final List<GlyphProperty> listGlyph) {
		if (this.init == false) {
			return;
		}
		if (this.fftFace.hasKerning() == false) {
			Log.info("No kerning generation (disable) in the font");
		}
		// 300dpi (hight quality) 96 dpi (normal quality)
		final int fontQuality = 96;
		// Select size ...
		// note tha +6 == *64 corespond with the 1/64th of points calculation of freetype
		final boolean error = this.fftFace.setCharSize(fontSize + 6, fontSize + 6, fontQuality, fontQuality);
		if (error == false) {
			Log.error("FT_Set_Char_Size  == > error in settings ...");
			return;
		}
		// For all the kerning element we get the kerning value :
		for (int iii = 0; iii < listGlyph.size(); iii++) {
			listGlyph.get(iii).kerningClear();
			for (int kkk = 0; kkk < listGlyph.size(); kkk++) {
				final Kerning kerning = this.fftFace.getKerning(listGlyph.get(kkk).glyphIndex, listGlyph.get(iii).glyphIndex, FT_Kerning_Mode.FT_KERNING_UNFITTED);
				// add the kerning only if != 0 ... 
				if (kerning.x != 0) {
					listGlyph.get(iii).kerningAdd(listGlyph.get(kkk).UVal, kerning.x / 32.0f);
					//Log.debug("Kerning between : '" + (char)listGlyph[iii].this.UVal + "''" + (char)listGlyph[kkk].this.UVal + "' value : " + kerning.x + " => " + (kerning.x/64.0f));
				}
			}
		}
	}
	
	@Override
	public synchronized boolean getGlyphProperty(final int _fontSize, final GlyphProperty _property) {
		if (false == this.init) {
			return false;
		}
		// 300dpi (hight quality) 96 dpi (normal quality)
		final int fontQuality = 96;
		// Select size ...
		// note tha +6 == *64 corespond with the 1/64th of points calculation of freetype
		boolean error = this.fftFace.setCharSize(_fontSize + 6, _fontSize + 6, fontQuality, fontQuality);
		if (error == false) {
			Log.error("FT_Set_Char_Size  == > error in settings ...");
			return false;
		}
		// a small shortcut
		final GlyphSlot slot = this.fftFace.getGlyphSlot();
		// retrieve glyph index from character code 
		final int glyph_index = this.fftFace.getCharIndex(_property.UVal);
		// load glyph image into the slot (erase previous one)
		error = this.fftFace.loadGlyph(glyph_index, // glyph index
				FreeTypeConstants.FT_LOAD_DEFAULT);
		if (error == false) {
			Log.error("FT_Load_Glyph specify Glyph");
			return false;
		}
		// convert to an anti-aliased bitmap
		error = slot.renderGlyph(FT_Render_Mode.FT_RENDER_MODE_NORMAL);
		if (error == false) {
			Log.error("FT_Render_Glyph");
			return false;
		}
		// set properties :
		_property.glyphIndex = glyph_index;
		final Bitmap bitmap = slot.getBitmap();
		_property.sizeTexture.setValue(bitmap.getWidth(), bitmap.getRows());
		_property.bearing.setValue(slot.getMetrics().getHoriBearingX() >> 6, slot.getMetrics().getHoriBearingY() >> 6);
		_property.advance.setValue(slot.getMetrics().getHoriAdvance() >> 6, slot.getMetrics().getVertAdvance() >> 6);
		return true;
	}
	
	@Override
	public synchronized int getHeight(final int _fontSize) {
		return (int) (_fontSize * 1.43f); // this is a really "magic" number ...
	}
	
	@Override
	public synchronized Vector2f getSize(final int _fontSize, final String _unicodeString) {
		if (this.init == false) {
			return new Vector2f(0, 0);
		}
		// TODO ...
		return new Vector2f(0, 0);
	}
	
	@Override
	public synchronized float getSizeWithHeight(final float _fontHeight) {
		return _fontHeight * 0.6993f; // this is a really "magic" number ...
	}
	
}

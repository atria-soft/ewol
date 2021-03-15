/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource.font;

import java.util.List;

import org.atriasoft.egami.Image;
import org.atriasoft.egami.ImageMono;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.gale.resource.Resource;

// https://developer.mozilla.org/fr/docs/Web/SVG/Tutorial/SVG_fonts
// https://convertio.co/fr/ttf-svg/

public abstract class FontBase extends Resource {
	public FontBase(final Uri _uri) {
		super(_uri);
	}
	
	@Override
	public void cleanUp() {
		// TODO Auto-generated method stub
		
	}
	
	public void display() {}
	
	public abstract boolean drawGlyph(final Image _imageOut, final int _fontSize, final Vector2i _glyphPosition, GlyphProperty _property, int _posInImage);
	
	public boolean drawGlyph(final ImageMono _imageOut, final int _fontSize, final GlyphProperty _property) {
		return drawGlyph(_imageOut, _fontSize, _property, 0);
	}
	
	public abstract boolean drawGlyph(final ImageMono _imageOut, final int _fontSize, GlyphProperty _property, int _borderSize);
	
	public void generateKerning(final int _fontSize, final List<GlyphProperty> _listGlyph) {}
	
	public abstract boolean getGlyphProperty(final int _fontSize, GlyphProperty _property);
	
	public abstract int getHeight(final int _fontSize);
	
	public abstract Vector2f getSize(final int _fontSize, final String _unicodeString);
	
	public abstract float getSizeWithHeight(final float _fontHeight);;
}
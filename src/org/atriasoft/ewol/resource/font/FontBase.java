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

// https://developer.mozilla.org/fr/docs/Web/SVG/Tutorial/SVGfonts
// https://convertio.co/fr/ttf-svg/

public abstract class FontBase extends Resource {
	public FontBase(final Uri uri) {
		super(uri);
	}
	
	@Override
	public void cleanUp() {
		// TODO Auto-generated method stub
		
	}
	
	public void display() {}
	
	public abstract boolean drawGlyph(final Image imageOut, final int fontSize, final Vector2i glyphPosition, GlyphProperty property, int posInImage);
	
	public boolean drawGlyph(final ImageMono imageOut, final int fontSize, final GlyphProperty property) {
		return drawGlyph(imageOut, fontSize, property, 0);
	}
	
	public abstract boolean drawGlyph(final ImageMono imageOut, final int fontSize, GlyphProperty property, int borderSize);
	
	public void generateKerning(final int fontSize, final List<GlyphProperty> listGlyph) {}
	
	public abstract boolean getGlyphProperty(final int fontSize, GlyphProperty property);
	
	public abstract int getHeight(final int fontSize);
	
	public abstract Vector2f getSize(final int fontSize, final String unicodeString);
	
	public abstract float getSizeWithHeight(final float fontHeight);
}
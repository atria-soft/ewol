/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource.font;

import org.atriasoft.esvg.SvgFont;
import org.atriasoft.esvg.font.Glyph;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;

/*
                             |            |          |            |            
                             |            |          |            |            
                             |            |          |            |            
                      Y      |            |          |            |            
                      ^      |------------|          |------------|            
                      |                                                        
    advance.y:   /->  |                                                        
                 |    |                                                        
                 |    |                                                        
 sizeTex.x /->   |    |         |------------|          |------------|         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |     A      |          |     G      |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           \->   |    |         |------------|          |------------|         
        /-->     |    |                                                        
        \-->     \->  |                                                        
  bearing.y           |                                                        
                      |>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   X    
                           <------------------------> : advance.x              
                                <------------> : sizeTexture.x                 
                           <---> : bearing.x                                 
                       
*/
/**
 * @notindoc
 */
public class GlyphProperty {
	private final int charcode;
	private final int fontSize;
	public Glyph glyph = null;
	private final float scaleFactor;
	public Vector2i sizeTexture = new Vector2i(10, 10); //!< size of the element to display
	public Vector2f texturePosSize = Vector2f.ZERO; //!< Texture normalized size (SIZE)
	public Vector2f texturePosStart = Vector2f.ZERO; //!< Texture normalized position (START)
	public Vector2f textureRenderOffset = Vector2f.ZERO; //!< Offset to apply on the rendering to display glyph at the good position (correct position when render texture is bigger than the glyph size
	
	public GlyphProperty(final SvgFont font, final Glyph glyph, final int fontSize) {
		this.glyph = glyph;
		this.charcode = this.glyph.getUnicodeValue();
		this.fontSize = fontSize;
		this.sizeTexture = font.calculateWidthRendering(glyph.getUnicodeValue(), fontSize);
		this.scaleFactor = font.calculateScaleFactor(fontSize);
		this.textureRenderOffset = font.calculateRenderOffset(fontSize);
	}
	
	public GlyphProperty(final SvgFont font, final int charcode, final int fontSize) {
		this.glyph = null;
		this.charcode = charcode;
		this.fontSize = fontSize;
		//this.sizeTexture = null;
		this.scaleFactor = 0;
	}
	
	/**
	 * get the status of the char, if it exist or not in the FONT
	 * @return true if the char is availlable, false otherwise
	 */
	public boolean exist() {
		return this.glyph != null;
	}
	
	public float getAdvenceX() {
		if (this.glyph == null) {
			return 500 * this.scaleFactor;
		}
		return this.glyph.getHorizAdvX() * this.scaleFactor;
	}
	
	public Vector2f getTextureRenderOffset() {
		return this.textureRenderOffset;
	}
	
	public int getUnicodeValue() {
		return this.charcode;
	}
	
	public float kerningGet(final Character charcode) {
		if (this.glyph == null) {
			return 0;
		}
		return this.glyph.getKerning(charcode) * this.scaleFactor;
	}
	
}

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource.font;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;

/*
                             |            |          |            |            
                             |            |          |            |            
                             |            |          |            |            
                      Y      |            |          |            |            
                      ^      |------------|          |------------|            
                      |                                                        
    advance.y:/->     |                                                        
                 |    |                                                        
                 |    |                                                        
 sizeTex.x/->    |    |         |------------|          |------------|         
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
                      |**>>   X        
                                                                               
                                                                               
                           <-----------------------. : advance.x            
                                                                               
                                <-----------. : sizeTexture.x               
                                                                               
                           <--. : bearing.x                                 
                       
*/
/**
 * @notindoc
 */
public class GlyphProperty {
	public Vector2i advance = new Vector2i(10, 10); //!< space use in the display for this specific char
	public Vector2i bearing = new Vector2i(2, 2); //!< offset to display the data (can be negatif id the texture sise is bigger than the theoric places in the string)
	public boolean exist = true;
	public int glyphIndex = 0; //!< Glyph index in the system
	private final List<Kerning> kerning = new ArrayList<>(); //!< kerning values of link of all elements
	public Vector2i sizeTexture = new Vector2i(10, 10); //!< size of the element to display
	public Vector2f texturePosSize = new Vector2f(0, 0); //!< Texture normalized position (SIZE)
	public Vector2f texturePosStart = new Vector2f(0, 0); //!< Texture normalized position (START)
	public Character uVal = 0; //!< Unicode value
	
	public GlyphProperty() {
		
	}
	
	/**
	 * get the status of the char, if it exist or not in the FONT
	 * @return true if the char is availlable, false otherwise
	 */
	public boolean exist() {
		return this.exist;
	}
	
	public void kerningAdd(final Character charcode, final float value) {
		this.kerning.add(new Kerning(charcode, value));
	}
	
	public void kerningClear() {
		this.kerning.clear();
	}
	
	public float kerningGet(final Character charcode) {
		for (int iii = 0; iii < this.kerning.size(); iii++) {
			if (this.kerning.get(iii).uVal == charcode) {
				return this.kerning.get(iii).value;
			}
		}
		return 0;
	}
	
	/**
	 * set the element doen not exist !!!
	 */
	public void setNotExist() {
		this.exist = false;
	}
}

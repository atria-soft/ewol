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
        advance.y:/.  |                                                        
                 |    |                                                        
                 |    |                                                        
   sizeTex.x/.   |    |         |------------|          |------------|         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |     A      |          |     G      |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
           |     |    |         |            |          |            |         
            \.   |    |         |------------|          |------------|         
         /-.     |    |                                                        
          \-.     \.  |                                                        
  bearing.y           |                                                        
                      |____*________________________*____________>>   X        
                                                                               
                                                                               
                           <-----------------------. : advance.x            
                                                                               
                                <-----------. : sizeTexture.x               
                                                                               
                           <--. : bearing.x                                 
                       
*/
/**
 * @not_in_doc
 */
public class GlyphProperty {
	public Character UVal = 0; //!< Unicode value
	public boolean exist = true;
	public int glyphIndex = 0; //!< Glyph index in the system
	public Vector2i sizeTexture = new Vector2i(10, 10); //!< size of the element to display
	public Vector2i bearing = new Vector2i(2, 2); //!< offset to display the data (can be negatif id the texture sise is bigger than the theoric places in the string)
	public Vector2i advance = new Vector2i(10, 10); //!< space use in the display for this specific char
	public Vector2f texturePosStart = new Vector2f(0, 0); //!< Texture normalized position (START)
	public Vector2f texturePosSize = new Vector2f(0, 0); //!< Texture normalized position (SIZE)
	private final List<Kerning> kerning = new ArrayList<>(); //!< kerning values of link of all elements
	
	public GlyphProperty() {
		
	}
	
	/**
	 * get the status of the char, if it exist or not in the FONT
	 * @return true if the char is availlable, false otherwise
	 */
	public boolean exist() {
		return this.exist;
	}
	
	public void kerningAdd(final Character _charcode, final float _value) {
		this.kerning.add(new Kerning(_charcode, _value));
	}
	
	public void kerningClear() {
		this.kerning.clear();
	}
	
	public float kerningGet(final Character _charcode) {
		for (int iii = 0; iii < this.kerning.size(); iii++) {
			if (this.kerning.get(iii).UVal == _charcode) {
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

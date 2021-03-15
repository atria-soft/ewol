/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource.font;

/**
 * @not_in_doc
 * @brief Kerning properties of one specific Glyph with an other
 * 
 * Without Kerning :
 * [pre]
 *                                     
 *        \          /      /\         
 *         \        /      /  \        
 *          \      /      /    \       
 *           \    /      /______\      
 *            \  /      /        \     
 *             \/      /          \    
 *        v          v a          a    
 * [/pre]
 * 
 * With Kerning :
 * [pre]
 *                                     
 *        \          /  /\             
 *         \        /  /  \            
 *          \      /  /    \           
 *           \    /  /______\          
 *            \  /  /        \         
 *             \/  /          \        
 *        v        a v        a        
 * [/pre]
 * 
 * @note The "Kerning" is the methode to provide a better display for some string like
 *       the "VA" has 2 letter that overlap themself. This name Kerning
 */
public class Kerning {
	public Character UVal; //!< unicode value (the previous character that must be before)
	public float value; //!< kerning real offset
	
	/**
	 * @brief Simple ructor that allow to allocate the List element
	 */
	public Kerning() {
		this.UVal = 0;
		this.value = 0;
	};
	
	/**
	 * @brief Normal ructor
	 * @param[in] _charcode The Unicode value of the coresponding character that might be before
	 * @param[in] _value The Kerning value of the offset (nb pixel number)
	 */
	public Kerning(final Character _charcode, final float _value) {
		this.UVal = _charcode;
		this.value = _value;
	}
}

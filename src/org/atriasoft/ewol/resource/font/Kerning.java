/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource.font;

/**
 * @notindoc
 * Kerning properties of one specific Glyph with an other
 * 
 * Without Kerning :
 * [pre]
 *                                     
 *        \          /      /\         
 *         \        /      /  \        
 *          \      /      /    \       
 *           \    /      /\      
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
 *           \    /  /\          
 *            \  /  /        \         
 *             \/  /          \        
 *        v        a v        a        
 * [/pre]
 * 
 * @note The "Kerning" is the methode to provide a better display for some string like
 *       the "VA" has 2 letter that overlap themself. This name Kerning
 */
public class Kerning {
	public Character uVal; //!< unicode value (the previous character that must be before)
	public float value; //!< kerning real offset
	
	/**
	 * Simple ructor that allow to allocate the List element
	 */
	public Kerning() {
		this.uVal = 0;
		this.value = 0;
	}
	
	/**
	 * Normal ructor
	 * @param charcode The Unicode value of the coresponding character that might be before
	 * @param value The Kerning value of the offset (nb pixel number)
	 */
	public Kerning(final Character charcode, final float value) {
		this.uVal = charcode;
		this.value = value;
	}
}

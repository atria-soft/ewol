/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_FONT_KERNING_H__
#define __EWOL_FONT_KERNING_H__

namespace ewol
{
	/**
	 * @brief Kerning properties of one specific Glyph with an other
	 * @note The "Kerning" is the methode to provide a better display for some string like
	 *       the "VA" has 2 letter that overlap themself. This name Kerning
	 * 
	 * Without Kerning :
	 *                                     
	 *        \          /      /\         
	 *         \        /      /  \        
	 *          \      /      /    \       
	 *           \    /      /______\      
	 *            \  /      /        \     
	 *             \/      /          \    
	 *        v          v a          a    
	 *
	 * With Kerning :
	 *                                     
	 *        \          /  /\             
	 *         \        /  /  \            
	 *          \      /  /    \           
	 *           \    /  /______\          
	 *            \  /  /        \         
	 *             \/  /          \        
	 *        v        a v        a        
	 * 
	 */
	class Kerning
	{
		public:
			uniChar_t m_UVal;  //!< unicode value (the previous character that must be before)
			float     m_value; //!< kerning real offset
		public:
			/**
			 * @brief Simple constructor that allow to allocate the etk::Vector element
			 */
			Kerning(void) :
				m_UVal(0),
				m_value(0)
			{ };
			/**
			 * @brief Normal constructor
			 * @param[in] charcode The Unicode value of the coresponding character that might be before
			 * @param[in] value The Kerning value of the offset (nb pixel number)
			 */
			Kerning(const uniChar_t charcode, const float value) :
				m_UVal(charcode),
				m_value(value)
			{ };
			/**
			 * @brief normal destructor
			 */
			~Kerning(void) { };
	};
};

#endif


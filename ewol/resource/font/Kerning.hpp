/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

namespace ewol {
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
	class Kerning {
		public:
			char32_t m_UVal;  //!< unicode value (the previous character that must be before)
			float     m_value; //!< kerning real offset
		public:
			/**
			 * @brief Simple constructor that allow to allocate the std::vector element
			 */
			Kerning() :
			  m_UVal(0),
			  m_value(0) {
				
			};
			/**
			 * @brief Normal constructor
			 * @param[in] _charcode The Unicode value of the coresponding character that might be before
			 * @param[in] _value The Kerning value of the offset (nb pixel number)
			 */
			Kerning(const char32_t _charcode, const float _value) :
			  m_UVal(_charcode),
			  m_value(_value) {
				
			};
			/**
			 * @brief normal destructor
			 */
			~Kerning() { };
	};
};


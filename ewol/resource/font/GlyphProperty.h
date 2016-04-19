/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ewol/resource/font/Kerning.h>

namespace ewol {
	/*
	                             |            |          |            |            
	                             |            |          |            |            
	                             |            |          |            |            
	                      Y      |            |          |            |            
	                      ^      |------------|          |------------|            
	                      |                                                        
	     m_advance.y:/->  |                                                        
	                 |    |                                                        
	                 |    |                                                        
	m_sizeTex.x/->   |    |         |------------|          |------------|         
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
	m_bearing.y           |                                                        
	                      |____*________________________*____________>>   X        
	                                                                               
	                                                                               
	                           <------------------------> : m_advance.x            
	                                                                               
	                                <------------> : m_sizeTexture.x               
	                                                                               
	                           <---> : m_bearing.x                                 
	                       
	*/
	/**
	 * @not_in_doc
	 */
	class GlyphProperty {
		public:
			char32_t m_UVal; //!< Unicode value
		public:
			bool m_exist;
		public:
			int32_t m_glyphIndex; //!< Glyph index in the system
			ivec2 m_sizeTexture; //!< size of the element to display
			ivec2 m_bearing; //!< offset to display the data (can be negatif id the texture sise is bigger than the theoric places in the string)
			ivec2 m_advance; //!< space use in the display for this specific char
			vec2 m_texturePosStart; //!< Texture normalized position (START)
			vec2 m_texturePosSize; //!< Texture normalized position (SIZE)
		private:
			std::vector<ewol::Kerning> m_kerning; //!< kerning values of link of all elements
		public:
			GlyphProperty() :
			  m_UVal(0),
			  m_exist(true),
			  m_glyphIndex(0),
			  m_sizeTexture(0,0),
			  m_bearing(0,0),
			  m_advance(0,0),
			  m_texturePosStart(0,0),
			  m_texturePosSize(0,0) {
				
			};
			float kerningGet(const char32_t _charcode) {
				for(size_t iii=0; iii<m_kerning.size(); iii++ ) {
					if (m_kerning[iii].m_UVal == _charcode) {
						return m_kerning[iii].m_value;
					}
				}
				return 0;
			};
			void kerningAdd(const char32_t _charcode, float _value) {
				m_kerning.push_back(ewol::Kerning(_charcode, _value));
			};
			void kerningClear() {
				m_kerning.clear();
			};
			/**
			 * @brief get the status of the char, if it exist or not in the FONT
			 * @return true if the char is availlable, false otherwise
			 */
			bool exist() const {
				return m_exist;
			};
			/**
			 * @brief set the element doen not exist !!!
			 */
			void setNotExist() {
				m_exist = false;
			};
	};
};


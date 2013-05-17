/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_FONT_GLYPH_PROPERTY_H__
#define __EWOL_FONT_GLYPH_PROPERTY_H__

#include <ewol/renderer/resources/font/Kerning.h>


namespace ewol
{
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
	
	class GlyphProperty
	{
		public:
			uniChar_t m_UVal; //!< Unicode value
			int32_t m_glyphIndex; //!< Glyph index in the system
			ivec2 m_sizeTexture; //!< size of the element to display
			ivec2 m_bearing; //!< offset to display the data (can be negatif id the texture sise is bigger than the theoric places in the string)
			ivec2 m_advance; //!< space use in the display for this specific char
			vec2 m_texturePosStart; //!< Texture normalised position (START)
			vec2 m_texturePosStop; //!< Texture normalised position (STOP)
		private:
			etk::Vector<ewol::Kerning> m_kerning; //!< kerning values of link of all elements
		public:
			GlyphProperty(void) :
				m_UVal(0),
				m_glyphIndex(0),
				m_sizeTexture(0,0),
				m_bearing(0,0),
				m_advance(0,0),
				m_texturePosStart(0,0),
				m_texturePosStop(0,0)
			{ };
			float KerningGet(const uniChar_t charcode)
			{
				for(int32_t iii=0; iii<m_kerning.Size(); iii++ ) {
					if (m_kerning[iii].m_UVal == charcode) {
						return m_kerning[iii].m_value;
					}
				}
				return 0;
			}
			void KerningAdd(const uniChar_t charcode, float value)
			{
				m_kerning.PushBack(ewol::Kerning(charcode, value));
			}
			void KerningClear(void)
			{
				m_kerning.Clear();
			}
	};
};

#endif


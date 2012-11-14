/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_FONT_GLYPH_PROPERTY_H__
#define __EWOL_FONT_GLYPH_PROPERTY_H__

#include <ewol/font/Kerning.h>


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
			uniChar_t              m_UVal;            //!< Unicode value
			int32_t                m_glyphIndex;      //!< Glyph index in the system
			etk::Vector2D<int32_t> m_sizeTexture;     //!< size of the element to display
			etk::Vector2D<int32_t> m_bearing;         //!< offset to display the data (can be negatif id the texture sise is bigger than the theoric places in the string)
			etk::Vector2D<int32_t> m_advance;         //!< space use in the display for this specific char
			texCoord_ts            m_texturePosStart; //!< Texture normalised position (START)
			texCoord_ts            m_texturePosStop;  //!< Texture normalised position (STOP)
		private:
			etk::Vector<ewol::Kerning>   m_kerning;     //!< kerning values of link of all elements
		public:
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


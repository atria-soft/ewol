/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_DISTANT_FIELD_FONT_H__
#define __EWOL_DISTANT_FIELD_FONT_H__

#include <ewol/font/Font.h>
#include <ewol/texture/Texture.h>
#include <ewol/Resource.h>

namespace ewol
{
	class DistantFieldFont : public ewol::Texture {
		
		typedef struct {
			GlyphProperty    property;
			texCoord_ts      posStart;
			texCoord_ts      posStop;
		}freeTypeFontElement_ts;
		private:
			int32_t                             m_size;
			int32_t                             m_height;
			ewol::Font*                         m_font;
			etk::Vector<freeTypeFontElement_ts> m_listElement;
			// for the texture generation :
			etk::Vector2D<int32_t>                   m_lastGlyphPos;
			int32_t                             m_lastRawHeigh;
		public:
			DistantFieldFont(etk::UString fontName);
			~DistantFieldFont(void);
			virtual bool HasName(etk::UString& fileName);
			const char* GetType(void) { return "ewol::TexturedFont"; };
			int32_t getFontSize(void) { return m_size; };
			int32_t Draw(etk::Vector2D<float>                 textPos,
			             const etk::UString&             unicodeString,
			             etk::Vector<etk::Vector2D<float> > & coord,
			             etk::Vector<texCoord_ts> &      coordTex);
			int32_t Draw(etk::Vector2D<float>                 textPos,
			             const uniChar_t                 unicodeChar,
			             etk::Vector<etk::Vector2D<float> > & coord,
			             etk::Vector<texCoord_ts> &      coordTex);
			etk::Vector2D<float> GetSize(const etk::UString & unicodeString);
			etk::Vector2D<float> GetSize(const uniChar_t      unicodeChar);
			// TODO : Remove this element, it is stupid ...
			int32_t GetHeight(void) { return m_height; };
			int32_t GetFontSize(void) { return m_size; };
	};
	
	
};

#endif


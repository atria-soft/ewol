/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_TEXTURED_FONT_H__
#define __EWOL_TEXTURED_FONT_H__

#include <ewol/font/Font.h>
#include <ewol/texture/Texture.h>
#include <ewol/Resource.h>

namespace ewol
{
	namespace font
	{
		typedef enum {
			Regular=0,
			Italic,
			Bold,
			BoldItalic,
		} mode_te;
	};
	class TexturedFont : public ewol::Texture {
		
		typedef struct {
			GlyphProperty    property;
			texCoord_ts      posStart;
			texCoord_ts      posStop;
		}freeTypeFontElement_ts;
		private:
			etk::UString                        m_fileName[4];
			int32_t                             m_size;
			int32_t                             m_height[4];
			// specific element to have the the know if the specify element is known...
			// ==> otherwise I can just generate italic ...
			// ==> Bold is a little more complicated (maybe with the bordersize)
			ewol::Font*                         m_font[4];
			etk::Vector<freeTypeFontElement_ts> m_listElement[4];
			// for the texture generation :
			etk::Vector2D<int32_t>              m_lastGlyphPos[4];
			int32_t                             m_lastRawHeigh[4];
		public:
			TexturedFont(etk::UString fontName);
			~TexturedFont(void);
			virtual bool HasName(etk::UString& fileName);
			const char* GetType(void) { return "ewol::TexturedFont"; };
			int32_t getFontSize(void) { return m_size; };
			int32_t Draw(etk::Vector2D<float>                 textPos,
			             const etk::UString&                  unicodeString,
			             etk::Vector<etk::Vector2D<float> > & coord,
			             etk::Vector<texCoord_ts> &           coordTex,
			             etk::Vector<int32_t> &               vectDisplayMode,
			             bool                                 hasClipping,
			             clipping_ts&                         clipping,
			             ewol::font::mode_te                  displayMode);
			
			int32_t Draw(etk::Vector2D<float>                 textPos,
			             const uniChar_t                      unicodeChar,
			             etk::Vector<etk::Vector2D<float> > & coord,
			             etk::Vector<texCoord_ts> &           coordTex,
			             etk::Vector<int32_t> &               vectDisplayMode,
			             bool                                 hasClipping,
			             clipping_ts&                         clipping,
			             ewol::font::mode_te                  displayMode);
			
			etk::Vector2D<float> GetSize(const etk::UString & unicodeString);
			etk::Vector2D<float> GetSize(const uniChar_t      unicodeChar);
			// TODO : Remove this element, it is stupid ...
			int32_t GetHeight(void) { return m_height[0]; };
			int32_t GetFontSize(void) { return m_size; };
	};
	
	
};

#endif


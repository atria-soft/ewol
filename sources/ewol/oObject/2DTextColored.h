/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#ifndef __EWOL_O_OBJECT_2D_TEXT_COLORED_H__
#define __EWOL_O_OBJECT_2D_TEXT_COLORED_H__

#include <ewol/oObject/OObject.h>
#include <ewol/font/TexturedFont.h>
#include <ewol/ResourceManager.h>

namespace ewol {
	class OObject2DTextColored :public ewol::OObject
	{
		public:
			OObject2DTextColored(etk::UString FontName, int32_t size);
			OObject2DTextColored(void);
			virtual ~OObject2DTextColored(void);
		public:
			virtual void Draw(void);
			void SetColor(float red, float green, float blue, float alpha = 1.0);
			void SetColor(draw::Color color);
			// set a specific text
			void    Clear(void);
			int32_t Text(etk::Vector2D<float>  textPos, const etk::UString& unicodeString, ewol::font::mode_te displayMode = ewol::font::Regular);
			int32_t Text(etk::Vector2D<float>  textPos, const uniChar_t     unicodeChar, ewol::font::mode_te displayMode = ewol::font::Regular);
		protected:
			ewol::Program* m_GLprogram;
			int32_t        m_GLPosition;
			int32_t        m_GLMatrix;
			int32_t        m_GLColor;
			int32_t        m_GLtextMode;
			int32_t        m_GLtexture;
			int32_t        m_GLtexID;
			ewol::TexturedFont*                  m_font;          //!< ewol font system
			draw::Color                          m_color;         //!< tmp text color ...
			etk::Vector<etk::Vector2D<float> >   m_coord;         //!< internal coord of the object
			etk::Vector<texCoord_ts>             m_coordTex;      //!< internal texture coordinate for every point
			etk::Vector<draw::Colorf>            m_coordColor;    //!< internal color of the different point
			etk::Vector<int32_t>                 m_displayMode;   //!< Display mode availlable (bold/...)
		public:
			void SetFont(etk::UString fontName);
			void SetSize(int32_t fontSize);
			void SetFontProperty(etk::UString fontName, int32_t fontSize);
			int32_t GetHeight(void) { return (NULL!=m_font)?m_font->GetHeight():10; };
			etk::Vector2D<float> GetSize(const uniChar_t unicodeChar);
			etk::Vector2D<float> GetSize(const etk::UString& unicodeString);
	};
};

#endif


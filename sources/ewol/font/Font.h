/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_FONT_H__
#define __EWOL_FONT_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <draw/Image.h>
#include <ewol/texture/Texture.h>
#include <ewol/Resource.h>
#include <ewol/font/GlyphProperty.h>


namespace ewol
{
	class Font : public ewol::Resource
	{
		public:
			Font(etk::UString fontName) : ewol::Resource(fontName) {};
			
			virtual ~Font(void) {};
			
			const char* GetType(void) { return "ewol::Font"; };
			
			virtual bool GetGlyphProperty(int32_t              fontSize,
			                              ewol::GlyphProperty& property) = 0;
			
			virtual bool DrawGlyph(draw::Image&         imageOut,
			                       int32_t              fontSize,
			                       etk::Vector2D<int32_t>    glyphPosition,
			                       ewol::GlyphProperty& property,
			                       int8_t posInImage) = 0;
			
			virtual etk::Vector2D<float> GetSize(int32_t fontSize, const etk::UString &  unicodeString) = 0;
			
			virtual int32_t GetHeight(int32_t fontSize) = 0;
			
			virtual void GenerateKerning(int32_t fontSize, etk::Vector<ewol::GlyphProperty>& listGlyph) { };
			
			virtual void Display(void) {};
	};
};

#endif


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
		
		private:
			etk::UString                        m_fileName[4];
			int32_t                             m_size;
			int32_t                             m_height[4];
			// specific element to have the the know if the specify element is known...
			// ==> otherwise I can just generate italic ...
			// ==> Bold is a little more complicated (maybe with the bordersize)
			ewol::Font*                         m_font[4];
		public:
			etk::Vector<GlyphProperty>          m_listElement[4];
		private:
			// for the texture generation :
			etk::Vector2D<int32_t>              m_lastGlyphPos[4];
			int32_t                             m_lastRawHeigh[4];
		public:
			TexturedFont(etk::UString fontName);
			~TexturedFont(void);
			virtual bool HasName(etk::UString& fileName);
			const char* GetType(void) { return "ewol::TexturedFont"; };
			int32_t getFontSize(void) { return m_size; };
			// TODO : Remove : DEPRECATED ...
			int32_t Draw(etk::Vector2D<float>                 textPos,
			             const etk::UString&                  unicodeString,
			             etk::Vector<etk::Vector2D<float> > & coord,
			             etk::Vector<texCoord_ts> &           coordTex,
			             etk::Vector<int32_t> &               vectDisplayMode,
			             bool                                 hasClipping,
			             clipping_ts&                         clipping,
			             ewol::font::mode_te                  displayMode);
			
			// TODO : Remove : DEPRECATED ...
			int32_t Draw(etk::Vector2D<float>                 textPos,
			             const uniChar_t                      unicodeChar,
			             etk::Vector<etk::Vector2D<float> > & coord,
			             etk::Vector<texCoord_ts> &           coordTex,
			             etk::Vector<int32_t> &               vectDisplayMode,
			             bool                                 hasClipping,
			             clipping_ts&                         clipping,
			             ewol::font::mode_te                  displayMode,
			             const uniChar_t                      unicodeCharPrevious = 0);
			
			// TODO : Remove : DEPRECATED ...
			/**
			 * @brief Get the size of the specified String
			 * @param[in] unicodeString The string that we might calculate the display size
			 * @param[in] displayMode Mode to display the currrent font
			 * @return Dimention of the string use
			 */
			etk::Vector2D<float> GetSize(const etk::UString & unicodeString,
			                             const ewol::font::mode_te displayMode = ewol::font::Regular);
			
			// TODO : Remove : DEPRECATED ...
			/**
			 * @brief Get the size of the specified unicode value
			 * @param[in] unicodeChar the char that might be displayed
			 * @param[in] unicodeCharPrevious Previous char of this One ==> 0 no previous char
			 * @param[in] displayMode Mode to display the currrent font
			 * @return Dimention of the font use
			 */
			etk::Vector2D<float> GetSize(const uniChar_t unicodeChar,
			                             const uniChar_t unicodeCharPrevious = 0,
			                             const ewol::font::mode_te displayMode = ewol::font::Regular);
			
			/**
			 * @brief Get the display height of this font
			 * @param[in] displayMode Mode to display the currrent font
			 * @return Dimention of the font need between 2 lines
			 */
			int32_t GetHeight(const ewol::font::mode_te displayMode = ewol::font::Regular) { return m_height[displayMode]; };
			/**
			 * @brief Get the font height (user friendly)
			 * @return Dimention of the font the user requested
			 */
			int32_t GetFontSize(void) { return m_size; };
			/**
			 * @brief Get the ID of a unicode charcode
			 * @param[in] charcode The unicodeValue
			 * @param[in] displayMode Mode to display the currrent font
			 * @return The ID in the table (if it does not exist : return 0)
			 */
			int32_t GetIndex(const uniChar_t charcode, const ewol::font::mode_te displayMode) const;
			/**
			 * @brief Get the pointer on the coresponding glyph
			 * @param[in] charcode The unicodeValue
			 * @param[in] displayMode Mode to display the currrent font
			 * @return The pointer on the glyph ==> never NULL
			 */
			ewol::GlyphProperty* GetGlyphPointer(const uniChar_t charcode, const ewol::font::mode_te displayMode);
	};
	
	
};

#endif


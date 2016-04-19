/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ewol/resource/font/FontBase.h>
#include <ewol/resource/Texture.h>

namespace ewol {
	namespace font {
		/**
		 * @not_in_doc
		 */
		enum mode {
			Regular=0,
			Italic,
			Bold,
			BoldItalic,
		};
	}
	std::ostream& operator <<(std::ostream& _os, enum ewol::font::mode _obj);
	
	namespace resource {
		class TexturedFont : public ewol::resource::Texture {
			private:
				std::string m_fileName[4];
				int32_t m_size;
				int32_t m_height[4];
				// specific element to have the the know if the specify element is known...
				//  == > otherwise I can just generate italic ...
				//  == > Bold is a little more complicated (maybe with the bordersize)
				std::shared_ptr<ewol::resource::FontBase> m_font[4];
				enum ewol::font::mode m_modeWraping[4]; //!< This is a wrapping mode to prevent the fact that no font is define for a specific mode
			public:
				std::vector<GlyphProperty> m_listElement[4];
			private:
				// for the texture generation :
				ivec2 m_lastGlyphPos[4];
				int32_t m_lastRawHeigh[4];
			protected:
				TexturedFont();
				void init(const std::string& _fontName);
			public:
				DECLARE_RESOURCE_NAMED_FACTORY(TexturedFont);
				virtual ~TexturedFont();
			public:
				/**
				 * @brief get the display height of this font
				 * @param[in] _displayMode Mode to display the currrent font
				 * @return Dimention of the font need between 2 lines
				 */
				int32_t getHeight(const enum ewol::font::mode _displayMode = ewol::font::Regular) {
					return m_height[_displayMode];
				};
				/**
				 * @brief get the font height (user friendly)
				 * @return Dimention of the font the user requested
				 */
				int32_t getFontSize() {
					return m_size;
				};
				/**
				 * @brief get the ID of a unicode charcode
				 * @param[in] _charcode The unicodeValue
				 * @param[in] _displayMode Mode to display the currrent font
				 * @return The ID in the table (if it does not exist : return 0)
				 */
				int32_t getIndex(char32_t _charcode, const enum ewol::font::mode _displayMode);
				/**
				 * @brief get the pointer on the coresponding glyph
				 * @param[in] _charcode The unicodeValue
				 * @param[in] _displayMode Mode to display the currrent font
				 * @return The pointer on the glyph  == > never nullptr
				 */
				ewol::GlyphProperty* getGlyphPointer(const char32_t& _charcode, const enum ewol::font::mode _displayMode);
				/**
				 * @brief The wrapping mode is used to prevent the non existance of a specific mode.
				 *        For exemple when a blod mode does not exist, this resend a regular mode.
				 * @param[in] _source The requested mode.
				 * @return the best mode we have in stock.
				 */
				enum ewol::font::mode getWrappingMode(const enum ewol::font::mode _source) {
					return m_modeWraping[_source];
				};
			private:
				/**
				 * @brief add a glyph in a texture font.
				 * @param[in] _val Char value to add.
				 * @return true if the image size have change, false otherwise
				 */
				bool addGlyph(const char32_t& _val);
		};
	}
}


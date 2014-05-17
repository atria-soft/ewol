/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_TEXTURED_FONT_H__
#define __EWOL_TEXTURED_FONT_H__

#include <ewol/resource/font/FontBase.h>
#include <ewol/resource/Texture.h>
#include <ewol/resource/Resource.h>

namespace ewol {
	namespace font {
		/**
		 * @not-in-doc
		 */
		enum mode {
			Regular=0,
			Italic,
			Bold,
			BoldItalic,
		};
	};
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
				ewol::resource::FontBase* m_font[4];
				enum ewol::font::mode m_modeWraping[4]; //!< This is a wrapping mode to prevent the fact that no font is define for a specific mode
			public:
				std::vector<GlyphProperty> m_listElement[4];
			private:
				// for the texture generation :
				ivec2 m_lastGlyphPos[4];
				int32_t m_lastRawHeigh[4];
			protected:
				TexturedFont(const std::string& _fontName);
				~TexturedFont();
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
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the texture font.
				 * @return pointer on the resource or nullptr if an error occured.
				 */
				static ewol::resource::TexturedFont* keep(const std::string& _filename);
				/**
				 * @brief release the keeped resources
				 * @param[in,out] reference on the object pointer
				 */
				static void release(ewol::resource::TexturedFont*& _object);
			private:
				/**
				 * @brief add a glyph in a texture font.
				 * @param[in] _val Char value to add.
				 * @return true if the image size have change, false otherwise
				 */
				bool addGlyph(const char32_t& _val);
		};
	};
};

#endif


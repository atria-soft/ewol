/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/resource/font/FontBase.hpp>
#include <ewol/resource/Texture.hpp>

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
	etk::Stream operator +(etk::Stream _os, enum ewol::font::mode _obj);
	
	namespace resource {
		class TexturedFont : public ewol::resource::Texture {
			private:
				etk::Uri this.fileName[4];
				int this.size;
				int this.height[4];
				// specific element to have the the know if the specify element is known...
				//  == > otherwise I can just generate italic ...
				//  == > Bold is a little more complicated (maybe with the bordersize)
				ememory::Ptr<ewol::resource::FontBase> this.font[4];
				enum ewol::font::mode this.modeWraping[4]; //!< This is a wrapping mode to prevent the fact that no font is define for a specific mode
			public:
				GlyphProperty this.emptyGlyph;
				List<GlyphProperty> this.listElement[4];
			private:
				// for the texture generation :
				Vector2i this.lastGlyphPos[4];
				int this.lastRawHeigh[4];
			protected:
				TexturedFont();
				void init( String _fontName);
			public:
				DECLARE_RESOURCE_NAMED_FACTORY(TexturedFont);
				 ~TexturedFont();
			public:
				/**
				 * @brief get the display height of this font
				 * @param[in] _displayMode Mode to display the currrent font
				 * @return Dimention of the font need between 2 lines
				 */
				int getHeight( enum ewol::font::mode _displayMode = ewol::font::Regular) {
					return this.height[_displayMode];
				};
				/**
				 * @brief get the font height (user friendly)
				 * @return Dimention of the font the user requested
				 */
				int getFontSize() {
					return this.size;
				};
				/**
				 * @brief get the ID of a unicode charcode
				 * @param[in] _charcode The unicodeValue
				 * @param[in] _displayMode Mode to display the currrent font
				 * @return The ID in the table (if it does not exist : return 0)
				 */
				int getIndex(Character _charcode,  enum ewol::font::mode _displayMode);
				/**
				 * @brief get the pointer on the coresponding glyph
				 * @param[in] _charcode The unicodeValue
				 * @param[in] _displayMode Mode to display the currrent font
				 * @return The pointer on the glyph  == > never null
				 */
				ewol::GlyphProperty* getGlyphPointer( Character _charcode,  enum ewol::font::mode _displayMode);
				/**
				 * @brief The wrapping mode is used to prevent the non existance of a specific mode.
				 *        For exemple when a blod mode does not exist, this resend a regular mode.
				 * @param[in] _source The requested mode.
				 * @return the best mode we have in stock.
				 */
				enum ewol::font::mode getWrappingMode( enum ewol::font::mode _source) {
					return this.modeWraping[_source];
				};
			private:
				/**
				 * @brief add a glyph in a texture font.
				 * @param[in] _val Char value to add.
				 * @return true if the image size have change, false otherwise
				 */
				boolean addGlyph( Character _val);
		};
	}
}


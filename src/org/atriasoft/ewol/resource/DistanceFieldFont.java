/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/resource/font/FontBase.hpp>
#include <ewol/resource/Texture.hpp>
#include <ewol/resource/TexturedFont.hpp>

namespace ewol {
	namespace resource {
		class DistanceFieldFont : public ewol::resource::Texture {
			private:
				etk::Uri this.fileName;
				float this.sizeRatio;
				// specific element to have the the know if the specify element is known...
				//  == > otherwise I can just generate italic ...
				//  == > Bold is a little more complicated (maybe with the bordersize)
				ememory::Ptr<ewol::resource::FontBase> this.font;
			public:
				List<GlyphProperty> this.listElement;
			private:
				// for the texture generation :
				Vector2i this.lastGlyphPos;
				int this.lastRawHeigh;
			protected:
				DistanceFieldFont();
				void init( String _fontName);
			public:
				DECLARE_RESOURCE_NAMED_FACTORY(DistanceFieldFont);
				 ~DistanceFieldFont();
			public:
				float getDisplayRatio(float _size);
				/**
				 * @brief get the display height of this font
				 * @param[in] _size Request font size
				 * @return Dimention of the font need between 2 lines
				 */
				float getHeight(float _size) {
					return ((float)this.font.getHeight(_size));
				};
				/**
				 * @brief get the font size with a specific display size
				 * @param[in] _fontHeight Request font height
				 * @return Dimention of the font for this compleate line size.
				 */
				float getSize(float _fontHeight) {
					return this.font.getSizeWithHeight(_fontHeight);
				}
				/**
				 * @brief get the ID of a unicode charcode
				 * @param[in] _charcode The unicodeValue
				 * @return The ID in the table (if it does not exist : return 0)
				 */
				int getIndex(Character _charcode);
				/**
				 * @brief get the pointer on the coresponding glyph
				 * @param[in] _charcode The unicodeValue
				 * @return The pointer on the glyph  == > never null
				 */
				ewol::GlyphProperty* getGlyphPointer( Character _charcode);
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the texture font.
				 * @return pointer on the resource or null if an error occured.
				 */
				static ememory::Ptr<ewol::resource::DistanceFieldFont> keep( String _filename);
			private:
				/**
				 * @brief add a glyph in a texture font.
				 * @param[in] _val Char value to add.
				 * @return true if the image size have change, false otherwise
				 */
				boolean addGlyph( Character _val);
				
				void generateDistanceField( egami::ImageMono _input, egami::Image _output);
			private:
				float this.borderSize; //!< number of pixel added on the border of a glyph
				Vector2f this.textureBorderSize; //!< Transformed the border size in the texture dimention
			public:
				float getPixelBorderSize() {
					return this.borderSize;
				}
				 Vector2f getTextureBorderSize() {
					return this.textureBorderSize;
				}
			public:
				void exportOnFile();
				boolean importFromFile();
		};
	};
};

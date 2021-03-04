/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <egami/Image.hpp>
#include <egami/ImageMono.hpp>
#include <ewol/resource/Texture.hpp>

namespace ewol {
	namespace resource {
		class ImageDF : public ewol::resource::Texture {
			protected:
				Vector2f this.realImageSize;
			protected:
				ImageDF();
				void init();
				void init(String _genName,  etk::Uri _uri,  Vector2i _size);
			public:
				 ~ImageDF() { };
			protected:
				/**
				 * @brief Generate distance field of this Image input.
				 * @param[in] _input Input image to change in distance field mode.
				 * @param[out] _output New image generate with this image _input.
				 */
				void generateDistanceField( egami::ImageMono _input, egami::Image _output);
			public:
				 Vector2f getRealSize() {
					return this.realImageSize;
				};
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the image file.
				 * @param[in] _requested size of the image (usefull when loading .svg to automatic rescale)
				 * @return pointer on the resource or null if an error occured.
				 */
				static ememory::Ptr<ewol::resource::ImageDF> create( etk::Uri _uri, Vector2i _size=Vector2i(-1,-1));
		};
	};
};


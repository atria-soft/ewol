/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
// TODO : Change tis file name ...
#pragma once

#include <etk/types.hpp>
#include <etk/types.hpp>
#include <egami/Image.hpp>
#include <ewol/resource/Texture.hpp>



namespace ewol {
	namespace resource {
		class TextureFile : public ewol::resource::Texture {
			public:
				static const ivec2 sizeAuto;
				static const ivec2 sizeDefault;
			protected:
				TextureFile();
				void init();
				void init(etk::String _genName, const etk::String& _fileName, const ivec2& _size);
			public:
				virtual ~TextureFile() { };
			public:
				const vec2& getRealSize() {
					return m_realImageSize;
				};
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the image file.
				 * @param[in] _requested size of the image (usefull when loading .svg to automatic rescale)
				 * @param[in] _sizeRegister size register in named (When you preaload the images the size write here will be )
				 * @return pointer on the resource or null if an error occured.
				 */
				static ememory::SharedPtr<ewol::resource::TextureFile> create(const etk::String& _filename,
				                                                           ivec2 _size=ewol::resource::TextureFile::sizeAuto,
				                                                           ivec2 _sizeRegister=ewol::resource::TextureFile::sizeAuto);
		};
	};
};


/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_RESOURCE_IMAGE_DISTANCE_FIELD_H__
#define __EWOL_RESOURCE_IMAGE_DISTANCE_FIELD_H__

#include <etk/types.h>
#include <etk/types.h>
#include <egami/Image.h>
#include <egami/ImageMono.h>
#include <ewol/resource/Texture.h>
#include <ewol/resource/Resource.h>

namespace ewol {
	namespace resource {
		class ImageDF : public ewol::resource::Texture {
			protected:
				vec2 m_realImageSize;
			protected:
				ImageDF(const std::string& _genName);
				ImageDF(std::string _genName, const std::string& _fileName, const ivec2& _size);
			public:
				virtual ~ImageDF() { };
			protected:
				/**
				 * @brief Generate distance field of this Image input.
				 * @param[in] _input Input image to change in distance field mode.
				 * @param[out] _output New image generate with this image _input.
				 */
				void generateDistanceField(const egami::ImageMono& _input, egami::Image& _output);
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
				 * @return pointer on the resource or nullptr if an error occured.
				 */
				static ewol::object::Shared<ewol::resource::ImageDF> keep(const std::string& _filename, ivec2 _size=ivec2(-1,-1));
		};
	};
};


#endif
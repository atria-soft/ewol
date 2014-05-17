/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_TEXTURE_FILE_H__
#define __EWOL_TEXTURE_FILE_H__

#include <etk/types.h>
#include <etk/types.h>
#include <egami/Image.h>
#include <ewol/resource/Texture.h>
#include <ewol/resource/Resource.h>

namespace ewol {
	namespace resource {
		class TextureFile : public ewol::resource::Texture {
			private:
				vec2 m_realImageSize;
			private:
				TextureFile(const std::string& _genName);
				TextureFile(std::string _genName, const std::string& _fileName, const ivec2& _size);
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
				 * @return pointer on the resource or nullptr if an error occured.
				 */
				static ewol::resource::TextureFile* keep(const std::string& _filename, ivec2 _size=ivec2(-1,-1));
				/**
				 * @brief release the keeped resources
				 * @param[in,out] reference on the object pointer
				 */
				static void release(ewol::resource::TextureFile*& _object);
		};
	};
};


#endif


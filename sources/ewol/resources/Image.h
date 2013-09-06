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
#include <etk/UString.h>
#include <egami/Image.h>
#include <ewol/resources/Texture.h>
#include <ewol/resources/Resource.h>

namespace ewol
{
	class TextureFile : public ewol::Texture
	{
		private:
			vec2 m_realImageSize;
		private:
			TextureFile(const etk::UString& _genName);
			TextureFile(etk::UString _genName, const etk::UString& _fileName, const ivec2& _size);
			~TextureFile(void) { };
		public:
			virtual const char* GetType(void) { return "ewol::TextureFile"; };
			const vec2& GetRealSize(void) { return m_realImageSize; };
		public:
			/**
			 * @brief Keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the image file.
			 * @param[in] _requested size of the image (usefull when loading .svg to automatic rescale)
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static ewol::TextureFile* Keep(const etk::UString& _filename, ivec2 _size=ivec2(-1,-1));
			/**
			 * @brief Release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void Release(ewol::TextureFile*& _object);
	};
};


#endif


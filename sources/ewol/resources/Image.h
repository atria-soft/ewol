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
		public:
			TextureFile(const etk::UString& _genName);
			TextureFile(etk::UString _genName, const etk::UString& _fileName, const ivec2& _size);
			~TextureFile(void) { };
			virtual const char* GetType(void) { return "ewol::TextureFile"; };
			const vec2& GetRealSize(void) { return m_realImageSize; };
	};
};


#endif


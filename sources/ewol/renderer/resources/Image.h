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
#include <draw/Image.h>
#include <ewol/renderer/resources/Texture.h>
#include <ewol/renderer/resources/Resource.h>

namespace ewol
{
	class TextureFile : public ewol::Texture
	{
		public:
			TextureFile(etk::UString genName, etk::UString fileName, ivec2 size);
			~TextureFile(void) { };
			virtual const char* GetType(void) { return "ewol::TextureFile"; };
	};
};


#endif


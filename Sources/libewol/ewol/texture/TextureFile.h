/**
 *******************************************************************************
 * @file ewol/texture/TextureFile.cpp
 * @brief ewol tecture file (sources)
 * @author Edouard DUPIN
 * @date 22/08/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_TEXTURE_FILE_H__
#define __EWOL_TEXTURE_FILE_H__

#include <etk/UString.h>
#include <draw/Image.h>
#include <ewol/texture/Texture.h>
#include <ewol/Resource.h>

namespace ewol
{
	class TextureFile : public ewol::Texture
	{
		public:
			TextureFile(etk::UString genName, etk::UString fileName, Vector2D<int32_t> size);
			~TextureFile(void) { };
			virtual const char* GetType(void) { return "ewol::TextureFile"; };
	};
};


#endif


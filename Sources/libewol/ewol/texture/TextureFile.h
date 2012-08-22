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

namespace ewol
{
	class TextureFile
	{
		private:
			uint32_t      m_counter;
			ewol::Texture m_texture;
			etk::UString  m_fileName;
		public:
			TextureFile(etk::UString fileName, Vector2D<int32_t> size);
			~TextureFile(void);
			bool HasName(etk::UString& fileName) { return fileName==m_fileName; };
			void Increment(void) { m_counter++; };
			bool Decrement(void) { m_counter--; return (m_counter==0)?true:false; };
			ewol::Texture& Get(void) { return m_texture; };
	};
};


#endif


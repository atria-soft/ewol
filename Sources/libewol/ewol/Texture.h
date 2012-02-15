/**
 *******************************************************************************
 * @file ewol/Texture.h
 * @brief ewol Texture loading system (header)
 * @author Edouard DUPIN
 * @date 28/10/2011
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

#ifndef __EWOL_TEXTURE_H__
#define __EWOL_TEXTURE_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <etk/File.h>

namespace ewol
{
	// TODO : Create a subNameSpace:
	/*
	namespace texture {
		int32_t Load(etk::File fileName);
		void    UnLoad(uint32_t textureID);
		int32_t GetSize(uint32_t textureID);
	};
	*/
	namespace texture {
		void     Init(void);
		void     UnInit(void);
	};
	int32_t  LoadTexture(etk::File fileName);
	int32_t  LoadTexture(int32_t target, int32_t level, int32_t internalFormat, int32_t width, int32_t height, int32_t border, int32_t format, int32_t type, const void* data, int32_t nbBytes, etk::UString filename);
	void     UnLoadTexture(uint32_t textureID);
	int32_t  GetTextureSize(uint32_t textureID);
	uint32_t GetTextureGLID(uint32_t textureID);
	void     UpdateTextureContext(void);
	void     UpdateTextureContextIsDestroy(void);
	void     TextureOGLContext(bool enable);
};

#endif


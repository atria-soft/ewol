/**
 *******************************************************************************
 * @file ewol/texture/Texture.cpp
 * @brief ewol Texture loading system (sources)
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



#include <ewol/texture/Texture.h>
#include <ewol/texture/TextureManager.h>
#include <ewol/openGL/openGL.h>
#include <ewol/ewol.h>



/**
 * @brief get the next power 2 if the input
 * @param[in] value Value that we want the next power of 2
 * @return result value
 */
static int32_t nextP2(int32_t value)
{
	int32_t val=1;
	for (int32_t iii=1; iii<31; iii++) {
		if (value <= val) {
			return val;
		}
		val *=2;
	}
	EWOL_CRITICAL("impossible CASE....");
	return val;
}



ewol::Texture::Texture(void)
{
	// add it to the texture manager
	m_uniqueId = ewol::textureManager::Add(this);
	
	m_loaded = false;
	m_texId = 0;
	m_endPointSize.x = 1.0;
	m_endPointSize.y = 1.0;
}

ewol::Texture::~Texture(void)
{
	// unregister from the texture manager
	ewol::textureManager::Rm(this);
	RemoveContext();
	
}


void ewol::Texture::UpdateContext(void)
{
	if (false == m_loaded) {
		// Request a new texture at OpenGl :
		glGenTextures(1, &m_texId);
		// TODO : check error ???
		glBindTexture(GL_TEXTURE_2D, m_texId);
		// TODO : Check error ???
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//--- mode nearest
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//--- Mode linear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		EWOL_INFO("TEXTURE: Add [" << m_uniqueId << "]=" << m_data.GetSize() << " OGl_Id=" <<m_texId);
		glTexImage2D(GL_TEXTURE_2D, // Target
		             0, // Level
		             GL_RGBA, // Format internal
		             m_data.GetWidth(),
		             m_data.GetHeight(),
		             0, // Border
		             GL_RGBA, // format
		             GL_UNSIGNED_BYTE, // type
		             m_data.GetTextureDataPointer() );
		// now the data is loaded
		m_loaded = true;
	} else {
		EWOL_TODO("UPDATE Texture ...");
	}
}

// really remove the texture
void ewol::Texture::RemoveContext(void)
{
	if (true == m_loaded) {
		// Request remove texture ...
		EWOL_INFO("TEXTURE: Rm [" << m_uniqueId << "] texId=" << m_texId);
		glDeleteTextures(1, &m_texId);
		m_loaded = false;
	}
}

// the OpenGl context is destroy, it is too late to remo it from nothing ... special case for Android
void ewol::Texture::RemoveContextToLate(void)
{
	m_loaded = false;
}


void ewol::Texture::Flush(void)
{
	// request to the manager to be call at the next update ...
	ewol::textureManager::Update(this);
}


void ewol::Texture::SetImageSize(Vector2D<int32_t> newSize)
{
	newSize.x = nextP2(newSize.x);
	newSize.y = nextP2(newSize.y);
	m_data.Resize(newSize);
}



/*
#include <ewol/texture/TextureBMP.h>
#include <ewol/texture/TextureSVG.h>
#include <ewol/texture/TexturePNG.h>
*/



/**
 * @brief Load a specific file texture
 * @note : dimention must be a power of 2, otherwise, the display can be wrong... For the SVG, the texture automaticly generate the power of 2 dimention ...
 * @param[in] fileName File that might be open
 * @param[in] requestedWidth Requested size of the file we desire (if we can not resize it, we load it whit his normal size)
 * @return The Internal ID of the texture, or -1 if an error occured ...
 */
// TODO : Load non square texture ...
// TODO : Check the size to regenerate the texture if the size change
#if 0
int32_t ewol::texture::Load(etk::UString tmpfileName, int32_t requestedWidth)
{
	int32_t outTextureID = -1;
	if (l_listLoadedTexture.Size()!=0) {
		for (int32_t iii=0; iii<l_listLoadedTexture.Size(); iii++) {
			if (NULL != l_listLoadedTexture[iii]) {
				if (l_listLoadedTexture[iii]->m_filename == tmpfileName) {
					l_listLoadedTexture[iii]->m_nbTimeLoaded++;
					// this prevent the removing of the texture while the cycle is not ended ...
					l_listLoadedTexture[iii]->m_destroy = false;
					return iii;
				}
			}
		}
	}
	etk::File fileName(tmpfileName, etk::FILE_TYPE_DATA);
	if (false == fileName.Exist()) {
		EWOL_ERROR("File does not Exist ... " << fileName);
	} else {
		// get the upper paw2 ot the size requested...
		requestedWidth = nextP2(requestedWidth);
		
		etk::UString fileExtention = fileName.GetExtention();
		if (fileExtention ==  "bmp") {
			// create the bitmap texture
			ewol::texture::TextureBMP * myBitmap = new ewol::texture::TextureBMP(fileName);
			// draw bitmap properties
			//myBitmap->Display();
			// check if all is OK
			if (myBitmap->LoadOK() == true) {
				if (myBitmap->Width() != nextP2(myBitmap->Width()) ) {
					EWOL_ERROR("Texture has not the good dimention power of 2 : Width=" << myBitmap->Width() << "px ==> maybe not drawable ...");
				}
				if (myBitmap->Width() != myBitmap->Height()) {
					EWOL_ERROR("Texture can not have Width=" << myBitmap->Width() << "px different of height=" << myBitmap->Height() << "px in file:" << fileName);
					return -1;
				}
				outTextureID = ewol::texture::Load(GL_TEXTURE_2D, 0, GL_RGBA, myBitmap->Width(), myBitmap->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myBitmap->Data(), myBitmap->DataSize(), tmpfileName);
			}
			// removet the bitmap handle
			delete (myBitmap);
		} else if (fileExtention ==  "svg") {
			/*if (requestedWidth < 32) {
				requestedWidth = 32;
			}*/
			// create the bitmap texture
			ewol::texture::TextureSVG * mySvg = new ewol::texture::TextureSVG(fileName, requestedWidth, requestedWidth);
			// draw bitmap properties
			//mySvg->Display();
			// check if all is OK
			if (mySvg->LoadOK() == true) {
				if (mySvg->Width() != mySvg->Height()) {
					EWOL_ERROR("Texture can not have Width=" << mySvg->Width() << "px different of height=" << mySvg->Height() << "px in file:" << fileName);
					return -1;
				}
				outTextureID = ewol::texture::Load(GL_TEXTURE_2D, 0, GL_RGBA, mySvg->Width(), mySvg->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, mySvg->Data(), mySvg->DataSize(), tmpfileName);
			}
			// removet the bitmap handle
			delete (mySvg);
		} else if (fileExtention ==  "png") {
			EWOL_ERROR("Extention not supported now, but soon " << fileName );
		} else {
			EWOL_ERROR("Extention not managed " << fileName << " Sopported extention : .bmp / .svg / .png");
		}
	}
	return outTextureID;
}
#endif

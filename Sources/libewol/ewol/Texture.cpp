/**
 *******************************************************************************
 * @file ewol/Texture.cpp
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



#include <ewol/Texture.h>
#include <ewol/importgl.h>
#include <ewol/ewol.h>
#include <ewol/Texture/TextureBMP.h>
#include <ewol/Texture/TextureSVG.h>
#include <ewol/Texture/TexturePNG.h>

//! One Texture element
class LoadedTexture
{
	public:
		etk::UString m_filename;
		int32_t     m_nbTimeLoaded;
		// openGl configuration : 
		uint32_t    m_openGlTextureID;
		int32_t     m_target;
		int32_t     m_level;
		int32_t     m_internalFormat;
		int32_t     m_width;
		int32_t     m_height;
		int32_t     m_border;
		int32_t     m_format;
		int32_t     m_type;
		char*       m_data;
		int32_t     m_nbBytes;
		bool        m_loaded;
		bool        m_destroy;
};
//! List of all Texture loaded ...
etk::VectorType<LoadedTexture*> l_listLoadedTexture;
#undef __class__
#define __class__	"texture"

static pthread_mutex_t localMutex;

/**
 * @brief Initialise the texture namespace (init a mutex)
 * @param ---
 * @return ---
 */
void ewol::texture::Init(void)
{
	EWOL_DEBUG("==> Init Texture-Manager");
	// create interface mutex :
	int ret = pthread_mutex_init(&localMutex, NULL);
	EWOL_ASSERT(ret == 0, "Error creating Mutex ...");
}


/**
 * @brief Un-Initialise the Texture namespace (Remove all loaded texture and temporary data and remove Mutex)
 * @param ---
 * @return ---
 */
void ewol::texture::UnInit(void)
{
	pthread_mutex_lock(&localMutex);
	EWOL_DEBUG("==> Un-Init Texture-Manager");
	for (int32_t iii=0; iii<l_listLoadedTexture.Size(); iii++) {
		if (l_listLoadedTexture[iii] != NULL) {
			delete(l_listLoadedTexture[iii]);
		}
		l_listLoadedTexture[iii] = NULL;
	}
	l_listLoadedTexture.Clear();
	pthread_mutex_unlock(&localMutex);
	int ret = pthread_mutex_destroy(&localMutex);
	EWOL_ASSERT(ret == 0, "Error destroying Mutex ...");
}


/**
 * @brief Specific for Android, some configuration restart openGl context when the screen is rotate, then, 
 *        Android inform us that the openGl context has been destroy. We mark all the texture like not loaded
 *        To load it again when a new context will be enable (@ref UpdateContext)
 * @param ---
 * @return ---
 */
void ewol::texture::UpdateContextIsDestroy(void)
{
	pthread_mutex_lock(&localMutex);
	for (int32_t iii=0; iii < l_listLoadedTexture.Size(); iii++) {
		if(    NULL != l_listLoadedTexture[iii]
		    && NULL != l_listLoadedTexture[iii]->m_data)
		{
			l_listLoadedTexture[iii]->m_loaded = false;
			EWOL_INFO("TEXTURE: Disable [" << iii << "]=(" << l_listLoadedTexture[iii]->m_width << "px," << 
			          l_listLoadedTexture[iii]->m_height << "px) in file:" << 
			          l_listLoadedTexture[iii]->m_filename << " OGl_Id=" <<l_listLoadedTexture[iii]->m_openGlTextureID);
			// note : the context might be destroy... we can not remove the textures ...
			//glDeleteTextures(1, &l_listLoadedTexture[iii]->m_openGlTextureID);
		}
	}
	pthread_mutex_unlock(&localMutex);
}


/**
 * @brief Check all texture and load/Remove/Reload all texture that has been Add/Remove/Change from the previous display cycle
 * @param ---
 * @return ---
 */
// TODO : The reload might be writen ...
void ewol::texture::UpdateContext(void)
{
	bool needRedraw = false;
	pthread_mutex_lock(&localMutex);
	for (int32_t iii=0; iii < l_listLoadedTexture.Size(); iii++) {
		if(    NULL != l_listLoadedTexture[iii]
		    && NULL != l_listLoadedTexture[iii]->m_data)
		{
			if(    false == l_listLoadedTexture[iii]->m_destroy
			    && false == l_listLoadedTexture[iii]->m_loaded)
			{
				GLuint textureid;
				glGenTextures(1, &textureid);
				glBindTexture(l_listLoadedTexture[iii]->m_target, textureid);
				//glTexParameteri(tmpTex->m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
				//glTexParameteri(tmpTex->m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
				//--- mode nearest
				//glTexParameteri(tmpTex->m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				//glTexParameteri(tmpTex->m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				//--- Mode linear
				glTexParameteri(l_listLoadedTexture[iii]->m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(l_listLoadedTexture[iii]->m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				EWOL_INFO("TEXTURE: Add [" << iii << "]=(" << l_listLoadedTexture[iii]->m_width << "px," << 
				          l_listLoadedTexture[iii]->m_height << "px) in file:" << 
				          l_listLoadedTexture[iii]->m_filename << " OGl_Id=" <<textureid);
				glTexImage2D(l_listLoadedTexture[iii]->m_target,
				             l_listLoadedTexture[iii]->m_level,
				             l_listLoadedTexture[iii]->m_internalFormat,
				             l_listLoadedTexture[iii]->m_width,
				             l_listLoadedTexture[iii]->m_height,
				             l_listLoadedTexture[iii]->m_border,
				             l_listLoadedTexture[iii]->m_format,
				             l_listLoadedTexture[iii]->m_type,
				             l_listLoadedTexture[iii]->m_data);
				l_listLoadedTexture[iii]->m_openGlTextureID = textureid;
				l_listLoadedTexture[iii]->m_loaded = true;
				needRedraw = true;
			} else if (    true == l_listLoadedTexture[iii]->m_destroy
			            && true == l_listLoadedTexture[iii]->m_loaded)
			{
				// Request remove texture ...
				EWOL_INFO("TEXTURE: Rm [" << iii << "] file:" << l_listLoadedTexture[iii]->m_filename);
				glDeleteTextures(1, &l_listLoadedTexture[iii]->m_openGlTextureID);
				l_listLoadedTexture[iii]->m_loaded = false;
				l_listLoadedTexture[iii]->m_openGlTextureID = -1;
				if (NULL != l_listLoadedTexture[iii]->m_data) {
					delete[] l_listLoadedTexture[iii]->m_data;
					l_listLoadedTexture[iii]->m_data = NULL;
				}
				delete(l_listLoadedTexture[iii]);
				l_listLoadedTexture[iii] = NULL;
			}
		}
	}
	pthread_mutex_unlock(&localMutex);
	if (true == needRedraw) {
		ewol::ForceRedrawAll();
	}
}


/**
 * @brief Register a texture in the automatic system to load and reload
 * @param[in] target Standard element of OpenGL target
 * @param[in] level Standard element of OpenGL level
 * @param[in] internalFormat Standard element of OpenGL internalFormat
 * @param[in] width Standard element of OpenGL width
 * @param[in] height Standard element of OpenGL height
 * @param[in] border Standard element of OpenGL border
 * @param[in] format Standard element of OpenGL format
 * @param[in] type Standard element of OpenGL type
 * @param[in] data Pointer on the buffer where are contain the data (a copy is done automaticly)
 * @param[in] nbBytes Number of byte in the buffer
 * @param[in] filename File Name of the texture or "---" if it is an internal loaded texture
 * @return The Internal ID of the texture, or -1 if an error occured ...
 */
int32_t ewol::texture::Load(int32_t target, int32_t level, int32_t internalFormat, int32_t width, int32_t height,
                            int32_t border, int32_t format, int32_t type,
                            const void* data, int32_t nbBytes, etk::UString filename)
{

	LoadedTexture *tmpTex = new LoadedTexture();
	int32_t outTextureID = -1;
	if (NULL == tmpTex){
		EWOL_ERROR("Texture : Allocation ERROR... " << filename);
		return -1;
	}
	if (NULL == data){
		EWOL_ERROR("Texture : Input pointer of the data texture =NULL... " << filename);
		return -1;
	}
	
	tmpTex->m_filename        = filename;
	tmpTex->m_nbTimeLoaded    = 1;
	tmpTex->m_openGlTextureID = -1;
	tmpTex->m_target          = target;
	tmpTex->m_level           = level;
	tmpTex->m_internalFormat  = internalFormat;
	tmpTex->m_width           = width;
	tmpTex->m_height          = height;
	tmpTex->m_border          = border;
	tmpTex->m_format          = format;
	tmpTex->m_type            = type;
	tmpTex->m_nbBytes         = nbBytes;
	tmpTex->m_data            = new char[tmpTex->m_nbBytes+4096];
	tmpTex->m_loaded          = false;
	tmpTex->m_destroy         = false;
	if (NULL == tmpTex->m_data) {
		EWOL_ERROR("Texture : Data Allocation ERROR... " << filename);
		return -1;
	}
	memcpy(tmpTex->m_data, data, sizeof(char) * tmpTex->m_nbBytes);
	
	pthread_mutex_lock(&localMutex);
	l_listLoadedTexture.PushBack(tmpTex);
	outTextureID = l_listLoadedTexture.Size()-1;
	pthread_mutex_unlock(&localMutex);
	return outTextureID;
}


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


/**
 * @brief Load a specific file texture
 * @note : dimention must be a power of 2, otherwise, the display can be wrong... For the SVG, the texture automaticly generate the power of 2 dimention ...
 * @param[in] fileName File that might be open
 * @param[in] requestedWidth Requested size of the file we desire (if we can not resize it, we load it whit his normal size)
 * @return The Internal ID of the texture, or -1 if an error occured ...
 */
// TODO : Load non square texture ...
// TODO : Check the size to regenerate the texture if the size change
int32_t ewol::texture::Load(etk::UString tmpfileName, int32_t requestedWidth)
{
	int32_t outTextureID = -1;
	if (l_listLoadedTexture.Size()!=0) {
		for (int32_t iii=0; iii<l_listLoadedTexture.Size(); iii++) {
			if (NULL != l_listLoadedTexture[iii]) {
				if (l_listLoadedTexture[iii]->m_filename == tmpfileName) {
					l_listLoadedTexture[iii]->m_nbTimeLoaded++;
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


/**
 * @brief Remove a specific texture ID from the system
 * @note A texture can be loaded as many time we want, the texture will be destroy only when nobody want the texture anymore
 * @param[in] textureID The internal texture ID that might be remove
 * @return ---
 */
void ewol::texture::UnLoad(uint32_t textureID)
{
	//EWOL_INFO("Unload a specific tecture ID=" << textureID);
	if (textureID>=0 && (int32_t)textureID<l_listLoadedTexture.Size()) {
		if (NULL == l_listLoadedTexture[textureID]) {
			EWOL_ERROR("Texture : " << textureID << " does not existe anymore...");
			return;
		}
		l_listLoadedTexture[textureID]->m_nbTimeLoaded--;
		if (0 == l_listLoadedTexture[textureID]->m_nbTimeLoaded) {
			EWOL_DEBUG("Remove openGL texture ID=" << textureID << " file:" << l_listLoadedTexture[textureID]->m_filename);
			l_listLoadedTexture[textureID]->m_destroy = true;
		}
		return;
	}
	EWOL_CRITICAL("Can not find TextureId=" << (int)textureID << " in the list of texture loaded...==> to remove it ...");
}


/**
 * @brief Get the openGL texture ID whith the internal ID
 * @param textureID the internal texture ID
 * @return the OpenGl texture ID (or 0 if an error occured...)
 */
uint32_t ewol::texture::GetGLID(uint32_t textureID)
{
	if (textureID>=0 && (int32_t)textureID<l_listLoadedTexture.Size()) {
		return l_listLoadedTexture[textureID]->m_openGlTextureID;
	}
	return 0;
}


/**
 * @brief Get the size of the specific texture
 * @param[in] textureID the internal texture ID
 * @return the width of the texture
 */
int32_t ewol::texture::GetSize(uint32_t textureID)
{
	for (int32_t iii=0; iii<l_listLoadedTexture.Size(); iii++) {
		if (l_listLoadedTexture[iii]->m_openGlTextureID == textureID) {
			return l_listLoadedTexture[iii]->m_width;
		}
	}
	EWOL_ERROR("Can not find TextureId=" << textureID << " in the list of texture loaded...");
	return -1;
}
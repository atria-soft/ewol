/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/texture/Texture.h>
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



ewol::Texture::Texture(etk::UString tmpName) :
	Resource(tmpName)
{
	m_loaded = false;
	m_texId = 0;
	m_endPointSize.x = 1.0;
	m_endPointSize.y = 1.0;
}


ewol::Texture::~Texture(void)
{
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


void ewol::Texture::RemoveContext(void)
{
	if (true == m_loaded) {
		// Request remove texture ...
		EWOL_INFO("TEXTURE: Rm [" << m_uniqueId << "] texId=" << m_texId);
		glDeleteTextures(1, &m_texId);
		m_loaded = false;
	}
}


void ewol::Texture::RemoveContextToLate(void)
{
	m_loaded = false;
}


void ewol::Texture::Flush(void)
{
	// request to the manager to be call at the next update ...
	ewol::resource::Update(this);
}


void ewol::Texture::SetImageSize(etk::Vector2D<int32_t> newSize)
{
	newSize.x = nextP2(newSize.x);
	newSize.y = nextP2(newSize.y);
	m_data.Resize(newSize);
}


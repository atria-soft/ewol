/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <etk/types.h>
#include <egami/egami.h>
#include <ewol/renderer/ResourceManager.h>
#include <ewol/renderer/resources/Image.h>
#include <ewol/renderer/resources/Texture.h>


#undef __class__
#define __class__	"TextureFile"

ewol::TextureFile::TextureFile(etk::UString _genName, const etk::UString& _tmpfileName, const ivec2& _size) :
	Texture(_genName)
{
	if (false == egami::Load(m_data, _tmpfileName, _size)) {
		EWOL_ERROR("ERROR when loading the image : " << _tmpfileName);
	}
	ivec2 tmp = m_data.GetSize();
	m_realImageSize = vec2(tmp.x(), tmp.y());
	Flush();
}


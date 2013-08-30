/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/resources/ResourceManager.h>
#include <ewol/Material.h>
#include <ewol/debug.h>

ewol::MaterialGlId::MaterialGlId(void) :
	m_GL_ambientFactor(0),
	m_GL_diffuseFactor(0),
	m_GL_specularFactor(0),
	m_GL_shininess(0),
	m_GL_texture0(0)
{
	// nothing to do else ...
}


void ewol::MaterialGlId::Link(ewol::Program* _prog, const etk::UString& _baseName)
{
	if (NULL == _prog) {
		return;
	}
	m_GL_ambientFactor = _prog->GetUniform(_baseName+".ambientFactor");
	m_GL_diffuseFactor = _prog->GetUniform(_baseName+".diffuseFactor");
	m_GL_specularFactor = _prog->GetUniform(_baseName+".specularFactor");
	m_GL_shininess = _prog->GetUniform(_baseName+".shininess");
	m_GL_texture0 = _prog->GetUniform("EW_texID");
}

ewol::Material::Material(void) :
	m_ambientFactor(1,1,1,1),
	m_diffuseFactor(0,0,0,1),
	m_specularFactor(0,0,0,1),
	m_shininess(1),
	m_texture0(NULL)
{
	// nothing to do else ...
}
ewol::Material::~Material(void)
{
	if(NULL!=m_texture0) {
		ewol::ResourceManager::Release(m_texture0);
	}
}

void ewol::Material::Draw(ewol::Program* _prog, const MaterialGlId& _glID)
{
	_prog->Uniform4(_glID.m_GL_ambientFactor, m_ambientFactor);
	_prog->Uniform4(_glID.m_GL_diffuseFactor, m_diffuseFactor);
	_prog->Uniform4(_glID.m_GL_specularFactor, m_specularFactor);
	_prog->Uniform1f(_glID.m_GL_shininess, m_shininess);
	if (NULL != m_texture0) {
		_prog->SetTexture0(_glID.m_GL_texture0, m_texture0->GetId());
	}
}

void ewol::Material::SetTexture0(const etk::UString& _filename)
{
	ivec2 tmpSize(256, 256);
	// prevent overloard error :
	ewol::TextureFile* tmpCopy = m_texture0;
	m_texture0 = NULL;
	if (false == ewol::ResourceManager::Keep(_filename, m_texture0, tmpSize)) {
		EWOL_ERROR("Can not load specific texture : " << _filename);
		// retreave previous texture:
		m_texture0 = tmpCopy;
		return;
	}
	if (NULL != tmpCopy) {
		// really release previous texture. In case of same texture loading, then we did not have reload it .. just increase and decrease index...
		ewol::ResourceManager::Release(tmpCopy);
	}
}




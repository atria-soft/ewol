/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/renderer/Material.h>
#include <ewol/debug.h>

ewol::Material::Material(void) :
	m_ambientFactor(0,0,0,0),
	m_diffuseFactor(0,0,0,0),
	m_specularFactor(0,0,0,0),
	m_shininess(0),
	m_GL_ambientFactor(0),
	m_GL_diffuseFactor(0),
	m_GL_specularFactor(0),
	m_GL_shininess(0)
{
	// nothing to do else ...
}

ewol::Material::~Material(void)
{
	
}

void ewol::Material::Link(ewol::Program* prog, const etk::UString& baseName)
{
	if (NULL == prog) {
		return;
	}
	m_GL_ambientFactor = prog->GetUniform(baseName+".ambientFactor");
	m_GL_diffuseFactor = prog->GetUniform(baseName+".diffuseFactor");
	m_GL_specularFactor = prog->GetUniform(baseName+".specularFactor");
	m_GL_shininess = prog->GetUniform(baseName+".shininess");
}

void ewol::Material::Draw(ewol::Program* prog)
{
	prog->Uniform4(m_GL_ambientFactor, m_ambientFactor);
	prog->Uniform4(m_GL_diffuseFactor, m_diffuseFactor);
	prog->Uniform4(m_GL_specularFactor, m_specularFactor);
	prog->Uniform1f(m_GL_shininess, m_shininess);
}



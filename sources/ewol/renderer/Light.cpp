/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/renderer/Light.h>
#include <ewol/debug.h>

ewol::Light::Light(void) :
	m_direction(0,0,0),
	m_halfplane(0,0,0),
	m_ambientColor(0,0,0,0),
	m_diffuseColor(0,0,0,0),
	m_specularColor(0,0,0,0),
	m_GL_direction(0),
	m_GL_halfplane(0),
	m_GL_ambientColor(0),
	m_GL_diffuseColor(0),
	m_GL_specularColor(0)
{
	// nothing to do else ...
}

ewol::Light::~Light(void)
{
	
}

void ewol::Light::Link(ewol::Program* prog, const etk::UString& baseName)
{
	m_GL_direction = prog->GetUniform(baseName+".direction");
	m_GL_halfplane = prog->GetUniform(baseName+".halfplane");
	m_GL_ambientColor = prog->GetUniform(baseName+".ambientColor");
	m_GL_diffuseColor = prog->GetUniform(baseName+".diffuseColor");
	m_GL_specularColor = prog->GetUniform(baseName+".specularColor");
}

void ewol::Light::Draw(ewol::Program* prog)
{
	prog->Uniform3(m_GL_direction, m_direction);
	prog->Uniform3(m_GL_halfplane, m_halfplane);
	prog->Uniform4(m_GL_ambientColor, m_ambientColor);
	prog->Uniform4(m_GL_diffuseColor, m_diffuseColor);
	prog->Uniform4(m_GL_specularColor, m_specularColor);
}


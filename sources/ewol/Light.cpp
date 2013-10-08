/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/Light.h>
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

void ewol::Light::link(ewol::Program* prog, const etk::UString& baseName)
{
	if (NULL == prog) {
		return;
	}
	m_GL_direction = prog->getUniform(baseName+".direction");
	m_GL_halfplane = prog->getUniform(baseName+".halfplane");
	m_GL_ambientColor = prog->getUniform(baseName+".ambientColor");
	m_GL_diffuseColor = prog->getUniform(baseName+".diffuseColor");
	m_GL_specularColor = prog->getUniform(baseName+".specularColor");
}

void ewol::Light::draw(ewol::Program* prog)
{
	prog->uniform3(m_GL_direction, m_direction);
	prog->uniform3(m_GL_halfplane, m_halfplane);
	prog->uniform4(m_GL_ambientColor, m_ambientColor);
	prog->uniform4(m_GL_diffuseColor, m_diffuseColor);
	prog->uniform4(m_GL_specularColor, m_specularColor);
}

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::Light& _obj)
{
	_os << "light:{";
	_os << "dir=" << _obj.m_direction;
	_os << " halfplan=" << _obj.m_halfplane;
	_os << " color:{ anbiant=" << _obj.m_ambientColor;
	_os << " diffuse=" << _obj.m_diffuseColor;
	_os << " specular=" << _obj.m_specularColor;
	_os << "}}";
	return _os;
}


/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <etk/math/Matrix4.h>

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>


ewol::Compositing::Compositing(void)
{
	// nothing to do
}


ewol::Compositing::~Compositing(void)
{
	// nothing to do
}


void ewol::Compositing::ResetMatrix(void)
{
	m_matrixApply.Identity();
}


void ewol::Compositing::Translate(const vec3& _vect)
{
	m_matrixApply *= etk::matTranslate(_vect);
}


void ewol::Compositing::Rotate(const vec3& _vect, float _angle)
{
	m_matrixApply *= etk::matRotate(_vect, _angle);
}


void ewol::Compositing::Scale(const vec3& _vect)
{
	m_matrixApply *= etk::matScale(_vect);
}


void ewol::Compositing::Clear(void)
{
	m_matrixApply.Identity();
}


void ewol::Compositing::SetMatrix(const mat4& _mat)
{
	m_matrixApply = _mat;
}

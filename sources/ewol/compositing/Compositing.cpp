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


void ewol::Compositing::Translate(etk::Vector3D<float> vect)
{
	m_matrixApply *= etk::Matrix4::Translate(vect);
}


void ewol::Compositing::Rotate(etk::Vector3D<float> vect, float angle)
{
	m_matrixApply *= etk::Matrix4::Rotate(vect, angle);
}


void ewol::Compositing::Scale(etk::Vector3D<float> vect)
{
	m_matrixApply *= etk::Matrix4::Scale(vect);
}


void ewol::Compositing::Clear(void)
{
	m_matrixApply.Identity();
}

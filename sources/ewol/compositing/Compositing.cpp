/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/Debug.h>
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


void ewol::Compositing::Tranlate(etk::Vector3D<float> vect)
{
	m_matrixApply *= etk::matrix::Translate(vect.x, vect.y, vect.z);
}


void ewol::Compositing::Rotate(etk::Vector3D<float> vect)
{
	m_matrixApply *= etk::matrix::rotate(vect.x, vect.y, vect.z);
}


void ewol::Compositing::Scale(etk::Vector3D<float> vect)
{
	m_matrixApply *= etk::matrix::Scale(vect.x, vect.y, vect.z);
}
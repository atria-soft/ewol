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


ewol::compositing::Compose::Compose(void) {
	// nothing to do
}


ewol::compositing::Compose::~Compose(void) {
	// nothing to do
}


void ewol::compositing::Compose::resetMatrix(void) {
	m_matrixApply.identity();
}


void ewol::compositing::Compose::translate(const vec3& _vect) {
	m_matrixApply *= etk::matTranslate(_vect);
}


void ewol::compositing::Compose::rotate(const vec3& _vect, float _angle) {
	m_matrixApply *= etk::matRotate(_vect, _angle);
}


void ewol::compositing::Compose::scale(const vec3& _vect) {
	m_matrixApply *= etk::matScale(_vect);
}


void ewol::compositing::Compose::clear(void) {
	m_matrixApply.identity();
}


void ewol::compositing::Compose::setMatrix(const mat4& _mat) {
	m_matrixApply = _mat;
}

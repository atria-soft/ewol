/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <etk/math/Matrix4x4.hpp>

#include <ewol/debug.hpp>
#include <ewol/compositing/Compositing.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::Compositing);

ewol::Compositing::Compositing() {
	// nothing to do
}


void ewol::Compositing::resetMatrix() {
	this.matrixApply.identity();
}


void ewol::Compositing::translate( Vector3f _vect) {
	this.matrixApply *= etk::matTranslate(_vect);
}


void ewol::Compositing::rotate( Vector3f _vect, float _angle) {
	this.matrixApply *= etk::matRotate(_vect, _angle);
}


void ewol::Compositing::scale( Vector3f _vect) {
	this.matrixApply *= etk::matScale(_vect);
}


void ewol::Compositing::clear() {
	this.matrixApply.identity();
}


void ewol::Compositing::setMatrix( mat4 _mat) {
	this.matrixApply = _mat;
}

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/debug.hpp>
#include <etk/types.hpp>
#include <etk/math/Matrix4x4.hpp>

namespace ewol {
	class Compositing {
		protected:
			mat4 this.matrixApply;
		public:
			/**
			 * @brief generic ructor
			 */
			Compositing();
			/**
			 * @brief Generic destructor
			 */
			 ~Compositing() = default;
			/**
			 * @brief Virtal pure function that request the draw of all openGl elements
			 */
			 void draw(boolean _disableDepthTest = true) = 0;
			/**
			 * @brief clear alll tre registered element in the current element
			 */
			 void clear();
			/**
			 * @brief reset to the eye matrix the openGL mouving system
			 */
			 void resetMatrix();
			/**
			 * @brief translate the current display of this element
			 * @param[in] _vect The translation vector to apply at the transformation matrix
			 */
			 void translate( Vector3f _vect);
			/**
			 * @brief rotate the curent display of this element
			 * @param[in] _vect The rotation vector to apply at the transformation matrix
			 */
			 void rotate( Vector3f _vect, float _angle);
			/**
			 * @brief scale the current diaplsy of this element
			 * @param[in] _vect The scaling vector to apply at the transformation matrix
			 */
			 void scale( Vector3f _vect);
			/**
			 * @brief set the transformation matrix
			 * @param[in] _mat The new matrix.
			 */
			 void setMatrix( mat4 _mat);
	};
};

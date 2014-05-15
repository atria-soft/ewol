/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_H__
#define __EWOL_COMPOSITING_H__

#include <ewol/debug.h>
#include <etk/types.h>
#include <etk/math/Matrix4.h>

namespace ewol {
	class Compositing {
		protected:
			mat4 m_matrixApply;
		public:
			/**
			 * @brief generic constructor
			 */
			Compositing();
			/**
			 * @brief Generic destructor
			 */
			virtual ~Compositing();
			/**
			 * @brief Virtal pure function that request the draw of all openGl elements
			 */
			virtual void draw(bool _disableDepthTest = true) = 0;
			/**
			 * @brief clear alll tre registered element in the current element
			 */
			virtual void clear();
			/**
			 * @brief reset to the eye matrix the openGL mouving system
			 */
			virtual void resetMatrix();
			/**
			 * @brief translate the current display of this element
			 * @param[in] _vect The translation vector to apply at the transformation matrix
			 */
			virtual void translate(const vec3& _vect);
			/**
			 * @brief rotate the curent display of this element
			 * @param[in] _vect The rotation vector to apply at the transformation matrix
			 */
			virtual void rotate(const vec3& _vect, float _angle);
			/**
			 * @brief scale the current diaplsy of this element
			 * @param[in] _vect The scaling vector to apply at the transformation matrix
			 */
			virtual void scale(const vec3& _vect);
			/**
			 * @brief set the transformation matrix
			 * @param[in] _mat The new matrix.
			 */
			virtual void setMatrix(const mat4& _mat);
	};
};

#endif

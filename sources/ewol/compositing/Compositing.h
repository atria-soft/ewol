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
#include <etk/UString.h>
#include <etk/math/Matrix4.h>

namespace ewol
{
	class Compositing
	{
		protected:
			etk::Matrix4 m_matrixApply;
		public:
			/**
			 * @brief generic constructor
			 */
			Compositing(void);
			/**
			 * @brief Generic destructor
			 */
			virtual ~Compositing(void);
			/**
			 * @brief Virtal pure function that request the draw of all openGl elements
			 */
			virtual void Draw(void)=0;
			/**
			 * @brief Clear alll tre registered element in the current element
			 */
			virtual void Clear(void);
			/**
			 * @brief Reset to the eye matrix the openGL mouving system
			 */
			virtual void ResetMatrix(void);
			/**
			 * @brief Translate the current display of this element
			 * @param[in] vect The translation vector to apply at the transformation matrix
			 */
			virtual void Translate(etk::Vector3D<float> vect);
			/**
			 * @brief Rotate the curent display of this element
			 * @param[in] vect The rotation vector to apply at the transformation matrix
			 */
			virtual void Rotate(etk::Vector3D<float> vect, float angle);
			/**
			 * @brief Scale the current diaplsy of this element
			 * @param[in] vect The scaling vector to apply at the transformation matrix
			 */
			virtual void Scale(etk::Vector3D<float> vect);
	};
};

#endif

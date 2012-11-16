/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_H__
#define __EWOL_COMPOSITING_H__

#include <ewol/Debug.h>
#include <etk/math/Matrix4.h>
#include <etk/UString.h>

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
			void virtual Draw(void)=0;
			/**
			 * @brief Reset to the eye matrix the openGL mouving system
			 */
			void ResetMatrix(void);
			/**
			 * @brief Translate the current display of this element
			 * @param[in] vect The translation vector to apply at the transformation matrix
			 */
			void Tranlate(etk::Vector3D<float> vect);
			/**
			 * @brief Rotate the curent display of this element
			 * @param[in] vect The rotation vector to apply at the transformation matrix
			 */
			void Rotate(etk::Vector3D<float> vect);
			/**
			 * @brief Scale the current diaplsy of this element
			 * @param[in] vect The scaling vector to apply at the transformation matrix
			 */
			void Scale(etk::Vector3D<float> vect);
	};
};

#endif

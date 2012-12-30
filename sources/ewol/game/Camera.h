/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_CAMERA_H__
#define __GAME_CAMERA_H__


#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Matrix4.h>

namespace game
{
	class Camera
	{
		private:
			vec3  m_position;   //!< position of the camera.
			vec3  m_angles;     //!< Angles to the camera is seing.
			mat4  m_matrix;     //!< transformation matrix.
			bool  m_needUpdate; //!< matrix is nor anymore correct...
			/**
			 * @brief Update the matrix property
			 */
			void Update(void);
		public:
			/**
			 * @brief Constructor.
			 * @param[in] pos Position of the camera.
			 * @param[in] angles Rotations angles of the camera
			 */
			Camera(vec3 pos=vec3(0,0,0), vec3 angles=vec3(0,0,0));
			/**
			 * @brief Set the position of the camera.
			 * @param[in] pos Position of the camera.
			 */
			void SetPosition(vec3 pos);
			/**
			 * @brief Get the curent Camera position.
			 * @return the current position.
			 */
			vec3& GetPosition(void) { return m_position; };
			/**
			 * @brief Set the angle on the camera
			 * @param[in] angles Rotations angles of the camera
			 */
			void SetAngle(vec3 angles);
			/**
			 * @brief Get the curent Camera angles.
			 * @return the current angles.
			 */
			vec3& GetAngle(void) { return m_angles; };
			/**
			 * @brief Get the transformation matix for the camera.
			 * @return the current transformation matrix
			 */
			mat4& GetMatrix(void);
			
	};
};

#endif


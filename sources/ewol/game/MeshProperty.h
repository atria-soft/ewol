/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_MESH_PROPERTY_H__
#define __GAME_MESH_PROPERTY_H__

#include <etk/types.h>
#include <etk/Vector.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Matrix4.h>

namespace game
{
	class MeshProperty
	{
		public:
			vec3   m_scale;     //!< scale of the element.
			vec3   m_position;  //!< position of the element.
			vec3   m_speed;     //!< Speed of the element.
			vec3   m_angle;
			mat4   m_rotation;  //!< rotation matrix ...
			bool needUpdate;
			mat4   m_matrix;     //!< generated dispaly matrix.
		public:
			MeshProperty(void) :
				m_position(0,0,0),
				m_speed(0,0,0),
				needUpdate(true)
			{
				//m_matrix.Identity();
			};
			virtual ~MeshProperty(void) {};
			
			void Identity(void)
			{
				m_position = vec3(0,0,0);
				m_speed = vec3(0,0,0);
				m_angle = vec3(0,0,0);
				m_matrix.Identity();
			};
			void Translate(etk::Vector3D<float> vect)
			{
				m_position += vect;
				needUpdate = true;
			}
			void Scale(etk::Vector3D<float> vect)
			{
				m_scale = vect;
				needUpdate = true;
			}
			void Rotate(etk::Vector3D<float> vect, float angleRad=0.0)
			{
				m_rotation *= etk::matRotate(vect, angleRad);
				needUpdate = true;
			}
			mat4& GetMatrix(void)
			{
				if (needUpdate == true) {
					m_matrix = etk::matScale(m_scale) * m_rotation * etk::matTranslate(m_position);
				}
				return m_matrix;
			};
			vec3& GetSpeed(void)
			{
				return m_speed;
			}
			vec3& GetPosition(void)
			{
				return m_position;
			}
	};
};


#endif



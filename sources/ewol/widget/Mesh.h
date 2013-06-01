/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_MESH_H__
#define __EWOL_WIDGET_MESH_H__

#include <etk/types.h>
#include <ewol/widget/Widget.h>
#include <ewol/renderer/resources/MeshObj.h>

extern const char * const ewolEventMeshPressed;


namespace widget {
	class Mesh :public ewol::Widget
	{
		private:
			// mesh name :
			etk::UString m_meshName;
			ewol::MeshObj* m_object;
			// mesh display properties:
			vec3 m_position;
			vec3 m_angle;
			vec3 m_angleSpeed;
			float m_cameraDistance;
		public:
			Mesh(const etk::UString& filename); // automatic considering in the appl Data older
			virtual ~Mesh(void);
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "widget::Mesh"; };
			virtual void OnRegenerateDisplay(void);
			virtual void SystemDraw(const ewol::DrawProperty& displayProp);
			virtual void OnDraw(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual void PeriodicCall(const ewol::EventTime& _event);
		public:
			/**
			 * @brief Set a mesh name file
			 * @param[in] filename Name of the new mesh
			 */
			void SetFile(const etk::UString& filename);
			/**
			 * @brief Set the mesh position
			 * @param[in] pos The new position of the mesh
			 */
			void SetPosition(const vec3& pos);
			/**
			 * @brief Set the mesh angle of view
			 * @param[in] angle view angle of the mesh
			 */
			void SetAngle(const vec3& angle);
			/**
			 * @brief Set the mesh angle speed
			 * @param[in] spped radian speed of the mesh
			 */
			void SetAngleSpeed(const vec3& speed);
			/**
			 * @brief Set the camera distance of the mesh
			 * @param[in] dist Diatance of the mesh
			 */
			void SetDistance(float distance);
	};
};


#endif

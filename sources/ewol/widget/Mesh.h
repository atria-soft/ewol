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
#include <ewol/resources/Mesh.h>

extern const char * const ewolEventMeshPressed;


namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Mesh :public ewol::Widget {
			private:
				// mesh name :
				std::string m_meshName;
				ewol::Mesh* m_object;
				// mesh display properties:
				vec3 m_position;
				vec3 m_angle;
				vec3 m_angleSpeed;
				float m_cameraDistance;
			public:
				Mesh(const std::string& filename); // automatic considering in the appl Data older
				virtual ~Mesh(void);
			public: // Derived function
				virtual void onRegenerateDisplay(void);
				virtual void systemDraw(const ewol::DrawProperty& displayProp);
				virtual void onDraw(void);
				virtual bool onEventInput(const ewol::EventInput& _event);
				virtual void periodicCall(const ewol::EventTime& _event);
			public:
				/**
				 * @brief set a mesh name file
				 * @param[in] filename Name of the new mesh
				 */
				void setFile(const std::string& filename);
				/**
				 * @brief set the mesh position
				 * @param[in] pos The new position of the mesh
				 */
				void setPosition(const vec3& pos);
				/**
				 * @brief set the mesh angle of view
				 * @param[in] angle view angle of the mesh
				 */
				void setAngle(const vec3& angle);
				/**
				 * @brief set the mesh angle speed
				 * @param[in] spped radian speed of the mesh
				 */
				void setAngleSpeed(const vec3& speed);
				/**
				 * @brief set the camera distance of the mesh
				 * @param[in] dist Diatance of the mesh
				 */
				void setDistance(float distance);
		};
	};
};


#endif

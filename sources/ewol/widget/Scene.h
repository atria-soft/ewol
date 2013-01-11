/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SCENE_H__
#define __EWOL_SCENE_H__

#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Matrix4.h>
#include <etk/Vector.h>
#include <ewol/debug.h>
#include <ewol/game/Camera.h>
#include <ewol/widget/Widget.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/ResourceManager.h>

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class btDynamicsWorld;
#include <LinearMath/btScalar.h>
class btVector3;

namespace widget {
	class Scene :public ewol::Widget
	{
		protected:
			///this is the most important class
			btDefaultCollisionConfiguration* m_collisionConfiguration;
			btCollisionDispatcher*           m_dispatcher;
			btBroadphaseInterface*           m_broadphase;
			btConstraintSolver*              m_solver;
			btDynamicsWorld*                 m_dynamicsWorld;
			//keep the collision shapes, for deletion/cleanup
			etk::Vector<btCollisionShape*>   m_collisionShapes;
			game::Camera      m_camera;       //!< Display point of view
			bool              m_isRunning;    //!< the display is running (not in pause)
			double            m_lastCallTime; //!< previous call Time
			float             m_ratioTime;    //!< Ratio time for the speed of the game ...
			uint32_t          m_walk;         //!< Wolk properties
			int32_t           m_debugMode;
			ewol::Colored3DObject*    m_directDrawObject; // system to draw special object ...
		public:
			/**
			 * @brief Main scene constructor
			 * @param[in] gameEngine Used game engine for the display (can be NULL).
			 */
			Scene(btDynamicsWorld* gameEngine=NULL);
			/**
			 * @brief Destructor 
			 * @note The engine is not destroy, it is the reponsability of the user
			 */
			virtual ~Scene(void);
			/**
			 * @brief Set the scene in pause for a while
			 */
			void Pause(void);
			/**
			 * @brief Resume the scene activity
			 */
			void Resume(void);
			/**
			 * @brief Toggle between pause and running
			 */
			void PauseToggle(void);
			// Derived function
			virtual void GenDraw(ewol::DrawProperty displayProp);
		protected:
			// Derived function
			virtual void ScenePeriodicCall(int64_t localTime, int32_t deltaTime) { };
		// camera properties :
		private:
			float m_zoom;
		public:
			/**
			 * @brief Get the current camera reference for the scene rendering
			 */
			game::Camera& GetCamera(void) { return m_camera; };
			/**
			 * @brief Set the curent Time Ratio (default 1)
			 */
			void SetRatioTime(float newRatio) { m_ratioTime = newRatio; };
			/**
			 * @brief Convert the absolute position in the local Position (Relative)
			 * @param[in] pos Absolute position that you request convertion
			 * @return the relative position
			 */
			virtual vec2 RelativePosition(vec2 pos);
			
			// Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::Scene"; };
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void PeriodicCall(int64_t localTime);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te statusEvent, vec2 pos);
			// Derived function
			virtual bool OnEventKb(ewol::keyEvent::status_te statusEvent, uniChar_t unicodeData);
			// Derived function
			virtual bool OnEventKbMove(ewol::keyEvent::status_te statusEvent, ewol::keyEvent::keyboard_te specialKey);
			// Derived function
			virtual void OnGetFocus(void);
			// Derived function
			virtual void OnLostFocus(void);
			void renderscene(int pass);
			void DrawOpenGL(btScalar* m, 
			                const btCollisionShape* shape,
			                const btVector3& color,
			                int32_t	debugMode,
			                const btVector3& worldBoundsMin,
			                const btVector3& worldBoundsMax);
			void DrawSphere(btScalar radius, int lats, int longs, mat4& transformationMatrix, draw::Colorf& tmpColor);
	};
};

#endif

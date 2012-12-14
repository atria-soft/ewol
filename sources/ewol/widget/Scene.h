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
#include <etk/Vector.h>
#include <ewol/debug.h>
#include <ewol/game/Engine.h>
#include <ewol/game/Element.h>
#include <ewol/widget/Widget.h>


namespace widget {
	class Scene :public ewol::Widget
	{
		protected:
			game::Engine*     m_gameEngine;   //!< display engine system
			bool              m_isRunning;    //!< the display is running (not in pause)
			int64_t           m_lastCallTime; //!< previous call Time
		public:
			/**
			 * @brief Main scene constructor
			 * @param[in] gameEngine Used game engine for the display (can be NULL).
			 */
			Scene(game::Engine* gameEngine=NULL);
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
			/**
			 * @brief extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
			 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
			 * @note This function is virtual for the scrolled widget, and the more complicated OpenGl widget
			 */
			virtual void GenDraw(ewol::DrawProperty displayProp);
		protected:
			/**
			 * @brief Periodic call in the sub element timed
			 * @param localTime curent system time
			 * @param deltaTime delta time while the previous call
			 */
			virtual void ScenePeriodicCall(int64_t localTime, int32_t deltaTime) { };
		// camera properties :
		private:
			vec3  m_camRotation;
			vec3  m_camTranslation;
			float m_camAngleView;
			float m_camdistanceViewStart;
			float m_camdistanceViewStop;
			float m_zoom;
		public:
			void SetCamaraTranslation();
			void SetCamaraRotation();
			void SetCamaraAngleView();
			void SetCamaraDistanceViewStart();
			void SetCamaraDistanceViewStop();
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
			
	};
};

#endif

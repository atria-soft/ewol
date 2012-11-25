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
#include <ewol/debug.h>
#include <ewol/widget/WidgetScrolled.h>
#include <ewol/oObject/Sprite.h>
#include <ewol/game/GameElement.h>


namespace widget {
	class Scene :public ewol::Widget
	{
		// TODO : Set it in private ...
		protected:
			SceneElement           m_sceneElement; //!< all element neede in the scene
			bool                   m_isRunning;
			int64_t                m_lastCallTime;
		public:
			Scene(void);
			virtual ~Scene(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolScene"; };
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void PeriodicCall(int64_t localTime);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			/**
			 * @brief Set the scene in pause for a while
			 * @param ---
			 * @return ---
			 */
			void Pause(void) { m_isRunning = false; };
			/**
			 * @brief Resume the scene activity
			 * @param ---
			 * @return ---
			 */
			void Resume(void) { m_isRunning = true; };
			/**
			 * @brief Toggle between pause and running
			 * @param ---
			 * @return ---
			 */
			void PauseToggle(void) { if(true==m_isRunning){ m_isRunning=false;}else{m_isRunning=true;} };
			/**
			 * @brief extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
			 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
			 * @note This function is virtual for the scrolled widget, and the more complicated OpenGl widget
			 * @param ---
			 * @return ---
			 */
			virtual void GenDraw(DrawProperty displayProp);
		protected:
			/**
			 * @brief Periodic call in the sub element timed
			 * @param localTime curent system time
			 * @param deltaTime delta time while the previous call
			 * @return ---
			 */
			virtual void ScenePeriodicCall(int64_t localTime, int32_t deltaTime) { };
		// camera properties :
		private:
			etk::Vector3D<float>   m_camRotation;
			etk::Vector3D<float>   m_camTranslation;
			float                  m_camAngleView;
			float                  m_camdistanceViewStart;
			float                  m_camdistanceViewStop;
			float                  m_zoom;
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
			virtual etk::Vector2D<float>  RelativePosition(etk::Vector2D<float>  pos);
	};
};

#endif

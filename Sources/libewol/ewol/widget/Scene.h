/**
 *******************************************************************************
 * @file ewol/widget/Scene.h
 * @brief ewol Scene widget system (header)
 * @author Edouard DUPIN
 * @date 01/04/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_SCENE_H__
#define __EWOL_SCENE_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/WidgetScrolled.h>
#include <ewol/OObject/Sprite.h>
#include <ewol/Game/GameElement.h>


namespace ewol {
	class Scene :public ewol::WidgetScrooled
	{
		// TODO : Set it in private ...
		protected:
			SceneElement        m_sceneElement; //!< all element neede in the scene
			bool                m_isRunning;
			int64_t             m_lastCallTime;
		public:
			Scene(void);
			virtual ~Scene(void);
			/**
			 * @brief Check if the object has the specific type.
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type of the object we want to check
			 * @return true if the object is compatible, otherwise false
			 */
			virtual bool CheckObjectType(const char * const objectType);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void);
			virtual void OnRegenerateDisplay(void);
			
			/**
			 * @brief Periodic call of this widget
			 * @param localTime curent system time
			 * @return ---
			 */
			virtual void PeriodicCall(int64_t localTime);
			/**
			 * @brief Common widget drawing function (called by the drawing thread [Android, X11, ...])
			 * @param ---
			 * @return ---
			 */
			virtual void OnDraw(void);
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
		protected:
			/**
			 * @brief Periodic call in the sub element timed
			 * @param localTime curent system time
			 * @param deltaTime delta time while the previous call
			 * @return ---
			 */
			virtual void ScenePeriodicCall(int64_t localTime, int32_t deltaTime) { };
	};
	
	/**
	 * @brief Initilise the basic widget property ==> due to the android system
	 * @note all widget that have template might have this initializer ...
	 * @param ---
	 * @return ---
	 */
	void WIDGET_SceneInit(void);
	
	extern const char * const TYPE_EOBJECT_WIDGET_SCENE;
	
};
#define EWOL_CAST_WIDGET_SCENE(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_SCENE,ewol::Scene,curentPointer)


#endif

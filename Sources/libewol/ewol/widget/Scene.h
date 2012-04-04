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
#include <ewol/OObject/Sprite.h>
#include <ewol/widget/WidgetScrolled.h>


namespace ewol {
	class GameElement
	{
		private:
			bool       m_visible;
			coord2D_ts m_position;
		public:
			         GameElement(void) { m_visible = true; m_position.x=0.0; m_position.y=0.0;};
			virtual ~GameElement(void) {};
	};
	
	class Scene :public ewol::WidgetScrooled
	{
		// TODO : Set it in private ...
		protected:
			etk::VectorType<ewol::OObject*> m_backgroundElements[NB_BOUBLE_BUFFER];   //!< element that must be display the first
			etk::VectorType<ewol::Sprite*> m_backgrouanimatedElements[NB_BOUBLE_BUFFER];   //!< element that must be display the first
			etk::VectorType<ewol::GameElement*> m_listAnimatedElements;   //!< generic element to display...
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

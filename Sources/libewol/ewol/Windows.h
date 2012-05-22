/**
 *******************************************************************************
 * @file ewol/Windows.h
 * @brief ewol window system (header)
 * @author Edouard DUPIN
 * @date 20/10/2011
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

#ifndef __EWOL_WINDOWS_H__
#define __EWOL_WINDOWS_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <etk/VectorType.h>
#include <ewol/Widget.h>

namespace ewol {
	
	class Windows :public ewol::Widget
	{
		public:
			Windows(void);
			virtual ~Windows(void);
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
		// internal event at ewol system : 
		public:
			void SysDraw(void);
			void SysOnShow(void) {};
			void SysOnHide(void) {};
			void SysOnKill(void) {};
		public:
			virtual void OnShow(void) { };
			virtual void OnHide(void) { };
			virtual bool OnKill(void) { return true; };
			virtual void OnReduce(void) { };
			virtual void On(void) { };
		public:
			virtual bool CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY);
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @param[in] pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 */
			virtual ewol::Widget * GetWidgetAtPos(coord2D_ts pos);
		private:
			bool m_hasDecoration;
		public:
			void SetDecorationDisable(void)
			{
				m_hasDecoration = false;
			}
			
			void SetDecorationEnable(void)
			{
				m_hasDecoration = true;
			}
		private:
			ewol::Widget*                   m_subWidget[NB_BOUBLE_BUFFER];
			etk::VectorType<ewol::Widget*>  m_popUpWidgetList[NB_BOUBLE_BUFFER];
		public:
			void SetSubWidget(ewol::Widget * widget);
			void PopUpWidgetPush(ewol::Widget * widget);
		protected:
			virtual void OnDraw(void);
		public:
			virtual void OnRegenerateDisplay(void);
		public:
			/**
			 * @brief Event generated to inform a flip-flop has occured on the current widget
			 * @param ---
			 * @return ---
			 */
			virtual void OnFlipFlopEvent(void);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			virtual void OnObjectRemove(ewol::EObject * removeObject);
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_WINDOWS;
	
};
#define EWOL_CAST_WIDGET_WINDOWS(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_WINDOWS,ewol::Windows,curentPointer)

#endif


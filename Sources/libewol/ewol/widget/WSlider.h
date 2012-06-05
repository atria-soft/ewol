/**
 *******************************************************************************
 * @file ewol/widget/WSlider.h
 * @brief ewol widget slider widget system (Header)
 * @author Edouard DUPIN
 * @date 05/06/2012
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

#ifndef __EWOL_W_SLIDER_H__
#define __EWOL_W_SLIDER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>

namespace ewol {
	class WSlider :public ewol::Widget
	{
		public:
			WSlider(void);
			virtual ~WSlider(void);
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
		public:
			virtual bool   CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
			virtual void   SetMinSise(etkFloat_t x=-1, etkFloat_t y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual bool   CanExpentX(void);
			virtual void   SetExpendY(bool newExpend=false);
			virtual bool   CanExpentY(void);
			void           LockExpendContamination(bool lockExpend=false);
		private:
			bool                           m_lockExpendContamination;
			etk::VectorType<ewol::Widget*> m_subWidget[NB_BOUBLE_BUFFER];
			int32_t                        m_windowsSources;     // widget source viewed
			int32_t                        m_windowsDestination; // widget destinated viewed
			int32_t                        m_slidingProgress;    // ratio progression of a sliding
		public:
			virtual void    SubWidgetRemoveAll(void);
			virtual void    SubWidgetAdd(ewol::Widget* newWidget);
			virtual void    SubWidgetRemove(ewol::Widget* newWidget);
			virtual void    SubWidgetUnLink(ewol::Widget* newWidget);
			        void    SubWidgetSelectSet(int32_t id);
			        int32_t SubWidgetSelectGet(void) { return (int32_t)m_slidingProgress; };
			        int32_t SubWidgetNumber(void) { return m_subWidget[m_currentCreateId].Size(); };
		protected:
			virtual void   OnDraw(void);
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @param[in] pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 */
			virtual ewol::Widget * GetWidgetAtPos(coord2D_ts pos);
			/**
			 * @brief Event generated to inform a flip-flop has occured on the current widget
			 * @param ---
			 * @return ---
			 */
			virtual void   OnFlipFlopEvent(void);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			/**
			 * @brief Periodic call of this widget
			 * @param localTime curent system time
			 * @return ---
			 */
			virtual void PeriodicCall(int64_t localTime);
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_W_SLIDER;
	
};

#define EWOL_CAST_WIDGET_W_SLIDER(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_W_SLIDER,ewol::WSlider,curentPointer)

#endif

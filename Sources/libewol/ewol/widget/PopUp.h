/**
 *******************************************************************************
 * @file ewol/widget/PopUp.h
 * @brief ewol pop-up widget system (header)
 * @author Edouard DUPIN
 * @date 29/12/2011
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

#ifndef __EWOL_POP_UP_H__
#define __EWOL_POP_UP_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>
#include <ewol/widget/Drawable.h>

namespace ewol {
	class PopUp : public ewol::Drawable
	{
		public:
			PopUp(void);
			virtual ~PopUp(void);
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
			virtual bool   CalculateSize(float availlableX, float availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
			virtual void   SetMinSise(float x=-1, float y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual void   SetExpendY(bool newExpend=false);
			void           SetDisplayRatio(float ratio);
		private:
			color_ts      m_colorBackGroung;
			color_ts      m_colorBorder;
			color_ts      m_colorEmptyArea;
			ewol::Widget* m_subWidgetNext;
			ewol::Widget* m_subWidget[NB_BOUBLE_BUFFER];
			float    m_displayRatio;
		public:
			void           SubWidgetSet(ewol::Widget* newWidget);
			void           SubWidgetRemove(void);
		protected:
			virtual void   OnDraw(ewol::DrawProperty& displayProp);
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @param[in] pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 */
			virtual ewol::Widget * GetWidgetAtPos(Vector2D<float>  pos);
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
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_POP_UP;
	
};

#define EWOL_CAST_WIDGET_POP_UP(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_POP_UP,ewol::PopUp,curentPointer)

#endif

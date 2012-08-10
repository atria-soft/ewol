/**
 *******************************************************************************
 * @file ewol/widget/ContextMenu.h
 * @brief ewol contextual Menu widget system (header)
 * @author Edouard DUPIN
 * @date 16/02/2012
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

#ifndef __EWOL_CONTEXT_MENU_H__
#define __EWOL_CONTEXT_MENU_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>
#include <ewol/widget/Drawable.h>

namespace ewol {
	typedef enum {
		CONTEXT_MENU_MARK_TOP,
		CONTEXT_MENU_MARK_RIGHT,
		CONTEXT_MENU_MARK_BOTTOM,
		CONTEXT_MENU_MARK_LEFT,
		CONTEXT_MENU_MARK_NONE
	}markPosition_te;
	class ContextMenu : public ewol::Drawable
	{
		public:
			ContextMenu(void);
			virtual ~ContextMenu(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolContextMenu"; };
		public:
			virtual bool   CalculateSize(float availlableX, float availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
			virtual void   SetMinSise(float x=-1, float y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual void   SetExpendY(bool newExpend=false);
		private:
			color_ts        m_colorBackGroung;
			color_ts        m_colorBorder;
			Vector2D<float>       m_padding;
			Vector2D<float>       m_arrowPos;
			float      m_offset;
			markPosition_te m_arrawBorder;
			ewol::Widget*   m_subWidget;
		public:
			void           SubWidgetSet(ewol::Widget* newWidget);
			void           SubWidgetRemove(void);
			void           SetPositionMark(markPosition_te position, Vector2D<float>  arrowPos);
		protected:
			virtual void   OnDraw(DrawProperty& displayProp);
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
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool   OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float>  pos);
	};
	
};

#endif

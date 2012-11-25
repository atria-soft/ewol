/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_CONTEXT_MENU_H__
#define __EWOL_CONTEXT_MENU_H__

#include <etk/types.h>
#include <draw/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Drawable.h>

namespace widget {
	typedef enum {
		CONTEXT_MENU_MARK_TOP,
		CONTEXT_MENU_MARK_RIGHT,
		CONTEXT_MENU_MARK_BOTTOM,
		CONTEXT_MENU_MARK_LEFT,
		CONTEXT_MENU_MARK_NONE
	}markPosition_te;
	class ContextMenu : public widget::Drawable
	{
		public:
			ContextMenu(void);
			virtual ~ContextMenu(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolContextMenu"; };
		public:
			virtual bool   CalculateSize(float availlableX, float availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
			virtual void   SetMinSise(float x=-1, float y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual void   SetExpendY(bool newExpend=false);
		private:
			draw::Color        m_colorBackGroung;
			draw::Color        m_colorBorder;
			etk::Vector2D<float>       m_padding;
			etk::Vector2D<float>       m_arrowPos;
			float      m_offset;
			markPosition_te m_arrawBorder;
			ewol::Widget*   m_subWidget;
		public:
			void           SubWidgetSet(ewol::Widget* newWidget);
			void           SubWidgetRemove(void);
			void           SetPositionMark(markPosition_te position, etk::Vector2D<float>  arrowPos);
		protected:
			// Derived function
			virtual void   OnDraw(ewol::DrawProperty& displayProp);
		public:
			// Derived function
			virtual void   OnRegenerateDisplay(void);
			// Derived function
			virtual ewol::Widget * GetWidgetAtPos(etk::Vector2D<float>  pos);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
	};
	
};

#endif

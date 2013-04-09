/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_POP_UP_H__
#define __EWOL_POP_UP_H__

#include <etk/types.h>
#include <draw/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Drawable.h>

namespace widget {
	class PopUp : public widget::Drawable
	{
		public:
			PopUp(void);
			virtual ~PopUp(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolPopUp"; };
		public:
			// Derived function
			virtual void CalculateSize(const vec2& availlable);
			virtual void CalculateMinSize(void);
			virtual void SetMinSize(const vec2& size);
			virtual void SetExpand(const bvec2& newExpend);
			void SetDisplayRatio(float ratio);
		private:
			draw::Color      m_colorBackGroung;
			draw::Color      m_colorBorder;
			draw::Color      m_colorEmptyArea;
			ewol::Widget* m_subWidgetNext;
			ewol::Widget* m_subWidget;
			float    m_displayRatio;
		public:
			void           SubWidgetSet(ewol::Widget* newWidget);
			void           SubWidgetRemove(void);
		protected:
			// Derived function
			virtual void   OnDraw(ewol::DrawProperty& displayProp);
		public:
			// Derived function
			virtual void   OnRegenerateDisplay(void);
			// Derived function
			virtual ewol::Widget * GetWidgetAtPos(vec2  pos);
			// Derived function
			virtual void OnObjectRemove(ewol::EObject * removeObject);
	};
	
};

#endif

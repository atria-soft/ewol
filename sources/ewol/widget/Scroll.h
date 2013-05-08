/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_SCROLL_H__
#define __EWOL_WIDGET_SCROLL_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Drawing.h>

namespace widget {
	class Scroll : public widget::Container
	{
		public:
			static void Init(void);
			static void UnInit(void);
		private:
			ewol::Drawing m_draw;
		protected:
			vec2 m_scrollOrigin;
			vec2 m_scrollSize;
			//float m_limitScrolling;
		public:
			Scroll(void);
			virtual ~Scroll(void);
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "ewol::widget::Scroll"; };
			//virtual void OnRegenerateDisplay(void);
			//virtual void OnDraw(ewol::DrawProperty& _displayProp);
			virtual ewol::Widget* GetWidgetAtPos(const vec2& _pos);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			//virtual void GenDraw(ewol::DrawProperty _displayProp);
		protected:
	};
	
};

#endif

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
#include <ewol/widget/Container.h>
#include <ewol/compositing/Drawing.h>

namespace widget {
	class PopUp : public widget::Container
	{
		public:
			PopUp(void);
			virtual ~PopUp(void);
		private:
			draw::Color m_colorBackGroung;
			draw::Color m_colorBorder;
			draw::Color m_colorEmptyArea;
			ewol::Drawing m_compositing;
		protected: // Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
		public: // Derived function
			virtual void OnRegenerateDisplay(void);
			virtual void CalculateSize(const vec2& availlable);
			virtual void CalculateMinMaxSize(void);
			virtual const char * const GetObjectType(void) { return "ewol::PopUp"; };
			virtual ewol::Widget* GetWidgetAtPos(const vec2& pos);
	};
	
};

#endif

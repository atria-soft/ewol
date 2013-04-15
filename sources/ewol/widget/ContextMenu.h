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
#include <ewol/widget/Container.h>
#include <ewol/compositing/Drawing.h>

namespace widget {
	typedef enum {
		CONTEXT_MENU_MARK_TOP,
		CONTEXT_MENU_MARK_RIGHT,
		CONTEXT_MENU_MARK_BOTTOM,
		CONTEXT_MENU_MARK_LEFT,
		CONTEXT_MENU_MARK_NONE
	}markPosition_te;
	class ContextMenu : public widget::Container
	{
		public:
			ContextMenu(void);
			virtual ~ContextMenu(void);
		private:
			ewol::Drawing m_compositing;
			draw::Color m_colorBackGroung;
			draw::Color m_colorBorder;
			vec2 m_padding;
			vec2 m_arrowPos;
			float m_offset;
			markPosition_te m_arrawBorder;
		public:
			void SetPositionMark(markPosition_te position, vec2 arrowPos);
		protected: // Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
		public: // Derived function
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, const vec2& pos);
			virtual void CalculateSize(const vec2& availlable);
			virtual void CalculateMinMaxSize(void);
			virtual const char * const GetObjectType(void) { return "ewol::ContextMenu"; };
			virtual ewol::Widget* GetWidgetAtPos(const vec2& pos);
	};
	
};

#endif

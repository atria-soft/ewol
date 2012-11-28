/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_BUTTON_COLOR_H__
#define __EWOL_BUTTON_COLOR_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Text.h>
#include <ewol/widget/Widget.h>

extern const char * const ewolEventButtonColorChange;

namespace widget {
	class ButtonColor : public ewol::Widget
	{
		public:
			ButtonColor(void);
			ButtonColor(etk::UString newLabel);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolButtonColor"; };
			void Init(void);
			virtual ~ButtonColor(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
			etk::UString   GetLabel(void) {return m_label;};
			void           SetValue(bool val);
			bool           GetValue(void);
			void           SetPadding(etk::Vector2D<float>  newPadding);
		private:
			ewol::Text                 m_oObjectText;
			ewol::Drawing              m_oObjectDecoration;
			etk::Vector2D<float>       m_padding;
			etk::UString               m_label;
			draw::Color                m_textColorFg;    //!< Text color
			draw::Color                m_textColorBg;    //!< Background color
			draw::Color                m_selectedColor;  //!< user current selected Color
			widget::ContextMenu*       m_widgetContextMenu;
		public:
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
			draw::Color GetCurrentColor(void) { return m_selectedColor; };
			void     SetCurrentColor(draw::Color color);
			// Derived function
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
	};
};

#endif

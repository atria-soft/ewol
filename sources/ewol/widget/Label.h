/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_LABEL_H__
#define __EWOL_LABEL_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/widget/Widget.h>

extern const char * const ewolEventLabelPressed;

namespace widget {
	class Label : public ewol::Widget
	{
		public:
			Label(void);
			Label(etk::UString newLabel);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolLabel"; };
			void Init(void);
			virtual ~Label(void);
			// Derived function
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
		private:
			ewol::Text     m_oObjectText;
			etk::UString   m_label;
			draw::Color    m_textColorFg;  //!< Text color
		public:
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
	};
	
};

#endif

/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_CHECK_BOX_H__
#define __EWOL_CHECK_BOX_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Widget.h>

extern const char* const ewolEventCheckBoxClicked;

namespace widget {
	class CheckBox : public ewol::Widget
	{
		public:
			CheckBox(void);
			CheckBox(etk::UString newLabel);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolCheckBox"; };
			void Init(void);
			virtual ~CheckBox(void);
			// Derived function
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
			void           SetValue(bool val);
			bool           GetValue(void);
		private:
			ewol::Text     m_oObjectText;
			ewol::Drawing  m_oObjectDecoration;
			etk::UString   m_label;
			bool           m_value;
			draw::Color    m_textColorFg;  //!< Text color
			draw::Color    m_textColorBg;  //!< Background color
		public:
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
			// Derived function
			virtual bool OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData);
	};
	
};

#endif

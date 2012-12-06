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
		private:
			ewol::Text   m_text;  //!< Compositing text element.
			etk::UString m_label; //!< decorated text to display.
		public:
			/**
			 * @brief Constructor
			 * @param[in] newLabel The displayed decorated text.
			 */
			Label(etk::UString newLabel="---");
			/**
			 * @brief destructor
			 */
			virtual ~Label(void);
			/**
			 * @brief Change the label displayed
			 * @param[in] newLabel The displayed decorated text.
			 */
			void SetLabel(etk::UString newLabel);
			/**
			 * @brief Get the current displayed label
			 * @return The displayed decorated text.
			 */
			etk::UString GetLabel(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolLabel"; };
			// Derived function
			virtual bool   CalculateMinSize(void);
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos);
	};
	
};

#endif

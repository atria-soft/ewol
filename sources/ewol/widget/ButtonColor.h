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
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Widget.h>

extern const char * const ewolEventButtonColorChange;

namespace widget {
	class ButtonColor : public ewol::Widget
	{
		public:
			/**
			 * @brief Main constructor
			 */
			ButtonColor(draw::Color baseColor=draw::color::black);
			/**
			 * @brief Main destructor
			 */
			virtual ~ButtonColor(void);
		private:
			ewol::Shaper                m_shaper;             //!< Compositing theme.
			ewol::Text                  m_text;               //!< Compositing Test display.
			draw::Color                 m_textColorFg;        //!< Current color.
			widget::ContextMenu*        m_widgetContextMenu;  //!< Specific context menu.
			bool                        m_mouseHover;         //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
			bool                        m_buttonPressed;      //!< Flag to know if the button is curently pressed.
			// hover area :
			etk::Vector2D<float>        m_selectableAreaPos;  //!< Start position of the events
			etk::Vector2D<float>        m_selectableAreaSize; //!< Size of the event positions
		public:
			/**
			 * @brief Get the current color of the color selection widget
			 * @return The current color
			 */
			draw::Color GetValue(void);
			/**
			 * @brief Specify the current color.
			 * @param[in] color The new display color.
			 */
			void SetValue(draw::Color color);
		public:
			// Derived function
			virtual bool CalculateMinSize(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolButtonColor"; };
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
			// Derived function
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		private:
			/**
			 * @brief Internal system to Change the property of the current status
			 * @param[in] new state
			 */
			void ChangeStatusIn(int32_t newStatusId);
			// Derived function
			virtual void PeriodicCall(int64_t localTime);
	};
};

#endif

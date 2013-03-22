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
		private:
			ewol::Shaper          m_shaper;             //!< Compositing theme.
			ewol::Text            m_text;               //!< Compositing Test display.
			draw::Color           m_textColorFg;        //!< Current color.
			widget::ContextMenu*  m_widgetContextMenu;  //!< Specific context menu.
			bool                  m_mouseHover;         //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
			bool                  m_buttonPressed;      //!< Flag to know if the button is curently pressed.
			// hover area :
			vec2                  m_selectableAreaPos;  //!< Start position of the events
			vec2                  m_selectableAreaSize; //!< Size of the event positions
		public:
			/**
			 * @brief Main constructor.
			 * @param[in] baseColor basic displayed color.
			 * @param[in] shaperName The new shaper filename.
			 */
			ButtonColor(draw::Color baseColor=draw::color::black, etk::UString shaperName="THEME:GUI:widgetButton.conf");
			/**
			 * @brief Main destructor.
			 */
			virtual ~ButtonColor(void);
			/**
			 * @brief Set the shaper name (use the contructer one this permit to not noad unused shaper).
			 * @param[in] shaperName The new shaper filename.
			 */
			void SetShaperName(etk::UString shaperName);
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
			virtual const char * const GetObjectType(void) { return "widget::ButtonColor"; };
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos);
			// Derived function
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data);
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

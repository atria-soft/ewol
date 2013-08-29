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
#include <ewol/widget/WidgetManager.h>

extern const char * const ewolEventButtonColorChange;

namespace widget {
	class ButtonColor : public ewol::Widget
	{
		public:
			static void Init(ewol::WidgetManager& _widgetManager);
		private:
			ewol::Shaper m_shaper; //!< Compositing theme.
			ewol::Text m_text; //!< Compositing Test display.
			etk::Color<> m_textColorFg; //!< Current color.
			widget::ContextMenu* m_widgetContextMenu; //!< Specific context menu.
			bool m_mouseHover; //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
			bool m_buttonPressed; //!< Flag to know if the button is curently pressed.
			// hover area :
			vec2 m_selectableAreaPos; //!< Start position of the events
			vec2 m_selectableAreaSize; //!< Size of the event positions
		public:
			/**
			 * @brief Main constructor.
			 * @param[in] _baseColor basic displayed color.
			 * @param[in] _shaperName The new shaper filename.
			 */
			ButtonColor(etk::Color<> _baseColor=etk::color::black, etk::UString _shaperName="THEME:GUI:widgetButton.conf");
			/**
			 * @brief Main destructor.
			 */
			virtual ~ButtonColor(void);
			/**
			 * @brief Set the shaper name (use the contructer one this permit to not noad unused shaper).
			 * @param[in] _shaperName The new shaper filename.
			 */
			void SetShaperName(etk::UString _shaperName);
			/**
			 * @brief Get the current color of the color selection widget
			 * @return The current color
			 */
			etk::Color<> GetValue(void);
			/**
			 * @brief Specify the current color.
			 * @param[in] _color The new display color.
			 */
			void SetValue(etk::Color<> _color);
		protected: // Derived function
			virtual void OnDraw(void);
		public: // Derived function
			virtual void CalculateMinMaxSize(void);
			virtual const char * const GetObjectType(void) { return "widget::ButtonColor"; };
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual void OnReceiveMessage(const ewol::EMessage& _msg);
		private:
			/**
			 * @brief Internal system to Change the property of the current status
			 * @param[in] _newStatusId new state
			 */
			void ChangeStatusIn(int32_t _newStatusId);
			// Derived function
			virtual void PeriodicCall(const ewol::EventTime& _event);
	};
};

#endif

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
			static void init(ewol::WidgetManager& _widgetManager);
		private:
			ewol::Shaper m_shaper; //!< Compositing theme.
			ewol::Text m_text; //!< Compositing Test display.
			etk::Color<> m_textColorFg; //!< Current color.
			widget::ContextMenu* m_widgetContextMenu; //!< Specific context menu.
			bool m_mouseHover; //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
			bool m_buttonPressed; //!< Flag to know if the button is curently pressed.
			// hover area :
			vec2 m_selectableAreaPos; //!< Start position of the events
			vec2 m_selectableAreaSize; //!< size of the event positions
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
			 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper).
			 * @param[in] _shaperName The new shaper filename.
			 */
			void setShaperName(etk::UString _shaperName);
			/**
			 * @brief get the current color of the color selection widget
			 * @return The current color
			 */
			etk::Color<> getValue(void);
			/**
			 * @brief Specify the current color.
			 * @param[in] _color The new display color.
			 */
			void setValue(etk::Color<> _color);
		protected: // Derived function
			virtual void onDraw(void);
		public: // Derived function
			virtual void calculateMinMaxSize(void);
			virtual const char * const getObjectType(void) { return "widget::ButtonColor"; };
			virtual void onRegenerateDisplay(void);
			virtual bool onEventInput(const ewol::EventInput& _event);
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
		private:
			/**
			 * @brief internal system to change the property of the current status
			 * @param[in] _newStatusId new state
			 */
			void changeStatusIn(int32_t _newStatusId);
			// Derived function
			virtual void periodicCall(const ewol::EventTime& _event);
	};
};

#endif

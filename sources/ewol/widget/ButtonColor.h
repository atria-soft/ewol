/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
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
#include <ewol/widget/Manager.h>
#include <ewol/object/Signal.h>

namespace ewol {
	namespace widget {
		class ButtonColor : public ewol::Widget {
			public:
				// Event list of properties
				ewol::object::Signal<etk::Color<>> signalChange;
			private:
				ewol::compositing::Shaper m_shaper; //!< Compositing theme.
				ewol::compositing::Text m_text; //!< Compositing Test display.
				std::shared_ptr<ewol::widget::ContextMenu> m_widgetContextMenu; //!< Specific context menu.
				bool m_mouseHover; //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
				bool m_buttonPressed; //!< Flag to know if the button is curently pressed.
				// hover area :
				vec2 m_selectableAreaPos; //!< Start position of the events
				vec2 m_selectableAreaSize; //!< size of the event positions
			protected:
				/**
				 * @brief Main constructor.
				 * @param[in] _baseColor basic displayed color.
				 * @param[in] _shaperName The new shaper filename.
				 */
				ButtonColor();
				void init(etk::Color<> _baseColor=etk::color::black, std::string _shaperName="THEME:GUI:Button.json");
			public:
				DECLARE_WIDGET_FACTORY(ButtonColor, "ButtonColor");
				/**
				 * @brief Main destructor.
				 */
				virtual ~ButtonColor();
				/**
				 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper).
				 * @param[in] _shaperName The new shaper filename.
				 */
				void setShaperName(std::string _shaperName);
			protected:
				ewol::object::Param<etk::Color<>> m_textColorFg; //!< Current color.
			public:
				/**
				 * @brief get the current color of the color selection widget
				 * @return The current color
				 */
				const etk::Color<>& getValue() {
					return m_textColorFg.get();
				}
				/**
				 * @brief Specify the current color.
				 * @param[in] _color The new display color.
				 */
				void setValue(const etk::Color<>& _color) {
					m_textColorFg.set(_color);
				}
			protected: // Derived function
				virtual void onDraw();
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual void onParameterChangeValue(const ewol::object::ParameterRef& _paramPointer);
			private:
				/**
				 * @brief internal system to change the property of the current status
				 * @param[in] _newStatusId new state
				 */
				void changeStatusIn(int32_t _newStatusId);
				// Derived function
				virtual void periodicCall(const ewol::event::Time& _event);
				// Callback function:
				void onCallbackColorChange(const etk::Color<>& _color);
		};
	};
};

#endif

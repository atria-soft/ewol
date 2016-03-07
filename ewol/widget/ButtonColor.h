/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Manager.h>
#include <esignal/ISignal.h>

namespace ewol {
	namespace widget {
		class ButtonColor;
		using ButtonColorShared = ewol::SharedPtr<ewol::widget::ButtonColor>;
		using ButtonColorWeak = ewol::WeakPtr<ewol::widget::ButtonColor>;
		class ButtonColor : public ewol::Widget {
			public: // signals
				esignal::ISignal<etk::Color<>> signalChange;
			public: // properties
				eproperty::Value<etk::Color<>> propertyValue; //!< Current color.
				eproperty::Value<std::string> propertyShape; //!< Current color.
			private:
				ewol::compositing::Shaper m_shaper; //!< Compositing theme.
				ewol::compositing::Text m_text; //!< Compositing Test display.
				ewol::widget::ContextMenuShared m_widgetContextMenu; //!< Specific context menu.
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
				void init();
			public:
				DECLARE_WIDGET_FACTORY(ButtonColor, "ButtonColor");
				/**
				 * @brief Main destructor.
				 */
				virtual ~ButtonColor();
			protected: // Derived function
				virtual void onDraw();
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
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
			protected:
				virtual void onChangePropertyValue();
				virtual void onChangePropertyShape();
		};
	};
};


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
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Container2.h>
#include <ewol/widget/Manager.h>
#include <esignal/ISignal.h>


namespace ewol {
	namespace widget {
		class CheckBox : public ewol::widget::Container2 {
			public: // Event list
				esignal::ISignal<> signalPressed;
				esignal::ISignal<> signalDown;
				esignal::ISignal<> signalUp;
				esignal::ISignal<> signalEnter;
				esignal::ISignal<bool> signalValue;
			public: // propertie list
				eproperty::Value<bool> propertyValue; //!< Current state of the checkbox.
				eproperty::Value<std::string> propertyShape; //!< shape of the widget
			private:
				ewol::compositing::Shaper m_shaper; //!< Compositing theme.
				bool m_mouseHover; //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
				bool m_buttonPressed; //!< Flag to know if the button is curently pressed.
				// hover area :
				vec2 m_selectableAreaPos; //!< Start position of the events
				vec2 m_selectableAreaSize; //!< size of the event positions
				// shaper ids:
				int32_t m_shaperIdSize;
				int32_t m_shaperIdSizeInsize;
			protected:
				/**
				 * @brief Main checkbox constructor
				 * @param[in] _shaperName Shaper file properties
				 */
				CheckBox();
				void init(const std::string& _shaperName="{ewol}THEME:GUI:CheckBox.json");
			public:
				DECLARE_WIDGET_FACTORY(CheckBox, "CheckBox");
				/**
				 * @brief main destructor.
				 */
				virtual ~CheckBox();
			protected:
				/**
				 * @brief internal system to change the property of the current status
				 * @param[in] _newStatusId new state
				 */
				void changeStatusIn(int32_t _newStatusId);
				/**
				 * @brief update the status with the internal satte of the button ...
				 */
				void CheckStatus();
			protected: // Derived function
				virtual void onDraw();
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void onChangeSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual bool onEventEntry(const ewol::event::Entry& _event);
				virtual void periodicCall(const ewol::event::Time& _event);
		};
	};
};


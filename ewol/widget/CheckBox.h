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
#include <esignal/Signal.h>


namespace ewol {
	namespace widget {
		class CheckBox : public ewol::widget::Container2 {
			public:
				// Event list of properties
				esignal::Signal<void> signalPressed;
				esignal::Signal<void> signalDown;
				esignal::Signal<void> signalUp;
				esignal::Signal<void> signalEnter;
				esignal::Signal<bool> signalValue;
			private:
				eproperty::Value<ewol::compositing::Shaper> m_shaper; //!< Compositing theme.
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
				/**
				 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper)
				 * @param[in] _shaperName The new shaper filename
				 */
				void setShaperName(const std::string& _shaperName) {
					m_shaper.set(_shaperName);
				}
			protected:
				eproperty::Value<bool> m_value; //!< Current state of the checkbox.
			public:
				/**
				 * @brief set the current value of the checkbox (check or not)
				 * @param[in] _val New value of the button
				 */
				void setValue(bool _val) {
					m_value.set(_val);
				}
				/**
				 * @brief get the current button value.
				 * @return True : The checkbox is active.
				 * @return false : The checkbox is disable.
				 */
				bool getValue() const {
					return m_value;
				};
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


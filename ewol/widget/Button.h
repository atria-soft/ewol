/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#ifndef __EWOL_BUTTON_H__
#define __EWOL_BUTTON_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Image.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Container2.h>
#include <ewol/widget/Manager.h>
#include <ewol/signal/Signal.h>



namespace ewol {
	namespace widget {
		/**
		 * @brief a composed button is a button with an inside composed with the specify XML element 
		 * ==> this permit to generate standard element simple
		 */
		class Button : public ewol::widget::Container2 {
			public:
				// Event list of properties
				ewol::Signal<void> signalPressed;
				ewol::Signal<void> signalDown;
				ewol::Signal<void> signalUp;
				ewol::Signal<void> signalEnter;
				ewol::Signal<void> signalLeave;
				ewol::Signal<bool> signalValue;
				enum buttonLock{
					lockNone, //!< normal status of the button
					lockWhenPressed, //!< When the state is set in pressed, the status stay in this one
					lockWhenReleased, //!< When the state is set in not pressed, the status stay in this one
					lockAccess, //!< all event are trashed  == > acctivity of the button is disable
				};
			private:
				ewol::parameter::Value<ewol::compositing::Shaper> m_shaper; //!< Compositing theme.
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _shaperName Shaper file properties
				 */
				Button();
				void init(const std::string& _shaperName="{ewol}THEME:GUI:Button.json");
			public:
				DECLARE_WIDGET_FACTORY(Button, "Button");
				/**
				 * @brief Destructor
				 */
				virtual ~Button();
				/**
				 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper)
				 * @param[in] _shaperName The new shaper filename
				 */
				void setShaperName(const std::string& _shaperName) {
					m_shaper.setString(_shaperName);
				}
			protected:
				ewol::parameter::Value<bool> m_value; //!< Current state of the button.
			public:
				/**
				 * @brief set the currentValue of the Button (pressed or not)
				 * @note Work only in toggle mode
				 * @param[in] _val New value of the button
				 */
				void setValue(bool _val) {
					m_value.set(_val);
				}
				/**
				 * @brief get the current button value.
				 * @return True : The button is pressed.
				 * @return false : The button is released.
				 */
				bool getValue() const {
					return m_value;
				};
			protected:
				ewol::parameter::List<enum buttonLock> m_lock; //!< Current lock state of the button.
			public:
				/**
				 * @brief set the button lock state.
				 * @param[in] _lock New lock mode of the button
				 */
				void setLock(enum buttonLock _lock) {
					m_lock.set(_lock);
				}
				/**
				 * @brief get the current button lock value.
				 * @return The requested lock mode
				 */
				enum buttonLock getLock() const {
					return m_lock;
				};
			protected:
				ewol::parameter::Value<bool> m_toggleMode; //!< The button is able to toggle.
			public:
				/**
				 * @brief change the toggle mode.
				 * @param[in] _togg New toggle mode
				 */
				void setToggleMode(bool _togg) {
					m_toggleMode.set(_togg);
				}
				/**
				 * @brief get the current toggle mode.
				 * @return the current toggle mode.
				 */
				bool getToggleMode() const {
					return m_toggleMode;
				};
			protected:
				ewol::parameter::Value<bool> m_enableSingle; //!< When a single subwidget is set display all time it.
			public:
				/**
				 * @brief Chane the display single widget mode availlable.
				 * @param[in] _single single mode widget set
				 */
				void setEnableSingle(bool _single) {
					m_enableSingle.set(_single);
				}
				/**
				 * @brief get the current single mode enableling.
				 * @return the current value.
				 */
				bool getEnableSingle() const {
					return m_enableSingle;
				};
			private:
				bool m_mouseHover; //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
				bool m_buttonPressed; //!< Flag to know if the button is curently pressed.
				// hover area :
				vec2 m_selectableAreaPos; //!< Start position of the events
				vec2 m_selectableAreaSize; //!< size of the event positions
			private:
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
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void calculateSize(const vec2& _availlable);
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual bool onEventEntry(const ewol::event::Entry& _event);
				virtual void onDetectPresenceToggleWidget() {
					setToggleMode(true);
				}
			private: // derived function
				virtual void periodicCall(const ewol::event::Time& _event);
				virtual void onLostFocus();
		};
	};
};

#endif

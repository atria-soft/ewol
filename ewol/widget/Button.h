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
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Image.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Container2.h>
#include <ewol/widget/Manager.h>
#include <esignal/ISignal.h>



namespace ewol {
	namespace widget {
		/**
		 * @brief a composed button is a button with an inside composed with the specify XML element 
		 * ==> this permit to generate standard element simple
		 */
		class Button : public ewol::widget::Container2 {
			public:
				enum buttonLock{
					lockNone, //!< normal status of the button
					lockWhenPressed, //!< When the state is set in pressed, the status stay in this one
					lockWhenReleased, //!< When the state is set in not pressed, the status stay in this one
					lockAccess, //!< all event are trashed  == > acctivity of the button is disable
				};
			public: // Event list
				esignal::ISignal<> signalPressed;
				esignal::ISignal<> signalDown;
				esignal::ISignal<> signalUp;
				esignal::ISignal<> signalEnter;
				esignal::ISignal<> signalLeave;
				esignal::ISignal<bool> signalValue;
			public: // propertie list
				eproperty::Value<std::string> propertyShape; //!< shaper name property
				eproperty::Value<bool> propertyValue; //!< Current state of the button.
				eproperty::List<enum buttonLock> propertyLock; //!< Current lock state of the button.
				eproperty::Value<bool> propertyToggleMode; //!< The button is able to toggle.
				eproperty::Value<bool> propertyEnableSingle; //!< When a single subwidget is set display all time it.
			private:
				ewol::compositing::Shaper m_shaper; //!< Compositing theme.
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _shaperName Shaper file properties
				 */
				Button();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(Button, "Button");
				/**
				 * @brief Destructor
				 */
				virtual ~Button();
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
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void onChangeSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual bool onEventEntry(const ewol::event::Entry& _event);
				virtual void onDetectPresenceToggleWidget() {
					propertyToggleMode.set(true);
				}
			private: // derived function
				virtual void periodicCall(const ewol::event::Time& _event);
				virtual void onLostFocus();
			protected:
				virtual void onChangePropertyShape();
				virtual void onChangePropertyValue();
				virtual void onChangePropertyLock();
				virtual void onChangePropertyToggleMode();
				virtual void onChangePropertyEnableSingle();
		};
	};
};

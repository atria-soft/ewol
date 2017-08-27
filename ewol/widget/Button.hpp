/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/compositing/Image.hpp>
#include <ewol/compositing/Shaper.hpp>
#include <ewol/widget/Container2.hpp>
#include <ewol/widget/Manager.hpp>
#include <esignal/Signal.hpp>



namespace ewol {
	namespace widget {
		class Button;
		using ButtonShared = ememory::SharedPtr<ewol::widget::Button>;
		using ButtonWeak = ememory::WeakPtr<ewol::widget::Button>;
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
				esignal::Signal<> signalPressed;
				esignal::Signal<> signalDown;
				esignal::Signal<> signalUp;
				esignal::Signal<> signalEnter;
				esignal::Signal<> signalLeave;
				esignal::Signal<bool> signalValue;
			public: // propertie list
				eproperty::Value<etk::String> propertyShape; //!< shaper name property
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
				void init() override;
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
				virtual void onDraw() override;
			public:
				void calculateMinMaxSize() override;
				void onChangeSize() override;
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
				bool onEventEntry(const ewol::event::Entry& _event) override;
				void onDetectPresenceToggleWidget() override {
					propertyToggleMode.set(true);
				}
			protected:
				esignal::Connection m_PCH; //!< Periodic Call Handle to remove it when needed
				/**
				 * @brief Periodic call to update grapgic display
				 * @param[in] _event Time generic event
				 */
				void periodicCall(const ewol::event::Time& _event);
				void onLostFocus() override;
			protected:
				virtual void onChangePropertyShape();
				virtual void onChangePropertyValue();
				virtual void onChangePropertyLock();
				virtual void onChangePropertyToggleMode();
				virtual void onChangePropertyEnableSingle();
		};
	};
};

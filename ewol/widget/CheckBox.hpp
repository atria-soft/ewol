/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/compositing/Shaper.hpp>
#include <ewol/widget/Container2.hpp>
#include <ewol/widget/Manager.hpp>
#include <esignal/Signal.hpp>


namespace ewol {
	namespace widget {
		class CheckBox;
		using CheckBoxShared = ememory::SharedPtr<ewol::widget::CheckBox>;
		using CheckBoxWeak = ememory::WeakPtr<ewol::widget::CheckBox>;
		class CheckBox : public ewol::widget::Container2 {
			public: // Event list
				esignal::Signal<> signalPressed;
				esignal::Signal<> signalDown;
				esignal::Signal<> signalUp;
				esignal::Signal<> signalEnter;
				esignal::Signal<bool> signalValue;
			public: // propertie list
				eproperty::Value<bool> propertyValue; //!< Current state of the checkbox.
				eproperty::Value<etk::Uri> propertyShape; //!< shape of the widget
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
				void init() override;
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
			protected:
				void onDraw() override;
			public:
				void calculateMinMaxSize() override;
				void onChangeSize() override;
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
				bool onEventEntry(const ewol::event::Entry& _event) override;
			protected:
				esignal::Connection m_PCH; //!< Periodic call handle to remove it when needed
				/**
				 * @brief Periodic call to update grapgic display
				 * @param[in] _event Time generic event
				 */
				void periodicCall(const ewol::event::Time& _event);
			protected:
				virtual void onChangePropertyShape();
				virtual void onChangePropertyValue();
		};
	};
};


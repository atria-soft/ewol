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
		using CheckBox = ememory::Ptr<ewol::widget::CheckBox>;
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
				ewol::compositing::Shaper this.shaper; //!< Compositing theme.
				boolean this.mouseHover; //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
				boolean this.buttonPressed; //!< Flag to know if the button is curently pressed.
				// hover area :
				Vector2f this.selectableAreaPos; //!< Start position of the events
				Vector2f this.selectableAreaSize; //!< size of the event positions
				// shaper ids:
				int this.shaperIdSize;
				int this.shaperIdSizeInsize;
			protected:
				/**
				 * Main checkbox ructor
				 * @param _shaperName Shaper file properties
				 */
				CheckBox();
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(CheckBox, "CheckBox");
				/**
				 * main destructor.
				 */
				 ~CheckBox();
			protected:
				/**
				 * internal system to change the property of the current status
				 * @param _newStatusId new state
				 */
				void changeStatusIn(int _newStatusId);
				/**
				 * update the status with the internal satte of the button ...
				 */
				void CheckStatus();
			protected:
				void onDraw() ;
			public:
				void calculateMinMaxSize() ;
				void onChangeSize() ;
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
				boolean onEventEntry( ewol::event::Entry _event) ;
			protected:
				esignal::Connection this.PCH; //!< Periodic call handle to remove it when needed
				/**
				 * Periodic call to update grapgic display
				 * @param _event Time generic event
				 */
				void periodicCall( ewol::event::Time _event);
			protected:
				 void onChangePropertyShape();
				 void onChangePropertyValue();
		};
	};
};


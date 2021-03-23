/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/ContextMenu.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/compositing/Shaper.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Manager.hpp>
#include <esignal/Signal.hpp>

namespace ewol {
	namespace widget {
		class ButtonColor;
		using ButtonColor = ememory::Ptr<ewol::widget::ButtonColor>;
		using ButtonColorWeak = ememory::WeakPtr<ewol::widget::ButtonColor>;
		class ButtonColor : public Widget {
			public: // signals
				esignal::Signal<etk::Color<>> signalChange;
			public: // properties
				eproperty::Value<etk::Color<>> propertyValue; //!< Current color.
				eproperty::Value<etk::Uri> propertyShape; //!< Current color.
			private:
				ewol::compositing::Shaper this.shaper; //!< Compositing theme.
				ewol::compositing::Text this.text; //!< Compositing Test display.
				ewol::widget::ContextMenu this.widgetContextMenu; //!< Specific context menu.
				boolean this.mouseHover; //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
				boolean this.buttonPressed; //!< Flag to know if the button is curently pressed.
				// hover area :
				Vector2f this.selectableAreaPos; //!< Start position of the events
				Vector2f this.selectableAreaSize; //!< size of the event positions
			protected:
				/**
				 * Main ructor.
				 * @param _baseColor basic displayed color.
				 * @param _shaperName The new shaper filename.
				 */
				ButtonColor();
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(ButtonColor, "ButtonColor");
				/**
				 * Main destructor.
				 */
				 ~ButtonColor();
			protected:
				void onDraw() ;
			public:
				void calculateMinMaxSize() ;
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
			private:
				/**
				 * internal system to change the property of the current status
				 * @param _newStatusId new state
				 */
				void changeStatusIn(int _newStatusId);
				esignal::Connection this.PCH; //!< Periodic call handle to remove it when needed
				/**
				 * Periodic call to update grapgic display
				 * @param _event Time generic event
				 */
				void periodicCall( ewol::event::Time _event);
				// Callback function:
				void onCallbackColorChange( etk::Color<> _color);
			protected:
				 void onChangePropertyValue();
				 void onChangePropertyShape();
		};
	};
};


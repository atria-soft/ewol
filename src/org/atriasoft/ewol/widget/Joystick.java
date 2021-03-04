/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Color.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <esignal/Signal.hpp>

// TODO : Rework ==> use property and shaper ...

namespace ewol {
	namespace widget {
		class Joystick;
		using Joystick = ememory::Ptr<ewol::widget::Joystick>;
		using JoystickWeak = ememory::WeakPtr<ewol::widget::Joystick>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Joystick :public Widget {
			public:
				// Event list of properties
				esignal::Signal<> signalEnable;
				esignal::Signal<> signalDisable;
				esignal::Signal<Vector2f> signalMove;
			public:
				enum joystickMode {
					modeNormal,
					modeArrow,
				};
			private:
				etk::Color<> this.colorFg; //!< Forground  color
				etk::Color<> this.colorBg; //!< Background color
				Vector2f this.displayPos; //!< direction of the cursor ...
				float this.distance; //!< dintance from the center
				float this.angle; //!< angle of the arraw (if < 0 : No arraw...) 0 is the TOP ...
				boolean this.lock; //!< flag to mark the lock when the cursor is free when we are outside the circle
				enum joystickMode this.displayMode; //!< Type of fonctionnal mode of the joystick
			private:
				// generic property of the joystick:
				boolean this.displayBackground;
				String this.background;
				String this.foreground;
				float this.ratio;
			protected:
				Joystick();
			public:
				DECLARE_WIDGET_FACTORY(Joystick, "Joystick");
				 ~Joystick();
			public:
				void setLockMode(boolean _lockWhenOut) {
					this.lock = _lockWhenOut;
				};
				void setDisplayMode(enum joystickMode _newMode) {
					this.displayMode = _newMode;
				};
				/**
				 * @brief set the ratio of the widget joystick
				 * @param[in] _newRatio the new ratio that might be set
				 */
				void ratio(float _newRatio);
				/**
				 * @brief set the Background of the widget joystick
				 * @param[in] _imageNameInData the new rbackground that might be set
				 * @param[in] _display
				 */
				void background(String _imageNameInData, boolean _display=true);
				/**
				 * @brief set the Foreground of the widget joystick
				 * @param[in] _imageNameInData the new Foreground that might be set
				 */
				void foreground(String _imageNameInData);
				/**
				 * @brief get the property of the joystick
				 * @param[out] _distance distance to the center
				 * @param[out] _angle angle of the joy
				 */
				void getProperty(float _distance, float _angle);
				
			public:
				 void onRegenerateDisplay() ;
				 boolean onEventInput( ewol::event::Input _event) ;
		};
	};
};


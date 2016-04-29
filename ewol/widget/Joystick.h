/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <esignal/ISignal.h>

// TODO : Rework ==> use property and shaper ...

namespace ewol {
	namespace widget {
		class Joystick;
		using JoystickShared = ememory::SharedPtr<ewol::widget::Joystick>;
		using JoystickWeak = ememory::WeakPtr<ewol::widget::Joystick>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Joystick :public ewol::Widget {
			public:
				// Event list of properties
				esignal::ISignal<> signalEnable;
				esignal::ISignal<> signalDisable;
				esignal::ISignal<vec2> signalMove;
			public:
				enum joystickMode {
					modeNormal,
					modeArrow,
				};
			private:
				etk::Color<> m_colorFg; //!< Forground  color
				etk::Color<> m_colorBg; //!< Background color
				vec2 m_displayPos; //!< direction of the cursor ...
				float m_distance; //!< dintance from the center
				float m_angle; //!< angle of the arraw (if < 0 : No arraw...) 0 is the TOP ...
				bool m_lock; //!< flag to mark the lock when the cursor is free when we are outside the circle
				enum joystickMode m_displayMode; //!< Type of fonctionnal mode of the joystick
			private:
				// generic property of the joystick:
				bool m_displayBackground;
				std::string m_background;
				std::string m_foreground;
				float m_ratio;
			protected:
				Joystick();
			public:
				DECLARE_WIDGET_FACTORY(Joystick, "Joystick");
				virtual ~Joystick();
			public:
				void setLockMode(bool _lockWhenOut) {
					m_lock = _lockWhenOut;
				};
				void setDisplayMode(enum joystickMode _newMode) {
					m_displayMode = _newMode;
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
				void background(std::string _imageNameInData, bool _display=true);
				/**
				 * @brief set the Foreground of the widget joystick
				 * @param[in] _imageNameInData the new Foreground that might be set
				 */
				void foreground(std::string _imageNameInData);
				/**
				 * @brief get the property of the joystick
				 * @param[out] _distance distance to the center
				 * @param[out] _angle angle of the joy
				 */
				void getProperty(float& _distance, float& _angle);
				
			public:
				virtual void onRegenerateDisplay() override;
				virtual bool onEventInput(const ewol::event::Input& _event) override;
		};
	};
};


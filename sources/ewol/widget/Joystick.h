/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_JOYSTICK_H__
#define __EWOL_JOYSTICK_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <draw/Color.h>


namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Joystick :public ewol::Widget {
			public:
				// Event list of properties
				static const char * const eventEnable;
				static const char * const eventDisable;
				static const char * const eventMove;
			public:
				enum joystickMode {
					modeNormal,
					modeArrow,
				};
			private:
				draw::Color m_colorFg; //!< Forground  color
				draw::Color m_colorBg; //!< Background color
				vec2  m_displayPos; //!< direction of the cursor ...
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
			public:
				Joystick();
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
				
			public: // Derived function
				virtual void calculateSize(const vec2& _availlable);
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
		};
	};
};

#endif

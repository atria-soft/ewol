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

extern const char * const ewolEventJoystickEnable;
extern const char * const ewolEventJoystickDisable;
extern const char * const ewolEventJoystickMove;

namespace widget {
	typedef enum {
		JOYSTICK_NORMAL_MODE,
		JOYSTICK_ARROW_MODE,
	} joystickMode_te;
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class Joystick :public ewol::Widget {
		private:
			draw::Color m_colorFg; //!< Forground  color
			draw::Color m_colorBg; //!< Background color
			vec2  m_displayPos; //!< direction of the cursor ...
			float m_distance; //!< dintance from the center
			float m_angle; //!< angle of the arraw (if < 0 : No arraw...) 0 is the TOP ...
			bool m_lock; //!< flag to mark the lock when the cursor is free when we are outside the circle
			joystickMode_te m_displayMode; //!< Type of fonctionnal mode of the joystick
		private:
			// generic property of the joystick:
			bool m_displayBackground;
			etk::UString m_background;
			etk::UString m_foreground;
			float m_ratio;
		public:
			Joystick(void);
			virtual ~Joystick(void);
			// Derived function
			virtual const char * const getObjectType(void) {
				return "widget::Joystick";
			};
			virtual void calculateSize(const vec2& availlable);
			virtual void onRegenerateDisplay(void);
			virtual bool onEventInput(const ewol::EventInput& _event);
			
			void setLockMode(bool lockWhenOut) { m_lock = lockWhenOut; };
			void setDisplayMode(joystickMode_te newMode) { m_displayMode = newMode; };
			/**
			 * @brief set the ratio of the widget joystick
			 * @param[in] newRatio the new ratio that might be set
			 */
			void ratio(float newRatio);
			
			/**
			 * @brief set the Background of the widget joystick
			 * @param[in] imageNameInData the new rbackground that might be set
			 */
			void background(etk::UString imageNameInData, bool display=true);
			
			/**
			 * @brief set the Foreground of the widget joystick
			 * @param[in] imageNameInData the new Foreground that might be set
			 */
			void foreground(etk::UString imageNameInData);
			
			/**
			 * @brief get the property of the joystick
			 * @param[out] distance distance to the center
			 * @param[out] angle angle of the joy
			 */
			void getProperty(float& distance, float& angle);
			
	};
};

#endif

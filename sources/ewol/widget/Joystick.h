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
#include <ewol/widget/Drawable.h>
#include <draw/Color.h>

extern const char * const ewolEventJoystickEnable;
extern const char * const ewolEventJoystickDisable;
extern const char * const ewolEventJoystickMove;

namespace widget {
	typedef enum {
		JOYSTICK_NORMAL_MODE,
		JOYSTICK_ARROW_MODE,
	} joystickMode_te;
	class Joystick :public widget::Drawable
	{
		private:
			draw::Color           m_colorFg;      //!< Forground  color
			draw::Color           m_colorBg;      //!< Background color
			vec2  m_displayPos;   //!< direction of the cursor ...
			float                 m_distance;     //!< dintance from the center
			float                 m_angle;        //!< angle of the arraw (if < 0 : No arraw...) 0 is the TOP ...
			bool                  m_lock;         //!< flag to mark the lock when the cursor is free when we are outside the circle
			joystickMode_te       m_displayMode;  //!< Type of fonctionnal mode of the joystick
		private:
			// generic property of the joystick:
			bool               m_displayBackground;
			etk::UString       m_background;
			etk::UString       m_foreground;
			float              m_ratio;
		public:
			Joystick(void);
			virtual ~Joystick(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::Joystick"; };
			virtual void CalculateSize(const vec2& availlable);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, const vec2& pos);
			
			void SetLockMode(bool lockWhenOut) { m_lock = lockWhenOut; };
			void SetDisplayMode(joystickMode_te newMode) { m_displayMode = newMode; };
			/**
			 * @brief Set the ratio of the widget joystick
			 * @param[in] newRatio the new ratio that might be set
			 * @return ---
			 */
			void Ratio(float newRatio);
			
			/**
			 * @brief Set the Background of the widget joystick
			 * @param[in] imageNameInData the new rbackground that might be set
			 * @return ---
			 */
			void Background(etk::UString imageNameInData, bool display=true);
			
			/**
			 * @brief Set the Foreground of the widget joystick
			 * @param[in] imageNameInData the new Foreground that might be set
			 * @return ---
			 */
			void Foreground(etk::UString imageNameInData);
			
			/**
			 * @brief Get the property of the joystick
			 * @param[out] distance distance to the center
			 * @param[out] angle angle of the joy
			 * @return ---
			 */
			void GetProperty(float& distance, float& angle);
			
	};
};

#endif

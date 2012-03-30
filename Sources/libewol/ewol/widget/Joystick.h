/**
 *******************************************************************************
 * @file ewol/widget/Joystick.h
 * @brief ewol Joystick widget system (header)
 * @author Edouard DUPIN
 * @date 30/03/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_JOYSTICK_H__
#define __EWOL_JOYSTICK_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventJoystickEnable;
extern const char * const ewolEventJoystickDisable;
extern const char * const ewolEventJoystickMove;

namespace ewol {
	typedef enum {
		JOYSTICK_NORMAL_MODE,
		JOYSTICK_ARROW_MODE,
	} joystickMode_te;
	class Joystick :public ewol::Drawable
	{
		private:
			color_ts           m_colorFg;      //!< Forground  color
			color_ts           m_colorBg;      //!< Background color
			coord2D_ts         m_displayPos;   //!< direction of the cursor ...
			etkFloat_t         m_distance;     //!< dintance from the center
			etkFloat_t         m_angle;        //!< angle of the arraw (if < 0 : No arraw...) 0 is the TOP ...
			bool               m_lock;         //!< flag to mark the lock when the cursor is free when we are outside the circle
			joystickMode_te    m_displayMode;  //!< Type of fonctionnal mode of the joystick
		public:
			Joystick(void);
			virtual ~Joystick(void);
			/**
			 * @brief Check if the object has the specific type.
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type of the object we want to check
			 * @return true if the object is compatible, otherwise false
			 */
			virtual bool CheckObjectType(const char * const objectType);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void);
			/**
			 * @brief Parrent set the possible diplay size of the current widget whith his own possibilities
			 *        By default this save the widget availlable size in the widget size
			 * @param[in] availlableX Availlable horisantal pixel size
			 * @param[in] availlableY Availlable vertical pixel size
			 * @return ---
			 */
			virtual bool CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY);
			virtual void   OnRegenerateDisplay(void);
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos);
			void SetLockMode(bool lockWhenOut) { m_lock = lockWhenOut; };
			void SetDisplayMode(joystickMode_te newMode) { m_displayMode = newMode; };
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_JOYSTICK;
	
};
#define EWOL_CAST_WIDGET_JOYSTICK(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_JOYSTICK,ewol::Button,curentPointer)

#endif
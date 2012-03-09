/**
 *******************************************************************************
 * @file ewol/widgetMeta/Keyboard.h
 * @brief ewol keyboard meta widget system (header)
 * @author Edouard DUPIN
 * @date 07/01/2012
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

#ifndef __EWOL_WIDGET_META_KEYBOARD_H__
#define __EWOL_WIDGET_META_KEYBOARD_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventKeyboardHide;

namespace ewol {
	typedef enum {
		KEYBOARD_MODE_TEXT,
		KEYBOARD_MODE_CODE,
	} keyboardMode_te;
	
	class Keyboard : public ewol::Drawable
	{
		public:
			Keyboard(void);
			~Keyboard(void);
			/**
			 * @brief Event generated to inform a flip-flop has occured on the current widget
			 * @param ---
			 * @return ---
			 */
			virtual void OnFlipFlopEvent(void);
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] CallerObject Pointer on the EObject that information came from
			 * @param[in] eventId Message registered by this class
			 * @param[in] data Data registered by this class
			 * @return ---
			 */
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @param[in] pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 */
			virtual ewol::Widget * GetWidgetAtPos(coord2D_ts pos);
			void SetMode(keyboardMode_te mode);
			bool IsHide(void) { return m_isHide; };
			void Hide(void) { m_isHide=true; };
			void Show(void) { m_isHide=false; };
		private:
			bool            m_isHide;
			keyboardMode_te m_mode;
			ewol::Widget*   m_subWidget[NB_BOUBLE_BUFFER];
		public:
			virtual bool   CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
		protected:
			virtual void   OnDraw(void);
		public:
			virtual void   OnRegenerateDisplay(void);
			virtual void   SetMinSise(etkFloat_t x=-1, etkFloat_t y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual void   SetExpendY(bool newExpend=false);
	};
};

#endif

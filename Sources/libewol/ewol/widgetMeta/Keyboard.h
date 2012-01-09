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

namespace ewol {
	typedef enum {
		KEYBOARD_MODE_TEXT,
		KEYBOARD_MODE_CODE,
	} keyboardMode_te;
	
	class Keyboard : public ewol::Widget
	{
		public:
			Keyboard(int32_t windowsID);
			~Keyboard(void);
			virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y);
			void SetMode(keyboardMode_te mode);
		private:
			keyboardMode_te m_mode;
			ewol::Widget* m_subWidget;
			int32_t       m_windowsID;
		public:
			virtual bool   CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
		protected:
			virtual bool   OnDraw(void);
		public:
			virtual void   OnRegenerateDisplay(void);
			virtual void   SetMinSise(etkFloat_t x=-1, etkFloat_t y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual void   SetExpendY(bool newExpend=false);
			virtual bool   OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
	};
};

#endif

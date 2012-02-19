/**
 *******************************************************************************
 * @file ewol/widget/Button.h
 * @brief ewol Button widget system (header)
 * @author Edouard DUPIN
 * @date 07/11/2011
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

#ifndef __EWOL_BUTTON_H__
#define __EWOL_BUTTON_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventButtonPressed;
extern const char * const ewolEventButtonEnter;
extern const char * const ewolEventButtonLeave;

namespace ewol {
	typedef enum {
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_CENTER,
	} textAlignement_te;
	class Button :public ewol::Drawable
	{
		public:
			Button(void);
			Button(etk::UString newLabel);
			void Init(void);
			virtual ~Button(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
			etk::UString   GetLabel(void) {return m_label;};
			void           SetValue(bool val);
			bool           GetValue(void);
			void           SetAlignement(textAlignement_te typeAlign);
			void           SetPadding(coord2D_ts newPadding);
		private:
			textAlignement_te  m_alignement;
			coord2D_ts         m_padding;
			etk::UString       m_label;
			color_ts           m_textColorFg;  //!< Text color
			color_ts           m_textColorBg;  //!< Background color
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
			virtual bool OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData);
	};
};

#endif

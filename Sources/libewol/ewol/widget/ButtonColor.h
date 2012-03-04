/**
 *******************************************************************************
 * @file ewol/widget/ButtonColor.h
 * @brief ewol Button select color widget system (header)
 * @author Edouard DUPIN
 * @date 04/03/2012
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

#ifndef __EWOL_BUTTON_COLOR_H__
#define __EWOL_BUTTON_COLOR_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>
#include <ewol/widget/Button.h>

extern const char * const ewolEventButtonColorChange;

namespace ewol {
	class ButtonColor :public ewol::Drawable
	{
		public:
			ButtonColor(void);
			ButtonColor(etk::UString newLabel);
			void Init(void);
			virtual ~ButtonColor(void);
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
			color_ts           m_textColorFg;    //!< Text color
			color_ts           m_textColorBg;    //!< Background color
			color_ts           m_selectedColor;  //!< user current selected Color
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos);
			color_ts GetCurrentColor(void) { return m_selectedColor; };
			void     SetCurrentColor(color_ts color);
	};
};

#endif

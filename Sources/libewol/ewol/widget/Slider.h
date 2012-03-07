/**
 *******************************************************************************
 * @file ewol/widget/Slider.h
 * @brief ewol Slider widget system (header)
 * @author Edouard DUPIN
 * @date 06/03/2012
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

#ifndef __EWOL_SLIDER_H__
#define __EWOL_SLIDER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventSliderChange;

namespace ewol {
	class Slider :public ewol::Drawable
	{
		public:
			Slider(void);
			virtual ~Slider(void);
			virtual bool   CalculateMinSize(void);
			void           SetValue(int32_t val);
			int32_t        GetValue(void);
			void           SetMin(int32_t val);
			void           SetMax(int32_t val);
			void           SetColor(color_ts newColor) { m_textColorFg = newColor; };
		private:
			int32_t            m_value;
			int32_t            m_min;
			int32_t            m_max;
			color_ts           m_textColorFg;  //!< Text color
			color_ts           m_textColorBg;  //!< Background color
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos);
	};
};

#endif

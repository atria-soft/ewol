/**
 *******************************************************************************
 * @file ewol/widget/ColorBar.h
 * @brief ewol ColorBar widget system (header)
 * @author Edouard DUPIN
 * @date 05/03/2012
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

#ifndef __EWOL_COLORBAR_H__
#define __EWOL_COLORBAR_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventColorBarChange;

namespace ewol {
	class ColorBar :public ewol::Drawable
	{
		public:
			ColorBar(void);
			virtual ~ColorBar(void);
			virtual bool   CalculateMinSize(void);
			color_ts GetCurrentColor(void);
			void     SetCurrentColor(color_ts newOne);
		private:
			color_ts   m_currentColor;
			coord2D_ts m_currentUserPos;
			coord2D_ts m_padding;
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos);
	};
};

#endif

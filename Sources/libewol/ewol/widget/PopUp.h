/**
 *******************************************************************************
 * @file ewol/widget/PopUp.h
 * @brief ewol pop-up widget system (header)
 * @author Edouard DUPIN
 * @date 29/12/2011
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

#ifndef __EWOL_POP_UP_H__
#define __EWOL_POP_UP_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>
#include <ewol/widget/Drawable.h>

namespace ewol {
	class PopUp : public ewol::Drawable
	{
		public:
			PopUp(void);
			virtual ~PopUp(void);
		public:
			virtual bool   CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
			virtual void   SetMinSise(etkFloat_t x=-1, etkFloat_t y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual void   SetExpendY(bool newExpend=false);
			void           SetDisplayRatio(etkFloat_t ratio);
		private:
			color_ts      m_colorBackGroung;
			color_ts      m_colorEmptyArea;
			ewol::Widget* m_subWidget[NB_BOUBLE_BUFFER];
			etkFloat_t    m_displayRatio;
		public:
			void           SubWidgetSet(ewol::Widget* newWidget);
			void           SubWidgetRemove(void);
		protected:
			virtual bool   OnDraw(void);
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			virtual bool   OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos);
			virtual void   OnFlipFlopEvent(void);
	};
};

#endif

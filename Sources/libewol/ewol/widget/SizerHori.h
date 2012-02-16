/**
 *******************************************************************************
 * @file ewol/widget/SizerHori.h
 * @brief ewol hirisantal sizer widget system (header)
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

#ifndef __EWOL_SIZER_HORI_H__
#define __EWOL_SIZER_HORI_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>

namespace ewol {
	class SizerHori :public ewol::Widget
	{
		public:
			SizerHori(void);
			virtual ~SizerHori(void);
		public:
			virtual bool   CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
			virtual void   SetMinSise(etkFloat_t x=-1, etkFloat_t y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual bool   CanExpentX(void);
			virtual void   SetExpendY(bool newExpend=false);
			virtual bool   CanExpentY(void);
			void           LockExpendContamination(bool lockExpend=false);
		private:
			bool                           m_lockExpendContamination;
			etk::VectorType<ewol::Widget*> m_subWidget;
		public:
			virtual void   SubWidgetRemoveAll(void);
			virtual void   SubWidgetAdd(ewol::Widget* newWidget);
			virtual void   SubWidgetRemove(ewol::Widget* newWidget);
			virtual void   SubWidgetUnLink(ewol::Widget* newWidget);
		protected:
			virtual bool   OnDraw(void);
		public:
			virtual void   OnRegenerateDisplay(void);
			virtual bool   OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
	};
};

#endif

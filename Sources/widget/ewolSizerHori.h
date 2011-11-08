/**
 *******************************************************************************
 * @file ewolSizerHori.h
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

#include <etkTypes.h>
#include <ewolDebug.h>
#include <ewolWidget.h>

namespace ewol {
	class SizerHori :public ewol::Widget
	{
		public:
			SizerHori(void);
			virtual ~SizerHori(void);
		public:
			virtual bool   CalculateSize(double availlableX, double availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
			virtual void   SetMinSise(double x=-1, double y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual void   SetExpendY(bool newExpend=false);
		private:
			etk::VectorType<ewol::Widget*> m_subWidget;
		public:
			void           SubWidgetRemoveAll(void);
			void           SubWidgetAdd(ewol::Widget* newWidget);
			void           SubWidgetRemove(ewol::Widget* newWidget);
			void           SubWidgetUnLink(ewol::Widget* newWidget);
		protected:
			virtual bool   OnDraw(void);
		public:
			virtual void   OnRegenerateDisplay(void);
	};
};

#endif

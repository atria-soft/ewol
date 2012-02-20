/**
 *******************************************************************************
 * @file ewol/widget/WidgetShortCut.h
 * @brief basic ewol shortCut widget (header)
 * @author Edouard DUPIN
 * @date 19/02/2012
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

#ifndef __EWOL_WIDGET_SHORT_CUT_H__
#define __EWOL_WIDGET_SHORT_CUT_H__

#include <ewol/Widget.h>


namespace ewol {
	
	typedef struct {
		const char * generateEventId; // event generate ID (to be unique it was pointer on the string name)
		bool shift;
		bool control;
		bool alt;
		bool meta;
		uint32_t UnicodeValue;
	} eventShortCut_ts;
	
	class WidgetShortCut : virtual public ewol::Widget {
		public:
			         WidgetShortCut(void);
			virtual ~WidgetShortCut(void);
		private:
			etk::VectorType<eventShortCut_ts> m_inputShortCutEvent;  //!< generic short-cut event
		protected:
			bool AddEventShortCut(bool shift, bool control, bool alt, bool pomme, uint32_t unicodeValue, const char * generateEventId);
			bool AddEventShortCut(char * descriptiveString, const char * generateEventId);
		public:
			virtual bool GenEventShortCut(bool shift, bool control, bool alt, bool meta, uint32_t unicodeValue);
	};
};


#endif


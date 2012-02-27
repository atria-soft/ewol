/**
 *******************************************************************************
 * @file ewol/widgetMeta/StdPopUP.ch
 * @brief ewol standard pop-up widget system (Header)
 * @author Edouard DUPIN
 * @date 01/01/2012
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

#ifndef __EWOL_STD_POP_UP_H__
#define __EWOL_STD_POP_UP_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Button.h>

extern const char * const ewolEventFileStdPopUpButton1;
extern const char * const ewolEventFileStdPopUpButton2;
extern const char * const ewolEventFileStdPopUpButton3;
extern const char * const ewolEventFileStdPopUpButton4;
extern const char * const ewolEventFileStdPopUpButton5;
extern const char * const ewolEventFileStdPopUpButton6;

namespace ewol {
	class StdPopUp : public ewol::PopUp
	{
		public:
			StdPopUp(void);
			~StdPopUp(void);
			virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y);
			void SetTitle(etk::UString text);
			void SetComment(etk::UString text);
			void SetButtonLabel(int32_t btId, etk::UString text); // note : if no label no bt...
		private:
			ewol::widget::Label*  m_title;
			ewol::widget::Label*  m_comment;
			ewol::widget::Button* m_button[6];
	};
};

#endif

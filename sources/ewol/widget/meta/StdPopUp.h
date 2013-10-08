/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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

namespace widget {
	class StdPopUp : public widget::PopUp {
		public:
			StdPopUp(void);
			~StdPopUp(void);
			// Derived function
			virtual const char * const getObjectType(void) { return "EwolPopUp"; };
			virtual bool onEventAreaExternal(int32_t _widgetID, const char *_generateEventId, const char *_eventExternId, float _x, float _y);
			void setTitle(etk::UString _text);
			void setComment(etk::UString _text);
			void setButtonLabel(int32_t _btId, etk::UString _text); // note : if no label no bt...
		private:
			ewol::widget::Label*  m_title;
			ewol::widget::Label*  m_comment;
			ewol::widget::Button* m_button[6];
	};
	
};

#endif

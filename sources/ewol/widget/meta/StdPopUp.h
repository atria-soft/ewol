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

namespace widget {
	class StdPopUp : public widget::PopUp {
		public:
			// Event list of properties
			static const char * const eventButton;
		public:
			StdPopUp(void);
			~StdPopUp(void);
			// Derived function
			virtual const char * const getObjectType(void) {
				return "ewol::StdPopUp";
			};
			void setTitle(const etk::UString& _text);
			void setComment(const etk::UString& _text);
			void addButton(const etk::UString& _text);
		private:
			widget::Label* m_title;
			widget::Label* m_comment;
			etk::Vector<widget::Button*> m_button;
	};
	
};

#endif

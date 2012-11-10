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

namespace ewol {
	class StdPopUp : public ewol::PopUp
	{
		public:
			StdPopUp(void);
			~StdPopUp(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolPopUp"; };
			virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, float x, float y);
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

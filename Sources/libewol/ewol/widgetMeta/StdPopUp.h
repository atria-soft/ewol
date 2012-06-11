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
			/**
			 * @brief Check if the object has the specific type.
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type of the object we want to check
			 * @return true if the object is compatible, otherwise false
			 */
			virtual bool CheckObjectType(const char * const objectType);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void);
			virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, float x, float y);
			void SetTitle(etk::UString text);
			void SetComment(etk::UString text);
			void SetButtonLabel(int32_t btId, etk::UString text); // note : if no label no bt...
		private:
			ewol::widget::Label*  m_title;
			ewol::widget::Label*  m_comment;
			ewol::widget::Button* m_button[6];
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_STD_POP_UP;
	
};

#define EWOL_CAST_WIDGET_STD_POP_UP(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_STD_POP_UP,ewol::StdPopUp,curentPointer)

#endif

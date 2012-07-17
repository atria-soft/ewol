/**
 *******************************************************************************
 * @file ewol/widgetMeta/FileChooser.h
 * @brief ewol File chooser meta widget system (header)
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

#ifndef __EWOL_FILE_CHOOSER_H__
#define __EWOL_FILE_CHOOSER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/CheckBox.h>

extern const char * const ewolEventParameterSelection;

namespace ewol {
	class Parameter : public ewol::PopUp
	{
		public:
			Parameter(void);
			~Parameter(void);
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
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] CallerObject Pointer on the EObject that information came from
			 * @param[in] eventId Message registered by this class
			 * @param[in] data Data registered by this class
			 * @return ---
			 */
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			
			void MenuAdd(etk::UString label, etk::UString ref, etk::String image);
			void MenuRm(etk::UString ref);
			void MenuClear(void);
			void MenuSeparator(void);
			void MenuGroup(etk::UString title, etk::UString image);
			void MenuGroupEnd(void);
			void SetSubWidget(ewol::Widget* newWidget);
		private:
			ewol::Label*          m_widgetTitle;
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_FILE_CHOOSER;
	
};

#define EWOL_CAST_WIDGET_FILE_CHOOSER(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_FILE_CHOOSER,ewol::FileChooser,curentPointer)

#endif

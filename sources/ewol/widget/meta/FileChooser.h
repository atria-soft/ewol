/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_FILE_CHOOSER_H__
#define __EWOL_FILE_CHOOSER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/ListFileSystem.h>

extern const char * const ewolEventFileChooserCancel;
extern const char * const ewolEventFileChooserValidate;

namespace ewol {
	class FileChooser : public widget::PopUp
	{
		public:
			FileChooser(void);
			virtual ~FileChooser(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolFileChooser"; };
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
			void SetTitle(etk::UString label);
			void SetValidateLabel(etk::UString label);
			void SetCancelLabel(etk::UString label);
			void SetFolder(etk::UString folder);
			void SetFileName(etk::UString filename);
			etk::UString GetCompleateFileName(void);
			void UpdateCurrentFolder(void);
		private:
			widget::Label*          m_widgetTitle;
			widget::Button*         m_widgetValidate;
			widget::Button*         m_widgetCancel;
			widget::Entry*          m_widgetCurrentFolder;
			widget::Entry*          m_widgetCurrentFileName;
			widget::ListFileSystem* m_widgetListFolder;
			widget::ListFileSystem* m_widgetListFile;
			widget::CheckBox*       m_widgetCheckBox;
			etk::UString          m_folder;
			etk::UString          m_file;
	};
	
};

#endif

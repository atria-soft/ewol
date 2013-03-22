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

namespace widget {
	class FileChooser : public widget::PopUp
	{
		public:
			FileChooser(void);
			virtual ~FileChooser(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolFileChooser"; };
			// Derived function
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data);
			// Derived function
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

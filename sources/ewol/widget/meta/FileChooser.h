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
			virtual const char * const getObjectType(void) { return "Ewol::fileChooser"; };
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
			virtual void onObjectRemove(ewol::EObject * removeObject);
			void setTitle(etk::UString label);
			void setValidateLabel(etk::UString label);
			void setCancelLabel(etk::UString label);
			void setFolder(etk::UString folder);
			void setFileName(etk::UString filename);
			etk::UString getCompleateFileName(void);
			void updateCurrentFolder(void);
		private:
			widget::Label* m_widgetTitle;
			widget::Button* m_widgetValidate;
			widget::Button* m_widgetCancel;
			widget::Entry* m_widgetCurrentFolder;
			widget::Entry* m_widgetCurrentFileName;
			widget::ListFileSystem* m_widgetListFolder;
			widget::ListFileSystem* m_widgetListFile;
			widget::CheckBox* m_widgetCheckBox;
			etk::UString m_folder;
			etk::UString m_file;
	};
	
};

#endif

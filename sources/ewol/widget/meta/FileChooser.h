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

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class FileChooser : public widget::PopUp {
		public:
			// Event list of properties
			static const char* const eventCancel;
			static const char* const eventValidate;
			// Config list of properties
		public:
			FileChooser(void);
			virtual ~FileChooser(void);
		private:
			widget::Label* m_widgetTitle;
			widget::Button* m_widgetValidate;
			widget::Button* m_widgetCancel;
			widget::Entry* m_widgetCurrentFolder;
			widget::Entry* m_widgetCurrentFileName;
			widget::ListFileSystem* m_widgetListFolder;
			widget::ListFileSystem* m_widgetListFile;
			widget::CheckBox* m_widgetCheckBox;
			std::string m_folder;
			std::string m_file;
		public:
			void setTitle(const std::string& _label);
			void setValidateLabel(const std::string& _label);
			void setCancelLabel(const std::string& _label);
			void setFolder(const std::string& _folder);
			void setFileName(const std::string& _filename);
			std::string getCompleateFileName(void);
			void updateCurrentFolder(void);
		public: // Derived function
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
			virtual void onObjectRemove(ewol::EObject* _removeObject);
	};
	
};

#endif

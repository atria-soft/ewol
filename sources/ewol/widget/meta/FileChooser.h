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
#include <ewol/widget/Composer.h>

namespace ewol {
	namespace widget {
		/**
		 *  @brief File Chooser is a simple selector of file for opening, saving, and what you want ...
		 *  
		 *  As all other pop-up methode ( wost case we can have) the creating is simple , but event back is not all the time simple:
		 *  
		 *  Fist global static declaration and inclusion:
		 *  [code style=c++]
		 *  #include <ewol/widget/meta/FileChooser.h>
		 *  static const char* const g_eventOpenFile = "appl-local-event-open-file";
		 *  static const char* const g_eventClosePopUp = "appl-local-event-close-pop-up";
		 *  [/code]
		 *  
		 *  The first step is to create the file chooser pop-up :
		 *  [code style=c++]
		 *  ewol::widget::FileChooser* tmpWidget = new ewol::Widget::FileChooser();
		 *  if (tmpWidget == nullptr) {
		 *  	APPL_ERROR("Can not open File chooser !!! ");
		 *  	return -1;
		 *  }
		 *  // register on the Validate event:
		 *  tmpWidget->registerOnEvent(this, "validate", g_eventOpenFile);
		 *  // no need of this event watching ...
		 *  tmpWidget->registerOnEvent(this, "cancel", g_eventClosePopUp);
		 *  // set the title:
		 *   tmpWidget->setTitle("Open files ...");
		 *  // Set the validate Label:
		 *  tmpWidget->setValidateLabel("Open");
		 *  // simply set a folder (by default this is the home folder)
		 *  //tmpWidget->setFolder("/home/me");
		 *  // add the widget as windows pop-up ...
		 *  ewol::widget::Windows* tmpWindows = getWindows();
		 *  if (tmpWindows == nullptr) {
		 *  	APPL_ERROR("Can not get the current windows !!! ");
		 *  	return -1;
		 *  }
		 *  tmpWindows->popUpWidgetPush(tmpWidget);
		 *  [/code]
		 *  
		 *  Now we just need to wait the the open event message.
		 *  
		 *  [code style=c++]
		 *  void ********::onReceiveMessage(const ewol::object::Message& _msg) {
		 *  	APPL_DEBUG("Receive Message: " << _msg );
		 *  	if (_msg.getMessage() == g_eventOpenFile) {
		 *  		APPL_INFO("Request open file : '" << _msg.getData() << "'");
		 *  		return;
		 *  	}
		 *  	if (_msg.getMessage() == g_eventClosePopUp) {
		 *  		APPL_INFO("The File chooser has been closed");
		 *  		// generaly nothing to do ...
		 *  		return;
		 *  	}
		 *  }
		 *  [/code]
		 *  This is the best example of a Meta-widget.
		 */
		class FileChooser : public ewol::widget::Composer {
			public:
				// Event list of properties
				static const char* const eventCancel;
				static const char* const eventValidate;
				// Config list of properties
			public:
				FileChooser();
				virtual ~FileChooser();
			private:
				std::string m_folder;
				std::string m_file;
			public:
				void setTitle(const std::string& _label);
				void setValidateLabel(const std::string& _label);
				void setCancelLabel(const std::string& _label);
				void setFolder(const std::string& _folder);
				void setFileName(const std::string& _filename);
				std::string getCompleateFileName();
				void updateCurrentFolder();
			public: // Derived function
				virtual void onReceiveMessage(const ewol::object::Message& _msg);
				virtual void onGetFocus();
		};
	};
};

#endif

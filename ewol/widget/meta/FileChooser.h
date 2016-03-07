/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Composer.h>
#include <esignal/ISignal.h>

namespace ewol {
	namespace widget {
		class FileChooser;
		using FileChooserShared = ewol::SharedPtr<ewol::widget::FileChooser>;
		using FileChooserWeak = ewol::WeakPtr<ewol::widget::FileChooser>;
		/**
		 *  @brief File Chooser is a simple selector of file for opening, saving, and what you want ...
		 *  
		 *  As all other pop-up methode ( wost case we can have) the creating is simple , but event back is not all the time simple:
		 *  
		 *  Fist global static declaration and inclusion:
		 *  [code style=c++]
		 *  #include <ewol/widget/meta/FileChooser.h>
		 *  [/code]
		 *  
		 *  The first step is to create the file chooser pop-up : (never in the constructor!!!)
		 *  [code style=c++]
		 *  ewol::widget::FileChooserShared tmpWidget = ewol::Widget::FileChooser::create();
		 *  if (tmpWidget == nullptr) {
		 *  	APPL_ERROR("Can not open File chooser !!! ");
		 *  	return -1;
		 *  }
		 *  // register on the Validate event:
		 *  tmpWidget->signalValidate.connect(shared_from_this(), &****::onCallbackOpenFile);
		 *  // no need of this event watching ...
		 *  tmpWidget->signalCancel.connect(shared_from_this(), &****::onCallbackClosePopUp);
		 *  // set the title:
		 *   tmpWidget->setTitle("Open files ...");
		 *  // Set the validate Label:
		 *  tmpWidget->setValidateLabel("Open");
		 *  // simply set a folder (by default this is the home folder)
		 *  //tmpWidget->setFolder("/home/me");
		 *  // add the widget as windows pop-up ...
		 *  ewol::widget::WindowsShared tmpWindows = getWindows();
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
		 *  void ****::onCallbackOpenFile(const std::string& _value) {
		 *  	APPL_INFO("Request open file : '" << _value << "'");
		 *  }
		 *  void ****::onCallbackClosePopUp() {
		 *  	APPL_INFO("The File chooser has been closed");
		 *  }
		 *  [/code]
		 *  This is the best example of a Meta-widget.
		 */
		class FileChooser : public ewol::widget::Composer {
			public: // signals
				esignal::ISignal<> signalCancel;
				esignal::ISignal<std::string> signalValidate;
			public: // properties
				eproperty::Value<std::string> propertyPath;
				eproperty::Value<std::string> propertyFile;
				eproperty::Value<std::string> propertyLabelTitle;
				eproperty::Value<std::string> propertyLabelValidate;
				eproperty::Value<std::string> propertyLabelCancel;
			protected:
				FileChooser();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(FileChooser, "FileChooser");
				virtual ~FileChooser();
			private:
				std::string getCompleateFileName();
				void updateCurrentFolder();
			public: // Derived function
				virtual void onGetFocus();
			private:
				// callback functions:
				void onCallbackEntryFolderChangeValue(const std::string& _value);
				void onCallbackEntryFileChangeValue(const std::string& _value);
				void onCallbackButtonCancelPressed();
				void onCallbackHidenFileChangeChangeValue(const bool& _value);
				void onCallbackListFolderSelectChange(const std::string& _value);
				void onCallbackListFileSelectChange(const std::string& _value);
				void onCallbackListFileValidate(const std::string& _value);
				void onCallbackListValidate();
				void onCallbackHomePressed();
			protected:
				virtual void onChangePropertyPath();
				virtual void onChangePropertyFile();
				virtual void onChangePropertyLabelTitle();
				virtual void onChangePropertyLabelValidate();
				virtual void onChangePropertyLabelCancel();
		};
	};
};

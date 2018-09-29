/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Composer.hpp>
#include <esignal/Signal.hpp>

namespace ewol {
	namespace widget {
		class FileChooser;
		using FileChooserShared = ememory::SharedPtr<ewol::widget::FileChooser>;
		using FileChooserWeak = ememory::WeakPtr<ewol::widget::FileChooser>;
		/**
		 *  @brief File Chooser is a simple selector of file for opening, saving, and what you want ...
		 *  
		 *  As all other pop-up methode ( wost case we can have) the creating is simple , but event back is not all the time simple:
		 *  
		 *  Fist global static declaration and inclusion:
		 *  [code style=c++]
		 *  #include <ewol/widget/meta/FileChooser.hpp>
		 *  [/code]
		 *  
		 *  The first step is to create the file chooser pop-up : (never in the constructor!!!)
		 *  [code style=c++]
		 *  ewol::widget::FileChooserShared tmpWidget = ewol::widget::FileChooser::create();
		 *  if (tmpWidget == null) {
		 *  	APPL_ERROR("Can not open File chooser !!! ");
		 *  	return -1;
		 *  }
		 *  // register on the Validate event:
		 *  tmpWidget->signalValidate.connect(sharedFromThis(), &****::onCallbackOpenFile);
		 *  // no need of this event watching ...
		 *  tmpWidget->signalCancel.connect(sharedFromThis(), &****::onCallbackClosePopUp);
		 *  // set the title:
		 *   tmpWidget->propertyLabelTitle.set("Open files ...");
		 *  // Set the validate Label:
		 *  tmpWidget->propertyLabelValidate.set("Open");
		 *  // simply set a folder (by default this is the home folder)
		 *  //tmpWidget->propertyPath.set("/home/me");
		 *  // add the widget as windows pop-up ...
		 *  ewol::widget::WindowsShared tmpWindows = getWindows();
		 *  if (tmpWindows == null) {
		 *  	APPL_ERROR("Can not get the current windows !!! ");
		 *  	return -1;
		 *  }
		 *  tmpWindows->popUpWidgetPush(tmpWidget);
		 *  [/code]
		 *  
		 *  Now we just need to wait the the open event message.
		 *  
		 *  [code style=c++]
		 *  void ****::onCallbackOpenFile(const etk::String& _value) {
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
				esignal::Signal<> signalCancel; //!< abort the display of the pop-up or press cancel button
				esignal::Signal<etk::Path> signalValidate; //!< select file(s)
			public: // properties
				eproperty::Value<etk::Path> propertyPath; //!< Current path to explore
				eproperty::Value<etk::Path> propertyFile; //!< Selected file
				eproperty::Value<etk::String> propertyLabelTitle; //!< Label of the pop-up (can use translation)
				eproperty::Value<etk::String> propertyLabelValidate; //!< Label of validate button of the pop-up (can use translation)
				eproperty::Value<etk::String> propertyLabelCancel; //!< Label of cancel/close button of the pop-up (can use translation)
			protected:
				FileChooser();
				void init() override;
			public:
				DECLARE_WIDGET_FACTORY(FileChooser, "FileChooser");
				virtual ~FileChooser();
			private:
				etk::Path getCompleateFileName();
				void updateCurrentFolder();
			public:
				void onGetFocus() override;
			private:
				// callback functions:
				void onCallbackEntryFolderChangeValue(const etk::String& _value);
				void onCallbackEntryFileChangeValue(const etk::String& _value);
				void onCallbackEntryFileChangeValidate(const etk::String& _value);
				void onCallbackButtonCancelPressed();
				void onCallbackHidenFileChangeChangeValue(const bool& _value);
				void onCallbackListFolderSelectChange(const etk::Path& _value);
				void onCallbackListFileSelectChange(const etk::Path& _value);
				void onCallbackListFileValidate(const etk::Path& _value);
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

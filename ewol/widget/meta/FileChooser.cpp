/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/widget/meta/FileChooser.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/List.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/CheckBox.hpp>
#include <ewol/widget/ListFileSystem.hpp>
#include <ewol/widget/Entry.hpp>
#include <ewol/widget/Spacer.hpp>
#include <ewol/widget/Image.hpp>
#include <ewol/widget/Composer.hpp>
#include <ewol/widget/Manager.hpp>
//#include <etk/Vector.hpp>
#include <etk/Vector.hpp>
#include <etk/tool.hpp>
#include <etk/os/FSNode.hpp>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#include <ewol/ewol.hpp>

ewol::widget::FileChooser::FileChooser() :
  signalCancel(this, "cancel", ""),
  signalValidate(this, "validate", ""),
  propertyPath(this, "path",
                     etk::getUserHomeFolder(),
                     "",
                     &ewol::widget::FileChooser::onChangePropertyPath),
  propertyFile(this, "file",
                     "",
                     "",
                     &ewol::widget::FileChooser::onChangePropertyFile),
  propertyLabelTitle(this, "title",
                           "_T{FileChooser}",
                           "",
                           &ewol::widget::FileChooser::onChangePropertyLabelTitle),
  propertyLabelValidate(this, "label-validate",
                              "_T{Validate}",
                              "",
                              &ewol::widget::FileChooser::onChangePropertyLabelValidate),
  propertyLabelCancel(this, "label-cancel",
                            "_T{Cancel}",
                            "",
                            &ewol::widget::FileChooser::onChangePropertyLabelCancel) {
	addObjectType("ewol::widget::FileChooser");
}

void ewol::widget::FileChooser::init() {
	ewol::widget::Composer::init();
	// Load file with replacing the "{ID}" with the local ID of the widget ==> obtain unique ID
	loadFromFile("{ewol}DATA:ewol-gui-file-chooser.xml", getId());
	// Basic replacement of labels
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:title-label", "value", propertyLabelTitle);
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:validate-label", "value", propertyLabelValidate);
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:cancel-label", "value", propertyLabelCancel);
	
	subBind(ewol::widget::CheckBox, "[" + etk::toString(getId()) + "]file-shooser:show-hiden-file", signalValue, sharedFromThis(), &ewol::widget::FileChooser::onCallbackHidenFileChangeChangeValue);
	subBind(ewol::widget::Button, "[" + etk::toString(getId()) + "]file-shooser:button-validate", signalPressed, sharedFromThis(), &ewol::widget::FileChooser::onCallbackListValidate);
	subBind(ewol::widget::Button, "[" + etk::toString(getId()) + "]file-shooser:button-cancel", signalPressed, sharedFromThis(), &ewol::widget::FileChooser::onCallbackButtonCancelPressed);
	subBind(ewol::widget::ListFileSystem, "[" + etk::toString(getId()) + "]file-shooser:list-folder", signalFolderValidate, sharedFromThis(), &ewol::widget::FileChooser::onCallbackListFolderSelectChange);
	subBind(ewol::widget::ListFileSystem, "[" + etk::toString(getId()) + "]file-shooser:list-files", signalFileSelect, sharedFromThis(), &ewol::widget::FileChooser::onCallbackListFileSelectChange);
	subBind(ewol::widget::ListFileSystem, "[" + etk::toString(getId()) + "]file-shooser:list-files", signalFileValidate, sharedFromThis(), &ewol::widget::FileChooser::onCallbackListFileValidate);
	subBind(ewol::widget::Entry, "[" + etk::toString(getId()) + "]file-shooser:entry-file", signalModify, sharedFromThis(), &ewol::widget::FileChooser::onCallbackEntryFileChangeValue);
	subBind(ewol::widget::Entry, "[" + etk::toString(getId()) + "]file-shooser:entry-file", signalEnter, sharedFromThis(), &ewol::widget::FileChooser::onCallbackListFileValidate);
	subBind(ewol::widget::Entry, "[" + etk::toString(getId()) + "]file-shooser:entry-folder", signalModify, sharedFromThis(), &ewol::widget::FileChooser::onCallbackEntryFolderChangeValue);
	//composerBind(ewol::widget::CheckBox, "[" + etk::toString(getId()) + "]file-shooser:entry-folder", signalEnter, sharedFromThis(), &ewol::widget::FileChooser::);
	subBind(ewol::widget::Image, "[" + etk::toString(getId()) + "]file-shooser:img-home", signalPressed, sharedFromThis(), &ewol::widget::FileChooser::onCallbackHomePressed);
	// set the default Folder properties:
	updateCurrentFolder();
	propertyCanFocus.set(true);
}

void ewol::widget::FileChooser::onGetFocus() {
	// transfert focus on a specific widget...
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:entry-file", "focus", "true");
}

ewol::widget::FileChooser::~FileChooser() {
	
}

void ewol::widget::FileChooser::onChangePropertyPath() {
	propertyPath.getDirect() = *propertyPath + "/";
	updateCurrentFolder();
}

void ewol::widget::FileChooser::onChangePropertyFile() {
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:entry-file", "value", propertyFile);
	//updateCurrentFolder();
}

void ewol::widget::FileChooser::onChangePropertyLabelTitle() {
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:title-label", "value", propertyLabelTitle);
}

void ewol::widget::FileChooser::onChangePropertyLabelValidate() {
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:validate-label", "value", propertyLabelValidate);
}

void ewol::widget::FileChooser::onChangePropertyLabelCancel() {
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:cancel-label", "value", propertyLabelCancel);
}

void ewol::widget::FileChooser::onCallbackEntryFolderChangeValue(const etk::String& _value) {
	// == > change the folder name
	// TODO : change the folder, if it exit ...
}

void ewol::widget::FileChooser::onCallbackEntryFileChangeValue(const etk::String& _value) {
	// == > change the file name
	propertyFile.setDirect(_value);
	// update the selected file in the list :
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:list-files", "select", propertyFile);
}

void ewol::widget::FileChooser::onCallbackButtonCancelPressed() {
	// == > Auto remove ...
	signalCancel.emit();
	autoDestroy();
}

void ewol::widget::FileChooser::onCallbackHidenFileChangeChangeValue(const bool& _value) {
	if (_value == true) {
		propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:list-folder", "show-hidden", "true");
		propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:list-files", "show-hidden", "true");
	} else {
		propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:list-folder", "show-hidden", "false");
		propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:list-files", "show-hidden", "false");
	}
}

void ewol::widget::FileChooser::onCallbackListFolderSelectChange(const etk::String& _value) {
	// == > this is an internal event ...
	EWOL_DEBUG(" old PATH: '" << *propertyPath << "' + '" << _value << "'");
	propertyPath.setDirect(propertyPath.get() + _value);
	EWOL_DEBUG("new PATH: '" << *propertyPath << "'");
	propertyPath.setDirect(etk::simplifyPath(*propertyPath));
	propertyFile.setDirect("");
	updateCurrentFolder();
}

void ewol::widget::FileChooser::onCallbackListFileSelectChange(const etk::String& _value) {
	propertyFile.set(_value);
	/*
	etk::String tmpFileCompleatName = m_folder;
	tmpFileCompleatName += m_file;
	// TODO : generateEventId(_msg.getMessage(), tmpFileCompleatName);
	*/
}

void ewol::widget::FileChooser::onCallbackListFileValidate(const etk::String& _value) {
	// select the file  == > generate a validate
	propertyFile.set(_value);
	EWOL_VERBOSE(" generate a fiel opening : '" << propertyPath << "' / '" << propertyFile << "'");
	signalValidate.emit(getCompleateFileName());
	autoDestroy();
}

void ewol::widget::FileChooser::onCallbackListValidate() {
	if (propertyFile.get() == "") {
		EWOL_WARNING(" Validate : '" << propertyPath << "' / '" << propertyFile << "' ==> error No name ...");
		return;
	}
	EWOL_DEBUG(" generate a file opening : '" << propertyPath << "' / '" << propertyFile << "'");
	signalValidate.emit(getCompleateFileName());
	autoDestroy();
}

void ewol::widget::FileChooser::onCallbackHomePressed() {
	etk::String tmpUserFolder = etk::getUserHomeFolder();
	EWOL_DEBUG("new PATH : \"" << tmpUserFolder << "\"");
	
	propertyPath.setDirect(etk::simplifyPath(tmpUserFolder));
	
	propertyFile.setDirect("");
	updateCurrentFolder();
}

void ewol::widget::FileChooser::updateCurrentFolder() {
	if (*propertyPath != "") {
		if (propertyPath.get()[propertyPath->size()-1] != '/') {
			propertyPath.getDirect() +=  "/";
		}
	}
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:list-files", "path", propertyPath);
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:list-folder", "path", propertyPath);
	propertySetOnWidgetNamed("[" + etk::toString(getId()) + "]file-shooser:entry-folder", "value", propertyPath);
	markToRedraw();
}

etk::String ewol::widget::FileChooser::getCompleateFileName() {
	etk::String tmpString = propertyPath;
	tmpString += "/";
	tmpString += propertyFile;
	etk::FSNode node(tmpString);
	return node.getName();
}

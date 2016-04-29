/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */


#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/ListFileSystem.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Image.h>
#include <ewol/widget/Composer.h>
#include <ewol/widget/Manager.h>
//#include <vector>
#include <vector>
#include <etk/tool.h>
#include <etk/os/FSNode.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#include <ewol/ewol.h>

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
	std::string myDescription = std::string("")
	      + "<popup >\n"
	      + "	<sizer mode='vert' lock='true' fill='true' expand='true'>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<checkbox name='[" + etk::to_string(getId()) + "]file-shooser:show-hiden-file'>\n"
	      + "				<label>_T{ShowHiddenFiles}</label>\n"
	      + "			</checkbox>\n"
	      + "			<spacer expand='true,false'/>\n"
	      + "			<button name='[" + etk::to_string(getId()) + "]file-shooser:button-validate'>\n"
	      + "				<sizer mode='hori'>\n"
	      + "					<image src='{ewol}THEME:GUI:Load.edf' fill='true' size='7,7mm' distance-field='true'/>\n"
	      + "					<label name='[" + etk::to_string(getId()) + "]file-shooser:validate-label'>" + *propertyLabelValidate + "</label>\n"
	      + "				</sizer>\n"
	      + "			</button>\n"
	      + "			<button name='[" + etk::to_string(getId()) + "]file-shooser:button-cancel'>\n"
	      + "				<sizer mode='hori'>\n"
	      + "					<image src='{ewol}THEME:GUI:Remove.edf' fill='true' size='7,7mm' distance-field='true'/>\n"
	      + "					<label name='[" + etk::to_string(getId()) + "]file-shooser:cancel-label'>" + *propertyLabelCancel + "</label>\n"
	      + "				</sizer>\n"
	      + "			</button>\n"
	      + "		</sizer>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<spacer min-size='2,2mm'/>\n"
	      + "			<ListFileSystem name='[" + etk::to_string(getId()) + "]file-shooser:list-folder'\n"
	      + "			                min-size='20,0%'\n"
	      + "			                expand='false,true'\n"
	      + "			                show-hidden='false'\n"
	      + "			                show-file='false'\n"
	      + "			                show-folder='true'\n"
	      + "			                show-temporary='false'/>\n"
	      + "			<spacer min-size='2,2mm'/>\n"
	      + "			<ListFileSystem name='[" + etk::to_string(getId()) + "]file-shooser:list-files'\n"
	      + "			                expand='true,true'\n"
	      + "			                show-hidden='false'\n"
	      + "			                show-file='true'\n"
	      + "			                show-folder='false'\n"
	      + "			                show-temporary='false'/>\n"
	      + "			<spacer min-size='2,2mm'/>\n"
	      + "		</sizer>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<image name='[" + etk::to_string(getId()) + "]file-shooser:img-file' src='{ewol}THEME:GUI:File.edf' expand='false' size='8,8mm' distance-field='true'/>\n"
	      + "			<entry name='[" + etk::to_string(getId()) + "]file-shooser:entry-file' expand='true,false' fill='true,false'/>\n"
	      + "		</sizer>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<image name='[" + etk::to_string(getId()) + "]file-shooser:img-folder' src='{ewol}THEME:GUI:Folder.edf' expand='false' size='8,8mm' distance-field='true'/>\n"
	      + "			<entry name='[" + etk::to_string(getId()) + "]file-shooser:entry-folder' expand='true,false' fill='true,false'/>\n"
	      + "			<image name='[" + etk::to_string(getId()) + "]file-shooser:img-home' src='{ewol}THEME:GUI:Home.edf' expand='false' size='8,8mm' distance-field='true'/>\n"
	      + "		</sizer>\n"
	      + "		<label name='[" + etk::to_string(getId()) + "]file-shooser:title-label'>" + *propertyLabelTitle + "</label>\n"
	      + "	</sizer>\n"
	      + "</popup>";
	loadFromString(myDescription);
	subBind(ewol::widget::CheckBox, "[" + etk::to_string(getId()) + "]file-shooser:show-hiden-file", signalValue, shared_from_this(), &ewol::widget::FileChooser::onCallbackHidenFileChangeChangeValue);
	subBind(ewol::widget::Button, "[" + etk::to_string(getId()) + "]file-shooser:button-validate", signalPressed, shared_from_this(), &ewol::widget::FileChooser::onCallbackListValidate);
	subBind(ewol::widget::Button, "[" + etk::to_string(getId()) + "]file-shooser:button-cancel", signalPressed, shared_from_this(), &ewol::widget::FileChooser::onCallbackButtonCancelPressed);
	subBind(ewol::widget::ListFileSystem, "[" + etk::to_string(getId()) + "]file-shooser:list-folder", signalFolderValidate, shared_from_this(), &ewol::widget::FileChooser::onCallbackListFolderSelectChange);
	subBind(ewol::widget::ListFileSystem, "[" + etk::to_string(getId()) + "]file-shooser:list-files", signalFileSelect, shared_from_this(), &ewol::widget::FileChooser::onCallbackListFileSelectChange);
	subBind(ewol::widget::ListFileSystem, "[" + etk::to_string(getId()) + "]file-shooser:list-files", signalFileValidate, shared_from_this(), &ewol::widget::FileChooser::onCallbackListFileValidate);
	subBind(ewol::widget::Entry, "[" + etk::to_string(getId()) + "]file-shooser:entry-file", signalModify, shared_from_this(), &ewol::widget::FileChooser::onCallbackEntryFileChangeValue);
	subBind(ewol::widget::Entry, "[" + etk::to_string(getId()) + "]file-shooser:entry-file", signalEnter, shared_from_this(), &ewol::widget::FileChooser::onCallbackListFileValidate);
	subBind(ewol::widget::Entry, "[" + etk::to_string(getId()) + "]file-shooser:entry-folder", signalModify, shared_from_this(), &ewol::widget::FileChooser::onCallbackEntryFolderChangeValue);
	//composerBind(ewol::widget::CheckBox, "[" + etk::to_string(getId()) + "]file-shooser:entry-folder", signalEnter, shared_from_this(), &ewol::widget::FileChooser::);
	subBind(ewol::widget::Image, "[" + etk::to_string(getId()) + "]file-shooser:img-home", signalPressed, shared_from_this(), &ewol::widget::FileChooser::onCallbackHomePressed);
	// set the default Folder properties:
	updateCurrentFolder();
	propertyCanFocus.set(true);
}

void ewol::widget::FileChooser::onGetFocus() {
	// transfert focus on a specific widget...
	propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:entry-file", "focus", "true");
}

ewol::widget::FileChooser::~FileChooser() {
	
}

void ewol::widget::FileChooser::onChangePropertyPath() {
	propertyPath.getDirect() = *propertyPath + "/";
	updateCurrentFolder();
}

void ewol::widget::FileChooser::onChangePropertyFile() {
	propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:entry-file", "value", propertyFile);
	//updateCurrentFolder();
}

void ewol::widget::FileChooser::onChangePropertyLabelTitle() {
	propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:title-label", "value", propertyLabelTitle);
}

void ewol::widget::FileChooser::onChangePropertyLabelValidate() {
	propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:validate-label", "value", propertyLabelValidate);
}

void ewol::widget::FileChooser::onChangePropertyLabelCancel() {
	propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:cancel-label", "value", propertyLabelCancel);
}

void ewol::widget::FileChooser::onCallbackEntryFolderChangeValue(const std::string& _value) {
	// == > change the folder name
	// TODO : change the folder, if it exit ...
}

void ewol::widget::FileChooser::onCallbackEntryFileChangeValue(const std::string& _value) {
	// == > change the file name
	propertyFile.setDirect(_value);
	// update the selected file in the list :
	propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-files", "select", propertyFile);
}

void ewol::widget::FileChooser::onCallbackButtonCancelPressed() {
	// == > Auto remove ...
	signalCancel.emit();
	autoDestroy();
}

void ewol::widget::FileChooser::onCallbackHidenFileChangeChangeValue(const bool& _value) {
	if (_value == true) {
		propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-folder", "show-hidden", "true");
		propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-files", "show-hidden", "true");
	} else {
		propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-folder", "show-hidden", "false");
		propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-files", "show-hidden", "false");
	}
}

void ewol::widget::FileChooser::onCallbackListFolderSelectChange(const std::string& _value) {
	// == > this is an internal event ...
	EWOL_DEBUG(" old PATH: '" << *propertyPath << "' + '" << _value << "'");
	propertyPath.setDirect(propertyPath.get() + _value);
	EWOL_DEBUG("new PATH: '" << *propertyPath << "'");
	propertyPath.setDirect(etk::simplifyPath(*propertyPath));
	propertyFile.setDirect("");
	updateCurrentFolder();
}

void ewol::widget::FileChooser::onCallbackListFileSelectChange(const std::string& _value) {
	propertyFile.set(_value);
	/*
	std::string tmpFileCompleatName = m_folder;
	tmpFileCompleatName += m_file;
	// TODO : generateEventId(_msg.getMessage(), tmpFileCompleatName);
	*/
}

void ewol::widget::FileChooser::onCallbackListFileValidate(const std::string& _value) {
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
	std::string tmpUserFolder = etk::getUserHomeFolder();
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
	propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-files", "path", propertyPath);
	propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-folder", "path", propertyPath);
	propertySetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:entry-folder", "value", propertyPath);
	markToRedraw();
}

std::string ewol::widget::FileChooser::getCompleateFileName() {
	std::string tmpString = propertyPath;
	tmpString += "/";
	tmpString += propertyFile;
	etk::FSNode node(tmpString);
	return node.getName();
}

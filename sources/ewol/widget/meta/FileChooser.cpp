/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
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

#undef __class__
#define __class__ "FileChooser"

ewol::widget::FileChooser::FileChooser() :
  signalCancel(*this, "cancel"),
  signalValidate(*this, "validate") {
	addObjectType("ewol::widget::FileChooser");
}

void ewol::widget::FileChooser::init() {
	ewol::widget::Composer::init();
	m_folder = etk::getUserHomeFolder();
	m_file = "";
	std::string myDescription = std::string("")
	      + "<popup >\n"
	      + "	<sizer mode='vert' lock='true' fill='true' expand='true'>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<checkbox name='[" + etk::to_string(getId()) + "]file-shooser:show-hiden-file'>\n"
	      + "				<label>" + TRANSLATE("ShowHiddenFiles") + "</label>\n"
	      + "			</checkbox>\n"
	      + "			<spacer expand='true,false'/>\n"
	      + "			<button name='[" + etk::to_string(getId()) + "]file-shooser:button-validate'>\n"
	      + "				<sizer mode='hori'>\n"
	      + "					<image src='THEME:GUI:Load.edf' fill='true' size='7,7mm' distance-field='true'/>\n"
	      + "					<label name='[" + etk::to_string(getId()) + "]file-shooser:validate-label'>" + TRANSLATE("Validate") + "</label>\n"
	      + "				</sizer>\n"
	      + "			</button>\n"
	      + "			<button name='[" + etk::to_string(getId()) + "]file-shooser:button-cancel'>\n"
	      + "				<sizer mode='hori'>\n"
	      + "					<image src='THEME:GUI:Remove.edf' fill='true' size='7,7mm' distance-field='true'/>\n"
	      + "					<label name='[" + etk::to_string(getId()) + "]file-shooser:cancel-label'>" + TRANSLATE("Cancel") + "</label>\n"
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
	      + "			<image name='[" + etk::to_string(getId()) + "]file-shooser:img-file' src='THEME:GUI:File.edf' expand='false' size='8,8mm' distance-field='true'/>\n"
	      + "			<entry name='[" + etk::to_string(getId()) + "]file-shooser:entry-file' expand='true,false' fill='true,false'/>\n"
	      + "		</sizer>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<image name='[" + etk::to_string(getId()) + "]file-shooser:img-folder' src='THEME:GUI:Folder.edf' expand='false' size='8,8mm' distance-field='true'/>\n"
	      + "			<entry name='[" + etk::to_string(getId()) + "]file-shooser:entry-folder' expand='true,false' fill='true,false'/>\n"
	      + "			<image name='[" + etk::to_string(getId()) + "]file-shooser:img-home' src='THEME:GUI:Home.edf' expand='false' size='8,8mm' distance-field='true'/>\n"
	      + "		</sizer>\n"
	      + "		<label name='[" + etk::to_string(getId()) + "]file-shooser:title-label'>" + TRANSLATE("FileChooser") + "</label>\n"
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
	setCanHaveFocus(true);
}

void ewol::widget::FileChooser::onGetFocus() {
	// transfert focus on a specific widget...
	parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:entry-file", "focus", "true");
}

ewol::widget::FileChooser::~FileChooser() {
	
}

void ewol::widget::FileChooser::setTitle(const std::string& _label) {
	parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:title-label", "value", _label);
}

void ewol::widget::FileChooser::setValidateLabel(const std::string& _label) {
	parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:validate-label", "value", _label);
}

void ewol::widget::FileChooser::setCancelLabel(const std::string& _label) {
	parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:cancel-label", "value", _label);
}

void ewol::widget::FileChooser::setFolder(const std::string& _folder) {
	m_folder = _folder + "/";
	updateCurrentFolder();
}

void ewol::widget::FileChooser::setFileName(const std::string& _filename) {
	m_file = _filename;
	parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:entry-file", "value", _filename);
}


void ewol::widget::FileChooser::onCallbackEntryFolderChangeValue(const std::string& _value) {
	// == > change the folder name
	// TODO : change the folder, if it exit ...
}

void ewol::widget::FileChooser::onCallbackEntryFileChangeValue(const std::string& _value) {
	// == > change the file name
	m_file = _value;
	// update the selected file in the list :
	parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-files", "select", m_file);
}

void ewol::widget::FileChooser::onCallbackButtonCancelPressed() {
	// == > Auto remove ...
	signalCancel.emit();
	autoDestroy();
}

void ewol::widget::FileChooser::onCallbackHidenFileChangeChangeValue(const bool& _value) {
	if (_value == true) {
		parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-folder", "show-hidden", "true");
		parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-files", "show-hidden", "true");
	} else {
		parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-folder", "show-hidden", "false");
		parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-files", "show-hidden", "false");
	}
}

void ewol::widget::FileChooser::onCallbackListFolderSelectChange(const std::string& _value) {
	// == > this is an internal event ...
	EWOL_DEBUG(" old PATH : \"" << m_folder << "\" + \"" << _value << "\"");
	m_folder = m_folder + _value;
	EWOL_DEBUG("new PATH : \"" << m_folder << "\"");
	m_folder = etk::simplifyPath(m_folder);
	setFileName("");
	updateCurrentFolder();
}

void ewol::widget::FileChooser::onCallbackListFileSelectChange(const std::string& _value) {
	setFileName(_value);
	std::string tmpFileCompleatName = m_folder;
	tmpFileCompleatName += m_file;
	// TODO : generateEventId(_msg.getMessage(), tmpFileCompleatName);
}

void ewol::widget::FileChooser::onCallbackListFileValidate(const std::string& _value) {
	// select the file  == > generate a validate
	setFileName(_value);
	EWOL_VERBOSE(" generate a fiel opening : \"" << m_folder << "\" / \"" << m_file << "\"");
	signalValidate.emit(getCompleateFileName());
	autoDestroy();
}

void ewol::widget::FileChooser::onCallbackListValidate() {
	if (m_file != "" ) {
		return;
	}
	EWOL_VERBOSE(" generate a fiel opening : \"" << m_folder << "\" / \"" << m_file << "\"");
	signalValidate.emit(getCompleateFileName());
	autoDestroy();
}

void ewol::widget::FileChooser::onCallbackHomePressed() {
	std::string tmpUserFolder = etk::getUserHomeFolder();
	EWOL_DEBUG("new PATH : \"" << tmpUserFolder << "\"");
	
	m_folder = etk::simplifyPath(tmpUserFolder);
	
	setFileName("");
	updateCurrentFolder();
}

void ewol::widget::FileChooser::updateCurrentFolder() {
	if (m_folder != "" ) {
		if (m_folder[m_folder.size()-1] != '/') {
			m_folder +=  "/";
		}
	}
	parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-files", "path", m_folder);
	parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:list-folder", "path", m_folder);
	parameterSetOnWidgetNamed("[" + etk::to_string(getId()) + "]file-shooser:entry-folder", "value", m_folder);
	markToRedraw();
}

std::string ewol::widget::FileChooser::getCompleateFileName() {
	std::string tmpString = m_folder;
	tmpString += "/";
	tmpString += m_file;
	etk::FSNode node(tmpString);
	return node.getName();
}

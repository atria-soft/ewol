/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/List.h>
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


const char * const ewol::widget::FileChooser::eventCancel     = "cancel";
const char * const ewol::widget::FileChooser::eventValidate   = "validate";

static const char * const ewolEventFileChooserHidenFileChange  = "ewol-event-file-chooser-Show/Hide-hiden-Files";
static const char * const ewolEventFileChooserEntryFolder      = "ewol-event-file-chooser-modify-entry-folder";
static const char * const ewolEventFileChooserEntryFolderEnter = "ewol-event-file-chooser-modify-entry-folder-enter";
static const char * const ewolEventFileChooserEntryFile        = "ewol-event-file-chooser-modify-entry-file";
static const char * const ewolEventFileChooserEntryFileEnter   = "ewol-event-file-chooser-modify-entry-file-enter";
static const char * const ewolEventFileChooserListFolder       = "ewol-event-file-chooser-modify-list-folder";
static const char * const ewolEventFileChooserListFile         = "ewol-event-file-chooser-modify-list-file";
static const char * const ewolEventFileChooserListFileValidate = "ewol-event-file-chooser-modify-list-file-validate";
static const char * const ewolEventFileChooserHome             = "ewol-event-file-chooser-home";


ewol::widget::FileChooser::FileChooser() {
	addObjectType("ewol::widget::FileChooser");
	addEventId(eventCancel);
	addEventId(eventValidate);
}

void ewol::widget::FileChooser::init() {
	ewol::widget::Composer::init();
	m_folder = etk::getUserHomeFolder();
	m_file = "";
	std::string myDescription = std::string("")
	      + "<popup >\n"
	      + "	<sizer mode='vert' lock='true' fill='true' expand='true'>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<checkbox name='[" + std::to_string(getId()) + "]file-shooser:show-hiden-file'>\n"
	      + "				<label>" + TRANSLATE("ShowHiddenFiles") + "</label>\n"
	      + "			</checkbox>\n"
	      + "			<spacer expand='true,false'/>\n"
	      + "			<button name='[" + std::to_string(getId()) + "]file-shooser:button-validate'>\n"
	      + "				<sizer mode='hori'>\n"
	      + "					<image src='THEME:GUI:Load.edf' fill='true' size='7,7mm' distance-field='true'/>\n"
	      + "					<label name='[" + std::to_string(getId()) + "]file-shooser:validate-label'>" + TRANSLATE("Validate") + "</label>\n"
	      + "				</sizer>\n"
	      + "			</button>\n"
	      + "			<button name='[" + std::to_string(getId()) + "]file-shooser:button-cancel'>\n"
	      + "				<sizer mode='hori'>\n"
	      + "					<image src='THEME:GUI:Remove.edf' fill='true' size='7,7mm' distance-field='true'/>\n"
	      + "					<label name='[" + std::to_string(getId()) + "]file-shooser:cancel-label'>" + TRANSLATE("Cancel") + "</label>\n"
	      + "				</sizer>\n"
	      + "			</button>\n"
	      + "		</sizer>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<spacer min-size='2,2mm'/>\n"
	      + "			<ListFileSystem name='[" + std::to_string(getId()) + "]file-shooser:list-folder'\n"
	      + "			                min-size='20,0%'\n"
	      + "			                expand='false,true'\n"
	      + "			                show-hidden='false'\n"
	      + "			                show-file='false'\n"
	      + "			                show-folder='true'\n"
	      + "			                show-temporary='false'/>\n"
	      + "			<spacer min-size='2,2mm'/>\n"
	      + "			<ListFileSystem name='[" + std::to_string(getId()) + "]file-shooser:list-files'\n"
	      + "			                expand='true,true'\n"
	      + "			                show-hidden='false'\n"
	      + "			                show-file='true'\n"
	      + "			                show-folder='false'\n"
	      + "			                show-temporary='false'/>\n"
	      + "			<spacer min-size='2,2mm'/>\n"
	      + "		</sizer>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<image name='[" + std::to_string(getId()) + "]file-shooser:img-file' src='THEME:GUI:File.edf' expand='false' size='8,8mm' distance-field='true'/>\n"
	      + "			<entry name='[" + std::to_string(getId()) + "]file-shooser:entry-file' expand='true,false' fill='true,false'/>\n"
	      + "		</sizer>\n"
	      + "		<sizer mode='hori'>\n"
	      + "			<image name='[" + std::to_string(getId()) + "]file-shooser:img-folder' src='THEME:GUI:Folder.edf' expand='false' size='8,8mm' distance-field='true'/>\n"
	      + "			<entry name='[" + std::to_string(getId()) + "]file-shooser:entry-folder' expand='true,false' fill='true,false'/>\n"
	      + "			<image name='[" + std::to_string(getId()) + "]file-shooser:img-home' src='THEME:GUI:Home.edf' expand='false' size='8,8mm' distance-field='true'/>\n"
	      + "		</sizer>\n"
	      + "		<label name='[" + std::to_string(getId()) + "]file-shooser:title-label'>" + TRANSLATE("FileChooser") + "</label>\n"
	      + "	</sizer>\n"
	      + "</popup>";
	loadFromString(myDescription);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:show-hiden-file", "value", ewolEventFileChooserHidenFileChange);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:button-validate", "pressed", eventValidate);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:button-cancel", "pressed", eventCancel);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:list-folder", "folder-validate", ewolEventFileChooserListFolder);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:list-files", "file-select", ewolEventFileChooserListFile);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:list-files", "file-validate", ewolEventFileChooserListFileValidate);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:entry-file", "modify", ewolEventFileChooserEntryFile);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:entry-file", "enter",  ewolEventFileChooserEntryFileEnter);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:entry-folder", "modify", ewolEventFileChooserEntryFolder);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:entry-folder", "enter",  ewolEventFileChooserEntryFolderEnter);
	registerOnEventNameWidget("[" + std::to_string(getId()) + "]file-shooser:img-home", "pressed", ewolEventFileChooserHome);
	// set the default Folder properties:
	updateCurrentFolder();
	setCanHaveFocus(true);
}

void ewol::widget::FileChooser::onGetFocus() {
	// transfert focus on a specific widget...
	setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:entry-file", "focus", "true");
}

ewol::widget::FileChooser::~FileChooser() {
	
}

void ewol::widget::FileChooser::setTitle(const std::string& _label) {
	setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:title-label", "value", _label);
}

void ewol::widget::FileChooser::setValidateLabel(const std::string& _label) {
	setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:validate-label", "value", _label);
}

void ewol::widget::FileChooser::setCancelLabel(const std::string& _label) {
	setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:cancel-label", "value", _label);
}

void ewol::widget::FileChooser::setFolder(const std::string& _folder) {
	m_folder = _folder + "/";
	updateCurrentFolder();
}

void ewol::widget::FileChooser::setFileName(const std::string& _filename) {
	m_file = _filename;
	setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:entry-file", "value", _filename);
}

void ewol::widget::FileChooser::onReceiveMessage(const ewol::object::Message& _msg) {
	EWOL_INFO("Receive Event from the LIST ... : " << _msg);
	if (_msg.getMessage() == ewolEventFileChooserEntryFolder) {
		// == > change the folder name
		// TODO : change the folder, if it exit ...
	} else if (_msg.getMessage() == ewolEventFileChooserEntryFile) {
		// == > change the file name
		m_file = _msg.getData();
		// update the selected file in the list :
		setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:list-files", "select", m_file);
	} else if (eventCancel == _msg.getMessage()) {
		// == > Auto remove ...
		generateEventId(_msg.getMessage());
		autoDestroy();
	} else if (_msg.getMessage() == ewolEventFileChooserHidenFileChange) {
		if (_msg.getData() == "true") {
			setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:list-folder", "show-hidden", "true");
			setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:list-files", "show-hidden", "true");
		} else {
			setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:list-folder", "show-hidden", "false");
			setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:list-files", "show-hidden", "false");
		}
	} else if (_msg.getMessage() == ewolEventFileChooserListFolder) {
		// == > this is an internal event ...
		EWOL_DEBUG(" old PATH : \"" << m_folder << "\" + \"" << _msg.getData() << "\"");
		m_folder = m_folder + _msg.getData();
		EWOL_DEBUG("new PATH : \"" << m_folder << "\"");
		m_folder = etk::simplifyPath(m_folder);
		setFileName("");
		updateCurrentFolder();
	} else if (_msg.getMessage() == ewolEventFileChooserListFile) {
		setFileName(_msg.getData());
		std::string tmpFileCompleatName = m_folder;
		tmpFileCompleatName += m_file;
		generateEventId(_msg.getMessage(), tmpFileCompleatName);
	} else if(     _msg.getMessage() == ewolEventFileChooserListFileValidate 
	           || (_msg.getMessage() == eventValidate && m_file != "" )
	           || (_msg.getMessage() == ewolEventFileChooserEntryFileEnter && m_file != "" ) ) {
		// select the file  == > generate a validate
		if (_msg.getData() != "") {
			setFileName(_msg.getData());
		}
		EWOL_VERBOSE(" generate a fiel opening : \"" << m_folder << "\" / \"" << m_file << "\"");
		generateEventId(eventValidate, getCompleateFileName());
		autoDestroy();
	} else if(_msg.getMessage() == ewolEventFileChooserHome) {
		std::string tmpUserFolder = etk::getUserHomeFolder();
		EWOL_DEBUG("new PATH : \"" << tmpUserFolder << "\"");
		
		m_folder = etk::simplifyPath(tmpUserFolder);
		
		setFileName("");
		updateCurrentFolder();
	}
	return;
};

void ewol::widget::FileChooser::updateCurrentFolder() {
	if (m_folder != "" ) {
		if (m_folder[m_folder.size()-1] != '/') {
			m_folder +=  "/";
		}
	}
	setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:list-files", "path", m_folder);
	setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:list-folder", "path", m_folder);
	setConfigNamed("[" + std::to_string(getId()) + "]file-shooser:entry-folder", "value", m_folder);
	markToRedraw();
}

std::string ewol::widget::FileChooser::getCompleateFileName() {
	std::string tmpString = m_folder;
	tmpString += "/";
	tmpString += m_file;
	etk::FSNode node(tmpString);
	return node.getName();
}

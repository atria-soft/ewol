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


extern const char * const ewol::widget::FileChooser::eventCancel     = "ewol-event-file-chooser-cancel";
extern const char * const ewol::widget::FileChooser::eventValidate   = "ewol-event-file-chooser-validate";
extern const char * const ewolEventFileChooserHidenFileChange  = "ewol-event-file-chooser-Show/Hide-hiden-Files";
extern const char * const ewolEventFileChooserEntryFolder      = "ewol-event-file-chooser-modify-entry-folder";
extern const char * const ewolEventFileChooserEntryFolderEnter = "ewol-event-file-chooser-modify-entry-folder-enter";
extern const char * const ewolEventFileChooserEntryFile        = "ewol-event-file-chooser-modify-entry-file";
extern const char * const ewolEventFileChooserEntryFileEnter   = "ewol-event-file-chooser-modify-entry-file-enter";
extern const char * const ewolEventFileChooserListFolder       = "ewol-event-file-chooser-modify-list-folder";
extern const char * const ewolEventFileChooserListFile         = "ewol-event-file-chooser-modify-list-file";
extern const char * const ewolEventFileChooserListFileValidate = "ewol-event-file-chooser-modify-list-file-validate";
extern const char * const ewolEventFileChooserHome             = "ewol-event-file-chooser-home";


ewol::widget::FileChooser::FileChooser(void) {
	addObjectType("ewol::widget::FileChooser");
	addEventId(eventCancel);
	addEventId(eventValidate);
	
	m_widgetTitle = NULL;
	m_widgetValidate = NULL;
	m_widgetCancel = NULL;
	m_widgetCurrentFolder = NULL;
	m_widgetCurrentFileName = NULL;
	m_widgetListFolder = NULL;
	m_widgetListFile = NULL;
	m_widgetCheckBox = NULL;
	
	ewol::widget::Sizer * mySizerVert = NULL;
	ewol::widget::Sizer * mySizerHori = NULL;
	ewol::widget::Spacer * mySpacer = NULL;
	//ewol::widget::Label * myLabel = NULL;
	ewol::widget::Image * myImage = NULL;
	m_folder = etk::getUserHomeFolder();
	#if defined(__TARGET_OS__Android)
		setMinSize(ewol::Dimension(vec2(90,90),ewol::Dimension::Pourcent));;
	#elif defined(__TARGET_OS__Windows)
		setMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));;
	#else
		setMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));;
	#endif
	m_file = "";
	
	/*
	SubWidgetSet(new ewol::widget::Composer(widget::Composer::String,
	        "<composer>\n"
	        "	<sizer mode=\"vert\" lock=\"true\">\n"
	        "		<sizer mode=\"hori\">\n"
	        "			<checkbox name=\"EWOL:file-shooser:show-hiden-file\"/>\n"
	        "			<label>Show hiden files</label>\n"
	        "			<spacer expand=\"true,false\"/>\n"
	        "			<button name=\"EWOL:file-shooser:button-validate\">\n"
	        "				<sizer mode=\"hori\">\n"
	        "					<image src=\"THEME:GUI:Load.svg\" fill=\"true\" size=\"70,70mm\"/>\n"
	        "					<label>Validate</label>\n"
	        "				</sizer>\n"
	        "			</button>\n"
	        "			<button name=\"EWOL:file-shooser:button-cancel\">\n"
	        "				<sizer mode=\"hori\">\n"
	        "					<image src=\"THEME:GUI:Remove.svg\" fill=\"true\" size=\"70,70mm\"/>\n"
	        "					<label>Cancel</label>\n"
	        "				</sizer>\n"
	        "			</button>\n"
	        "		</sizer>\n"
	        "		<sizer mode=\"hori\">\n"
	        "			<spacer min-size=\"2,2mm\"/>\n"
	        "			<ListFileSystem name=\"EWOL:file-shooser:list-folder\" min-size=\"20,0%\" expand=\"false,true\"/>\n"
	        "			<spacer min-size=\"2,2mm\"/>\n"
	        "			<ListFileSystem name=\"EWOL:file-shooser:list-files\" expand=\"true,true\"/>\n"
	        "			<spacer min-size=\"2,2mm\"/>\n"
	        "		</sizer>\n"
	        "		<sizer mode=\"hori\">\n"
	        "			\n"
	        "		</sizer>\n"
	        "	</sizer>\n"
	        "</composer\n"));
	*/
	mySizerVert = new ewol::widget::Sizer(ewol::widget::Sizer::modeVert);
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		mySizerVert->lockExpand(bvec2(true,true));
		mySizerVert->setExpand(bvec2(true,true));
		// set it in the pop-up-system : 
		setSubWidget(mySizerVert);
		
		mySizerHori = new ewol::widget::Sizer(ewol::widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(mySizerHori);
			m_widgetCheckBox = new ewol::widget::CheckBox("Show hiden files");
			if (NULL == m_widgetCheckBox) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				m_widgetCheckBox->registerOnEvent(this, ewolEventCheckBoxClicked, ewolEventFileChooserHidenFileChange);
				m_widgetCheckBox->setValue(false);
				mySizerHori->subWidgetAdd(m_widgetCheckBox);
			}
			mySpacer = new ewol::widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->setExpand(bvec2(true,false));
				mySizerHori->subWidgetAdd(mySpacer);
			}
			m_widgetValidate = new ewol::widget::Button();
			if (NULL == m_widgetValidate) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				m_widgetValidate->setSubWidget(
				    new ewol::widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:Load.svg\" expand=\"true\" size=\"8,8mm\"/>\n"
				        "		<label>Validate</label>\n"
				        "	</sizer>\n"
				        "</composer>\n"));
				m_widgetValidate->registerOnEvent(this, ewol::widget::Button::eventPressed, eventValidate);
				mySizerHori->subWidgetAdd(m_widgetValidate);
			}
			m_widgetCancel = new ewol::widget::Button();
			if (NULL == m_widgetCancel) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				m_widgetCancel->setSubWidget(
				    new ewol::widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:Remove.svg\" expand=\"true\" size=\"8,8mm\"/>\n"
				        "		<label>Cancel</label>\n"
				        "	</sizer>\n"
				        "</composer>\n"));
				m_widgetCancel->registerOnEvent(this, ewol::widget::Button::eventPressed, eventCancel);
				mySizerHori->subWidgetAdd(m_widgetCancel);
			}
		}
		mySizerHori = new ewol::widget::Sizer(widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(mySizerHori);
			mySpacer = new ewol::widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->setMinSize(vec2(2,2));
				mySizerHori->subWidgetAdd(mySpacer);
			}
			m_widgetListFolder = new ewol::widget::ListFileSystem();
			if (NULL == m_widgetListFolder) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				m_widgetListFolder->setShowFolder(true);
				m_widgetListFolder->setShowFiles(false);
				m_widgetListFolder->setShowHiddenFiles(false);
				m_widgetListFolder->registerOnEvent(this, ewolEventFSFolderValidate, ewolEventFileChooserListFolder);
				m_widgetListFolder->setExpand(bvec2(false,true));
				m_widgetListFolder->setFill(bvec2(false,true));
				mySizerHori->subWidgetAdd(m_widgetListFolder);
			}
			mySpacer = new ewol::widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->setMinSize(vec2(2,2));
				mySizerHori->subWidgetAdd(mySpacer);
			}
			m_widgetListFile = new ewol::widget::ListFileSystem();
			if (NULL == m_widgetListFile) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				m_widgetListFile->setShowFolder(false);
				m_widgetListFile->setShowFiles(true);
				m_widgetListFile->setShowHiddenFiles(false);
				m_widgetListFile->registerOnEvent(this, ewolEventFSFileSelect, ewolEventFileChooserListFile);
				m_widgetListFile->registerOnEvent(this, ewolEventFSFileValidate, ewolEventFileChooserListFileValidate);
				m_widgetListFile->setExpand(bvec2(true,true));
				m_widgetListFile->setFill(bvec2(true,true));
				mySizerHori->subWidgetAdd(m_widgetListFile);
			}
			mySpacer = new ewol::widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->setMinSize(vec2(2,2));
				mySizerHori->subWidgetAdd(mySpacer);
			}
		}
		mySizerHori = new ewol::widget::Sizer(widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(mySizerHori);
			myImage = new ewol::widget::Image("THEME:GUI:File.svg", ewol::Dimension(vec2(8,8),ewol::Dimension::Millimeter));
			if (NULL == myImage) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				//myImage->setImageSize(ewol::Dimension(vec2(8,8),ewol::Dimension::Millimeter));
				myImage->setExpand(bvec2(false,false));
				mySizerHori->subWidgetAdd(myImage);
			}
			m_widgetCurrentFileName = new ewol::widget::Entry(m_file);
			if (NULL == m_widgetCurrentFileName) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				m_widgetCurrentFileName->registerOnEvent(this, ewol::widget::Entry::eventModify, ewolEventFileChooserEntryFile);
				m_widgetCurrentFileName->registerOnEvent(this, ewol::widget::Entry::eventEnter,  ewolEventFileChooserEntryFileEnter);
				m_widgetCurrentFileName->setExpand(bvec2(true,false));
				m_widgetCurrentFileName->setFill(bvec2(true,false));
				//m_widgetCurrentFileName->setWidth(200);
				mySizerHori->subWidgetAdd(m_widgetCurrentFileName);
			}
		}
		mySizerHori = new ewol::widget::Sizer(widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(mySizerHori);
			myImage = new ewol::widget::Image("THEME:GUI:Folder.svg", ewol::Dimension(vec2(8,8),ewol::Dimension::Millimeter));
			if (NULL == myImage) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				//myImage->setImageSize(ewol::Dimension(vec2(8,8),ewol::Dimension::Millimeter));
				myImage->setExpand(bvec2(false,false));
				mySizerHori->subWidgetAdd(myImage);
			}
			
			m_widgetCurrentFolder = new ewol::widget::Entry(m_folder);
			if (NULL == m_widgetCurrentFolder) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				m_widgetCurrentFolder->registerOnEvent(this, ewol::widget::Entry::eventModify, ewolEventFileChooserEntryFolder);
				m_widgetCurrentFolder->registerOnEvent(this, ewol::widget::Entry::eventEnter,  ewolEventFileChooserEntryFolderEnter);
				m_widgetCurrentFolder->setExpand(bvec2(true,false));
				m_widgetCurrentFolder->setFill(bvec2(true,false));
				//m_widgetCurrentFolder->setWidth(200);
				mySizerHori->subWidgetAdd(m_widgetCurrentFolder);
			}
			myImage = new ewol::widget::Image("THEME:GUI:Home.svg", ewol::Dimension(vec2(8,8),ewol::Dimension::Millimeter));
			if (NULL == myImage) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				//myImage->setImageSize(ewol::Dimension(vec2(8,8),ewol::Dimension::Millimeter));
				myImage->registerOnEvent(this, ewol::widget::Image::eventPressed, ewolEventFileChooserHome);
				myImage->setExpand(bvec2(false,false));
				mySizerHori->subWidgetAdd(myImage);
			}
		}
		
		m_widgetTitle = new ewol::widget::Label("File chooser ...");
		if (NULL == m_widgetTitle) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(m_widgetTitle);
		}
	}
	// set the default Folder properties:
	updateCurrentFolder();
}


ewol::widget::FileChooser::~FileChooser(void) {
	
}

void ewol::widget::FileChooser::setTitle(const std::string& _label) {
	if (NULL == m_widgetTitle) {
		return;
	}
	m_widgetTitle->setLabel(_label);
}

void ewol::widget::FileChooser::setValidateLabel(const std::string& _label) {
	if (NULL == m_widgetValidate) {
		return;
	}
	// TODO : set if back :
	/*
	m_widgetValidate->setLabel(label);
	*/
}

void ewol::widget::FileChooser::setCancelLabel(const std::string& _label) {
	if (NULL == m_widgetCancel) {
		return;
	}
	// TODO : set if back :
	/*
	m_widgetCancel->setLabel(label);
	*/
}

void ewol::widget::FileChooser::setFolder(const std::string& _folder) {
	m_folder = _folder + "/";
	updateCurrentFolder();
}

void ewol::widget::FileChooser::setFileName(const std::string& _filename) {
	m_file = _filename;
	if (NULL == m_widgetCurrentFileName) {
		return;
	}
	m_widgetCurrentFileName->setValue(_filename);
}

void ewol::widget::FileChooser::onReceiveMessage(const ewol::object::Message& _msg) {
	EWOL_INFO("Receive Event from the LIST ... : " << _msg);
	if (ewolEventFileChooserEntryFolder == _msg.getMessage()) {
		// == > change the folder name
		// TODO : change the folder, if it exit ...
	} else if (ewolEventFileChooserEntryFile == _msg.getMessage()) {
		// == > change the file name
		m_file = _msg.getData();
		// update the selected file in the list : 
		if (m_widgetListFile != NULL) {
			m_widgetListFile->setSelect(m_file);
		}
	} else if (eventCancel == _msg.getMessage()) {
		// == > Auto remove ...
		generateEventId(_msg.getMessage());
		autoDestroy();
	} else if (ewolEventFileChooserHidenFileChange == _msg.getMessage()) {
		if (_msg.getData() == "true") {
			if (NULL!=m_widgetListFolder) {
				m_widgetListFolder->setShowHiddenFiles(true);
			}
			if (NULL!=m_widgetListFile) {
				m_widgetListFile->setShowHiddenFiles(true);
			}
		} else {
			if (NULL!=m_widgetListFolder) {
				m_widgetListFolder->setShowHiddenFiles(false);
			}
			if (NULL!=m_widgetListFile) {
				m_widgetListFile->setShowHiddenFiles(false);
			}
		}
	} else if (ewolEventFileChooserListFolder == _msg.getMessage()) {
		// == > this is an internal event ...
		EWOL_DEBUG(" old PATH : \"" << m_folder << "\" + \"" << _msg.getData() << "\"");
		m_folder = m_folder + _msg.getData();
		EWOL_DEBUG("new PATH : \"" << m_folder << "\"");
		m_folder = etk::simplifyPath(m_folder);
		setFileName("");
		updateCurrentFolder();
	} else if (ewolEventFileChooserListFile == _msg.getMessage()) {
		setFileName(_msg.getData());
		std::string tmpFileCompleatName = m_folder;
		tmpFileCompleatName += m_file;
		generateEventId(_msg.getMessage(), tmpFileCompleatName);
	} else if(     _msg.getMessage() == ewolEventFileChooserListFileValidate 
	           || (_msg.getMessage() == eventValidate       && m_file != "" )
	           || (_msg.getMessage() == ewolEventFileChooserEntryFileEnter && m_file != "" ) ) {
		// select the file  == > generate a validate
		if (_msg.getData() != "") {
			setFileName(_msg.getData());
		}
		EWOL_VERBOSE(" generate a fiel opening : \"" << m_folder << "\" / \"" << m_file << "\"");
		std::string tmpFileCompleatName = m_folder;
		tmpFileCompleatName += m_file;
		generateEventId(eventValidate, tmpFileCompleatName);
		autoDestroy();
	} else if(ewolEventFileChooserHome == _msg.getMessage()) {
		std::string tmpUserFolder = etk::getUserHomeFolder();
		EWOL_DEBUG("new PATH : \"" << tmpUserFolder << "\"");
		
		m_folder = etk::simplifyPath(tmpUserFolder);
		
		setFileName("");
		updateCurrentFolder();
	}
	return;
};

void ewol::widget::FileChooser::updateCurrentFolder(void) {
	if (m_folder != "" ) {
		if (m_folder[m_folder.size()-1] != '/') {
			m_folder +=  "/";
		}
	}
	if (NULL != m_widgetListFile) {
		m_widgetListFile->setFolder(m_folder);
	}
	if (NULL != m_widgetListFolder) {
		m_widgetListFolder->setFolder(m_folder);
	}
	if (NULL != m_widgetCurrentFolder) {
		m_widgetCurrentFolder->setValue(m_folder);
	}
	markToRedraw();
}

std::string ewol::widget::FileChooser::getCompleateFileName(void) {
	std::string tmpString = m_folder;
	tmpString += "/";
	tmpString += m_file;
	return tmpString;
}

/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved  == > the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 */
void ewol::widget::FileChooser::onObjectRemove(ewol::Object * removeObject) {
	// First step call parrent : 
	ewol::widget::PopUp::onObjectRemove(removeObject);
	// second step find if in all the elements ...
	if(removeObject == m_widgetTitle) {
		m_widgetTitle = NULL;
	}
	if(removeObject == m_widgetValidate) {
		m_widgetValidate = NULL;
	}
	if(removeObject == m_widgetCancel) {
		m_widgetCancel = NULL;
	}
	if(removeObject == m_widgetCurrentFolder) {
		m_widgetCurrentFolder = NULL;
	}
	if(removeObject == m_widgetCurrentFileName) {
		m_widgetCurrentFileName = NULL;
	}
	if(removeObject == m_widgetListFolder) {
		m_widgetListFolder = NULL;
	}
	if(removeObject == m_widgetListFile) {
		m_widgetListFile = NULL;
	}
	if(removeObject == m_widgetCheckBox) {
		m_widgetCheckBox = NULL;
	}
}


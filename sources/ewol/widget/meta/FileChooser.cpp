/**
 *******************************************************************************
 * @file ewol/widget/meta/FileChooser.cpp
 * @brief ewol File chooser meta widget system (Sources)
 * @author Edouard DUPIN
 * @date 29/12/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Image.h>
#include <ewol/widget/WidgetManager.h>
//#include <etk/Vector.h>
#include <etk/Vector.h>
#include <etk/tool.h>
#include <etk/os/FSNode.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"FileChooser(FolderList)"


#include <ewol/ewol.h>



#undef __class__
#define __class__	"FileChooser"


extern const char * const ewolEventFileChooserCancel           = "ewol-event-file-chooser-cancel";
extern const char * const ewolEventFileChooserValidate         = "ewol-event-file-chooser-validate";
extern const char * const ewolEventFileChooserHidenFileChange  = "ewol-event-file-chooser-Show/Hide-hiden-Files";
extern const char * const ewolEventFileChooserEntryFolder      = "ewol-event-file-chooser-modify-entry-folder";
extern const char * const ewolEventFileChooserEntryFolderEnter = "ewol-event-file-chooser-modify-entry-folder-enter";
extern const char * const ewolEventFileChooserEntryFile        = "ewol-event-file-chooser-modify-entry-file";
extern const char * const ewolEventFileChooserEntryFileEnter   = "ewol-event-file-chooser-modify-entry-file-enter";
extern const char * const ewolEventFileChooserListFolder       = "ewol-event-file-chooser-modify-list-folder";
extern const char * const ewolEventFileChooserListFile         = "ewol-event-file-chooser-modify-list-file";
extern const char * const ewolEventFileChooserListFileValidate = "ewol-event-file-chooser-modify-list-file-validate";
extern const char * const ewolEventFileChooserHome             = "ewol-event-file-chooser-home";


widget::FileChooser::FileChooser(void)
{
	AddEventId(ewolEventFileChooserCancel);
	AddEventId(ewolEventFileChooserValidate);
	
	m_widgetTitle = NULL;
	m_widgetValidate = NULL;
	m_widgetCancel = NULL;
	m_widgetCurrentFolder = NULL;
	m_widgetCurrentFileName = NULL;
	m_widgetListFolder = NULL;
	m_widgetListFile = NULL;
	m_widgetCheckBox = NULL;
	
	widget::SizerVert * mySizerVert = NULL;
	widget::SizerHori * mySizerHori = NULL;
	widget::Spacer * mySpacer = NULL;
	//widget::Label * myLabel = NULL;
	widget::Image * myImage = NULL;
	#if defined(__TARGET_OS__Android)
		m_folder = "/mnt/sdcard/";
		SetDisplayRatio(0.90);
	#elif defined(__TARGET_OS__Windows)
		m_folder = "c:/";
		SetDisplayRatio(0.80);
	#else
		m_folder = "/home/";
		SetDisplayRatio(0.80);
	#endif
	m_file = "";
	
	mySizerVert = new widget::SizerVert();
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		mySizerVert->LockExpendContamination(true);
		// set it in the pop-up-system : 
		SubWidgetSet(mySizerVert);
		
		mySizerHori = new widget::SizerHori();
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			m_widgetCheckBox = new widget::CheckBox("Show hiden files");
			if (NULL == m_widgetCheckBox) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_widgetCheckBox->RegisterOnEvent(this, ewolEventCheckBoxClicked, ewolEventFileChooserHidenFileChange);
				m_widgetCheckBox->SetValue(false);
				mySizerHori->SubWidgetAdd(m_widgetCheckBox);
			}
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetExpendX(true);
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			m_widgetValidate = new widget::Button("Validate");
			if (NULL == m_widgetValidate) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_widgetValidate->SetImage("THEME:GUI:Load.svg");
				m_widgetValidate->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventFileChooserValidate);
				mySizerHori->SubWidgetAdd(m_widgetValidate);
			}
			m_widgetCancel = new widget::Button("Cancel");
			if (NULL == m_widgetCancel) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_widgetCancel->SetImage("THEME:GUI:Remove.svg");
				m_widgetCancel->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventFileChooserCancel);
				mySizerHori->SubWidgetAdd(m_widgetCancel);
			}
		}
		mySizerHori = new widget::SizerHori();
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			m_widgetListFolder = new widget::ListFileSystem();
			if (NULL == m_widgetListFolder) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_widgetListFolder->SetShowFolder(true);
				m_widgetListFolder->SetShowFiles(false);
				m_widgetListFolder->SetShowHiddenFiles(false);
				m_widgetListFolder->RegisterOnEvent(this, ewolEventFSFolderValidate, ewolEventFileChooserListFolder);
				m_widgetListFolder->SetExpendY(true);
				m_widgetListFolder->SetFillY(true);
				mySizerHori->SubWidgetAdd(m_widgetListFolder);
			}
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			m_widgetListFile = new widget::ListFileSystem();
			if (NULL == m_widgetListFile) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_widgetListFile->SetShowFolder(false);
				m_widgetListFile->SetShowFiles(true);
				m_widgetListFile->SetShowHiddenFiles(false);
				m_widgetListFile->RegisterOnEvent(this, ewolEventFSFileSelect, ewolEventFileChooserListFile);
				m_widgetListFile->RegisterOnEvent(this, ewolEventFSFileValidate, ewolEventFileChooserListFileValidate);
				m_widgetListFile->SetExpendX(true);
				m_widgetListFile->SetFillX(true);
				m_widgetListFile->SetExpendY(true);
				m_widgetListFile->SetFillY(true);
				mySizerHori->SubWidgetAdd(m_widgetListFile);
			}
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			}
		}
		mySizerHori = new widget::SizerHori();
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			myImage = new widget::Image("THEME:GUI:File.svg");
			if (NULL == myImage) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myImage->SetFillY(true);
				mySizerHori->SubWidgetAdd(myImage);
			}
			m_widgetCurrentFileName = new widget::Entry(m_file);
			if (NULL == m_widgetCurrentFileName) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_widgetCurrentFileName->RegisterOnEvent(this, ewolEventEntryModify, ewolEventFileChooserEntryFile);
				m_widgetCurrentFileName->RegisterOnEvent(this, ewolEventEntryEnter,  ewolEventFileChooserEntryFileEnter);
				m_widgetCurrentFileName->SetExpendX(true);
				m_widgetCurrentFileName->SetFillX(true);
				m_widgetCurrentFileName->SetWidth(200);
				mySizerHori->SubWidgetAdd(m_widgetCurrentFileName);
			}
		}
		mySizerHori = new widget::SizerHori();
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			myImage = new widget::Image("THEME:GUI:Folder.svg");
			if (NULL == myImage) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myImage->SetFillY(true);
				mySizerHori->SubWidgetAdd(myImage);
			}
			
			m_widgetCurrentFolder = new widget::Entry(m_folder);
			if (NULL == m_widgetCurrentFolder) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_widgetCurrentFolder->RegisterOnEvent(this, ewolEventEntryModify, ewolEventFileChooserEntryFolder);
				m_widgetCurrentFolder->RegisterOnEvent(this, ewolEventEntryEnter,  ewolEventFileChooserEntryFolderEnter);
				m_widgetCurrentFolder->SetExpendX(true);
				m_widgetCurrentFolder->SetFillX(true);
				m_widgetCurrentFolder->SetWidth(200);
				mySizerHori->SubWidgetAdd(m_widgetCurrentFolder);
			}
			myImage = new widget::Image("THEME:GUI:Home.svg");
			if (NULL == myImage) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myImage->RegisterOnEvent(this, ewolEventImagePressed, ewolEventFileChooserHome);
				myImage->SetFillY(true);
				mySizerHori->SubWidgetAdd(myImage);
			}
		}
		
		m_widgetTitle = new widget::Label("File chooser ...");
		if (NULL == m_widgetTitle) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(m_widgetTitle);
		}
	}
	// set the default Folder properties:
	UpdateCurrentFolder();
}


widget::FileChooser::~FileChooser(void)
{
	
}


void widget::FileChooser::SetTitle(etk::UString label)
{
	if (NULL == m_widgetTitle) {
		return;
	}
	m_widgetTitle->SetLabel(label);
}

void widget::FileChooser::SetValidateLabel(etk::UString label)
{
	if (NULL == m_widgetValidate) {
		return;
	}
	m_widgetValidate->SetLabel(label);
}

void widget::FileChooser::SetCancelLabel(etk::UString label)
{
	if (NULL == m_widgetCancel) {
		return;
	}
	m_widgetCancel->SetLabel(label);
}

void widget::FileChooser::SetFolder(etk::UString folder)
{
	m_folder = folder + "/";
	UpdateCurrentFolder();
}

void widget::FileChooser::SetFileName(etk::UString filename)
{
	m_file = filename;
	if (NULL == m_widgetCurrentFileName) {
		return;
	}
	m_widgetCurrentFileName->SetValue(filename);
}


/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void widget::FileChooser::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	EWOL_INFO("Receive Event from the LIST ... : widgetPointer=" << CallerObject << "\"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (ewolEventFileChooserEntryFolder == eventId) {
		//==> change the folder name
		// TODO : Change the folder, if it exit ...
	} else if (ewolEventFileChooserEntryFile == eventId) {
		//==> change the file name
		m_file = data;
		// Update the selected file in the list : 
		if (m_widgetListFile != NULL) {
			m_widgetListFile->SetSelect(m_file);
		}
	} else if (ewolEventFileChooserCancel == eventId) {
		//==> Auto remove ...
		GenerateEventId(eventId);
		AutoDestroy();
	} else if (ewolEventFileChooserHidenFileChange == eventId) {
		if (data == "true") {
			if (NULL!=m_widgetListFolder) {
				m_widgetListFolder->SetShowHiddenFiles(true);
			}
			if (NULL!=m_widgetListFile) {
				m_widgetListFile->SetShowHiddenFiles(true);
			}
		} else {
			if (NULL!=m_widgetListFolder) {
				m_widgetListFolder->SetShowHiddenFiles(false);
			}
			if (NULL!=m_widgetListFile) {
				m_widgetListFile->SetShowHiddenFiles(false);
			}
		}
	} else if (ewolEventFileChooserListFolder == eventId) {
		//==> this is an internal event ...
		EWOL_DEBUG(" old PATH : \"" << m_folder << "\" + \"" << data << "\"");
		m_folder = m_folder + data;
		EWOL_DEBUG("new PATH : \"" << m_folder << "\"");
		m_folder = etk::tool::SimplifyPath(m_folder);
		SetFileName("");
		UpdateCurrentFolder();
	} else if (ewolEventFileChooserListFile == eventId) {
		SetFileName(data);
		etk::UString tmpFileCompleatName = m_folder;
		tmpFileCompleatName += m_file;
		GenerateEventId(eventId, tmpFileCompleatName);
	} else if(     eventId == ewolEventFileChooserListFileValidate 
	           || (eventId == ewolEventFileChooserValidate       && m_file != "" )
	           || (eventId == ewolEventFileChooserEntryFileEnter && m_file != "" ) ) {
		// select the File ==> generate a validate
		if (data != "") {
			SetFileName(data);
		}
		EWOL_VERBOSE(" generate a fiel opening : \"" << m_folder << "\" / \"" << m_file << "\"");
		etk::UString tmpFileCompleatName = m_folder;
		tmpFileCompleatName += m_file;
		GenerateEventId(ewolEventFileChooserValidate, tmpFileCompleatName);
		AutoDestroy();
	} else if(ewolEventFileChooserHome == eventId) {
		etk::UString tmpUserFolder = etk::GetUserHomeFolder();
		EWOL_DEBUG("new PATH : \"" << tmpUserFolder << "\"");
		
		m_folder = etk::tool::SimplifyPath(tmpUserFolder);
		
		SetFileName("");
		UpdateCurrentFolder();
	}
	return;
};



void widget::FileChooser::UpdateCurrentFolder(void)
{
	if (m_folder != "" ) {
		if (m_folder[m_folder.Size()-1] != '/') {
			m_folder +=  "/";
		}
	}
	if (NULL != m_widgetListFile) {
		m_widgetListFile->SetFolder(m_folder);
	}
	if (NULL != m_widgetListFolder) {
		m_widgetListFolder->SetFolder(m_folder);
	}
	if (NULL != m_widgetCurrentFolder) {
		m_widgetCurrentFolder->SetValue(m_folder);
	}
	MarkToRedraw();
}


etk::UString widget::FileChooser::GetCompleateFileName(void)
{
	etk::UString tmpString = m_folder;
	tmpString += "/";
	tmpString += m_file;
	return tmpString;
}


/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void widget::FileChooser::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	widget::PopUp::OnObjectRemove(removeObject);
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


/**
 *******************************************************************************
 * @file ewol/widgetMeta/FileChooser.cpp
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

#include <ewol/widgetMeta/FileChooser.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Image.h>
#include <ewol/WidgetManager.h>
//#include <etk/Vector.h>
#include <etk/VectorType.h>

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


ewol::FileChooser::FileChooser(void)
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
	
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	ewol::Spacer * mySpacer = NULL;
	//ewol::Label * myLabel = NULL;
	ewol::Image * myImage = NULL;
	#ifdef __PLATFORM__Android
		m_folder = "/mnt/sdcard/";
		SetDisplayRatio(0.90);
	#else
		m_folder = "/home/";
		SetDisplayRatio(0.80);
	#endif
	m_file = "";
	
	mySizerVert = new ewol::SizerVert();
		mySizerVert->LockExpendContamination(true);
		// set it in the pop-up-system : 
		SubWidgetSet(mySizerVert);
		
		m_widgetTitle = new ewol::Label("File chooser ...");
			mySizerVert->SubWidgetAdd(m_widgetTitle);
		
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			myImage = new ewol::Image("icon/Folder.svg");
				myImage->SetFillY(true);
				mySizerHori->SubWidgetAdd(myImage);
			
			m_widgetCurrentFolder = new ewol::Entry(m_folder);
				m_widgetCurrentFolder->RegisterOnEvent(this, ewolEventEntryModify, ewolEventFileChooserEntryFolder);
				m_widgetCurrentFolder->RegisterOnEvent(this, ewolEventEntryEnter,  ewolEventFileChooserEntryFolderEnter);
				m_widgetCurrentFolder->SetExpendX(true);
				m_widgetCurrentFolder->SetFillX(true);
				m_widgetCurrentFolder->SetWidth(200);
				mySizerHori->SubWidgetAdd(m_widgetCurrentFolder);
			myImage = new ewol::Image("icon/Home.svg");
				myImage->RegisterOnEvent(this, ewolEventImagePressed, ewolEventFileChooserHome);
				myImage->SetFillY(true);
				mySizerHori->SubWidgetAdd(myImage);
		
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			myImage = new ewol::Image("icon/File.svg");
				myImage->SetFillY(true);
				mySizerHori->SubWidgetAdd(myImage);
			m_widgetCurrentFileName = new ewol::Entry(m_file);
				m_widgetCurrentFileName->RegisterOnEvent(this, ewolEventEntryModify, ewolEventFileChooserEntryFile);
				m_widgetCurrentFileName->RegisterOnEvent(this, ewolEventEntryEnter,  ewolEventFileChooserEntryFileEnter);
				m_widgetCurrentFileName->SetExpendX(true);
				m_widgetCurrentFileName->SetFillX(true);
				m_widgetCurrentFileName->SetWidth(200);
				mySizerHori->SubWidgetAdd(m_widgetCurrentFileName);
		
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			mySpacer = new ewol::Spacer();
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			m_widgetListFolder = new ListFileSystem();
				m_widgetListFolder->SetShowFolder(true);
				m_widgetListFolder->SetShowFiles(false);
				m_widgetListFolder->SetShowHiddenFiles(false);
				m_widgetListFolder->RegisterOnEvent(this, ewolEventFSFolderValidate, ewolEventFileChooserListFolder);
				m_widgetListFolder->SetExpendY(true);
				m_widgetListFolder->SetFillY(true);
				mySizerHori->SubWidgetAdd(m_widgetListFolder);
			mySpacer = new ewol::Spacer();
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			m_widgetListFile = new ListFileSystem();
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
			mySpacer = new ewol::Spacer();
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			m_widgetCheckBox = new ewol::CheckBox("Show hiden files");
				m_widgetCheckBox->RegisterOnEvent(this, ewolEventCheckBoxClicked, ewolEventFileChooserHidenFileChange);
				m_widgetCheckBox->SetValue(false);
				mySizerHori->SubWidgetAdd(m_widgetCheckBox);
			mySpacer = new ewol::Spacer();
				mySpacer->SetExpendX(true);
				mySizerHori->SubWidgetAdd(mySpacer);
			m_widgetValidate = new ewol::Button("Validate");
				m_widgetValidate->SetImage("icon/Load.svg");
				m_widgetValidate->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventFileChooserValidate);
				mySizerHori->SubWidgetAdd(m_widgetValidate);
			m_widgetCancel = new ewol::Button("Cancel");
				m_widgetCancel->SetImage("icon/Remove.svg");
				m_widgetCancel->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventFileChooserCancel);
				mySizerHori->SubWidgetAdd(m_widgetCancel);
	
	// set the default Folder properties:
	UpdateCurrentFolder();
}


ewol::FileChooser::~FileChooser(void)
{
	
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_FILE_CHOOSER = "FileChooser";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::FileChooser::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_FILE_CHOOSER << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_FILE_CHOOSER) {
		return true;
	} else {
		if(true == ewol::PopUp::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_FILE_CHOOSER << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::FileChooser::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_FILE_CHOOSER;
}



void ewol::FileChooser::SetTitle(etk::UString label)
{
	if (NULL == m_widgetTitle) {
		return;
	}
	m_widgetTitle->SetLabel(label);
}

void ewol::FileChooser::SetValidateLabel(etk::UString label)
{
	if (NULL == m_widgetValidate) {
		return;
	}
	m_widgetValidate->SetLabel(label);
}

void ewol::FileChooser::SetCancelLabel(etk::UString label)
{
	if (NULL == m_widgetCancel) {
		return;
	}
	m_widgetCancel->SetLabel(label);
}

void ewol::FileChooser::SetFolder(etk::UString folder)
{
	m_folder = folder + "/";
	UpdateCurrentFolder();
}

void ewol::FileChooser::SetFileName(etk::UString filename)
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
void ewol::FileChooser::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
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
		MarkToRemove();
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
		EWOL_VERBOSE(" old PATH : \"" << m_folder << "\" + \"" << data << "\"");
		m_folder = m_folder + data;
		char buf[MAX_FILE_NAME];
		memset(buf, 0, MAX_FILE_NAME);
		char * ok;
		EWOL_DEBUG("new PATH : \"" << m_folder << "\"");
		
		ok = realpath(m_folder.Utf8Data(), buf);
		if (!ok) {
			EWOL_ERROR("Error to get the real path");
			m_folder = "/";
		} else {
			m_folder = buf;
		}
		if (m_folder != "/" ) {
			m_folder +=  "/";
		}
		SetFileName("");
		UpdateCurrentFolder();
	} else if (ewolEventFileChooserListFile == eventId) {
		SetFileName(data);
		GenerateEventId(eventId);
	} else if(     eventId == ewolEventFileChooserListFileValidate 
	           || (eventId == ewolEventFileChooserValidate       && m_file != "" )
	           || (eventId == ewolEventFileChooserEntryFileEnter && m_file != "" ) ) {
		// select the File ==> generate a validate
		SetFileName(data);
		GenerateEventId(ewolEventFileChooserValidate);
		MarkToRemove();
	} else if(ewolEventFileChooserHome == eventId) {
		etk::UString tmpUserFolder = etk::GetUserHomeFolder();
		char buf[MAX_FILE_NAME];
		memset(buf, 0, MAX_FILE_NAME);
		char * ok;
		EWOL_DEBUG("new PATH : \"" << tmpUserFolder << "\"");
		
		ok = realpath(tmpUserFolder.Utf8Data(), buf);
		if (!ok) {
			EWOL_ERROR("Error to get the real path");
			m_folder = "/";
		} else {
			m_folder = buf;
		}
		if (m_folder != "/" ) {
			m_folder +=  "/";
		}
		SetFileName("");
		UpdateCurrentFolder();
	}
	return;
};



void ewol::FileChooser::UpdateCurrentFolder(void)
{
	if (NULL != m_widgetListFile) {
		m_widgetListFile->SetFolder(m_folder);
	}
	if (NULL != m_widgetListFolder) {
		m_widgetListFolder->SetFolder(m_folder);
	}
	if (NULL != m_widgetCurrentFolder) {
		m_widgetCurrentFolder->SetValue(m_folder);
	}
	MarkToReedraw();
}


etk::UString ewol::FileChooser::GetCompleateFileName(void)
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
void ewol::FileChooser::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::PopUp::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	if(removeObject == m_widgetTitle) {
		m_widgetTitle = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetValidate) {
		m_widgetValidate = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetCancel) {
		m_widgetCancel = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetCurrentFolder) {
		m_widgetCurrentFolder = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetCurrentFileName) {
		m_widgetCurrentFileName = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetListFolder) {
		m_widgetListFolder = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetListFile) {
		m_widgetListFile = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetCheckBox) {
		m_widgetCheckBox = NULL;
		m_needFlipFlop = true;
	}
}


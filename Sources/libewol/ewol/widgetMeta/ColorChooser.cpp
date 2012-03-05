/**
 *******************************************************************************
 * @file ewol/widgetMeta/ColorChooser.cpp
 * @brief ewol Color chooser meta widget system (Sources)
 * @author Edouard DUPIN
 * @date 05/03/2012
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

#include <ewol/widgetMeta/ColorChooser.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/WidgetManager.h>
//#include <etk/Vector.h>
#include <etk/VectorType.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ColorChooser"




extern const char * const ewolEventFileChooserCancel          = "ewol-event-file-chooser-cancel";


ewol::ColorChooser::ColorChooser(void)
{
	AddEventId(ewolEventFileChooserCancel);
	AddEventId(ewolEventFileChooserValidate);
	
	m_hasSelectedFile = false;
	
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
	FileChooserFileList * myListFile = NULL;
	FileChooserFolderList * myListFolder = NULL;
	ewol::Label * myLabel = NULL;
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
			myLabel = new ewol::Label("Folder : ");
				myLabel->SetFillY(true);
				mySizerHori->SubWidgetAdd(myLabel);
			m_widgetCurrentFolder = new ewol::Entry(m_folder);
				m_widgetCurrentFolder->RegisterOnEvent(this, ewolEventEntryModify, ewolEventFileChooserEntryFolder);
				m_widgetCurrentFolder->SetExpendX(true);
				m_widgetCurrentFolder->SetFillX(true);
				m_widgetCurrentFolder->SetWidth(200);
				mySizerHori->SubWidgetAdd(m_widgetCurrentFolder);
		
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			myLabel = new ewol::Label("File Name : ");
				myLabel->SetFillY(true);
				mySizerHori->SubWidgetAdd(myLabel);
			m_widgetCurrentFileName = new ewol::Entry(m_file);
				m_widgetCurrentFileName->RegisterOnEvent(this, ewolEventEntryModify, ewolEventFileChooserEntryFile);
				m_widgetCurrentFileName->SetExpendX(true);
				m_widgetCurrentFileName->SetFillX(true);
				m_widgetCurrentFileName->SetWidth(200);
				mySizerHori->SubWidgetAdd(m_widgetCurrentFileName);
		
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			mySpacer = new ewol::Spacer();
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			myListFolder = new FileChooserFolderList();
				m_widgetListFolder = myListFolder;
				myListFolder->RegisterOnEvent(this, ewolEventFileChooserSelectFolder, ewolEventFileChooserSelectFolder);
				myListFolder->SetExpendY(true);
				myListFolder->SetFillY(true);
				mySizerHori->SubWidgetAdd(myListFolder);
			mySpacer = new ewol::Spacer();
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			myListFile = new FileChooserFileList();
				m_widgetListFile = myListFile;
				myListFile->RegisterOnEvent(this, ewolEventFileChooserSelectFile, ewolEventFileChooserSelectFile);
				myListFile->RegisterOnEvent(this, ewolEventFileChooserValidateFile, ewolEventFileChooserValidateFile);
				myListFile->SetExpendX(true);
				myListFile->SetFillX(true);
				myListFile->SetExpendY(true);
				myListFile->SetFillY(true);
				mySizerHori->SubWidgetAdd(myListFile);
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
			m_widgetValidate = new ewol::Button("Open");
				m_widgetCheckBox->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventFileChooserValidate);
				mySizerHori->SubWidgetAdd(m_widgetValidate);
			m_widgetCancel = new ewol::Button("Cancel");
				m_widgetCheckBox->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventFileChooserCancel);
				mySizerHori->SubWidgetAdd(m_widgetCancel);
	
	// set the default Folder properties:
	UpdateCurrentFolder();
}


ewol::ColorChooser::~ColorChooser(void)
{
	
}


void ewol::ColorChooser::SetTitle(etk::UString label)
{
	if (NULL == m_widgetTitle) {
		return;
	}
	m_widgetTitle->SetLabel(label);
}

void ewol::ColorChooser::SetValidateLabel(etk::UString label)
{
	if (NULL == m_widgetValidate) {
		return;
	}
	m_widgetValidate->SetLabel(label);
}

void ewol::ColorChooser::SetCancelLabel(etk::UString label)
{
	if (NULL == m_widgetCancel) {
		return;
	}
	m_widgetCancel->SetLabel(label);
}

void ewol::ColorChooser::SetFolder(etk::UString folder)
{
	m_folder = folder;
	UpdateCurrentFolder();
}

void ewol::ColorChooser::SetFileName(etk::UString filename)
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
void ewol::ColorChooser::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	EWOL_INFO("Receive Event from the LIST ... : widgetPointer=" << CallerObject << "\"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (ewolEventFileChooserEntryFolder == eventId) {
		//==> change the folder name
		// TODO : Change the folder, if it exit ...
		return;
	} else if (ewolEventFileChooserEntryFile == eventId) {
		//==> change the file name
		if (NULL != m_widgetCurrentFileName) {
			m_file = m_widgetCurrentFileName->GetValue();
		}
		// TODO : Remove file selection
		return;
	} else if (ewolEventFileChooserCancel == eventId) {
		//==> Auto remove ...
		GenerateEventId(eventId);
		MarkToRemove();
		return;
	} else if (ewolEventFileChooserHidenFileChange == eventId) {
		// regenerate the display ...
		UpdateCurrentFolder();
		return;
	} else if (ewolEventFileChooserSelectFolder == eventId) {
		//==> this is an internal event ...
		FileChooserFolderList * myListFolder = static_cast<FileChooserFolderList *>(m_widgetListFolder);
		etk::UString tmpString = myListFolder->GetSelectedLine();
		EWOL_DEBUG(" old PATH : \"" << m_folder << "\" + \"" << tmpString << "\"");
		m_folder = m_folder + tmpString;
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
		m_hasSelectedFile = false;
		return;
	} else if (ewolEventFileChooserSelectFile == eventId) {
		m_hasSelectedFile = true;
		FileChooserFileList * myListFile = static_cast<FileChooserFileList *>(m_widgetListFile);
		etk::UString file = myListFile->GetSelectedLine();
		SetFileName(file);
		GenerateEventId(eventId);
	} else if(    ewolEventFileChooserValidateFile == eventId
	           || (ewolEventFileChooserValidate == eventId && true == m_hasSelectedFile) ) {
		// select the File ==> generate a validate
		GenerateEventId(ewolEventFileChooserValidate);
		MarkToRemove();
		return;
	}
	return;
};



void ewol::ColorChooser::UpdateCurrentFolder(void)
{
	if (NULL == m_widgetListFile) {
		return;
	}
	if (NULL == m_widgetListFolder) {
		return;
	}
	FileChooserFileList * myListFile     = static_cast<FileChooserFileList *>(m_widgetListFile);
	FileChooserFolderList * myListFolder = static_cast<FileChooserFolderList *>(m_widgetListFolder);
	
	myListFile->ClearElements();
	myListFolder->ClearElements();
	bool ShowHidenFile = true;
	if (NULL != m_widgetCheckBox) {
		ShowHidenFile = m_widgetCheckBox->GetValue();
	} else {
		EWOL_ERROR("Can not get the hiden property of the file choozer...");
	}
	
	if (NULL != m_widgetCurrentFolder) {
		m_widgetCurrentFolder->SetValue(m_folder);
	}
	myListFolder->AddElement(etk::UString("."));
	if (m_folder != "/" ) {
		myListFolder->AddElement(etk::UString(".."));
	}
	DIR *dir;
	struct dirent *ent;
	dir = opendir(m_folder.Utf8Data());
	if (dir != NULL) {
		// for each element in the drectory...
		while ((ent = readdir(dir)) != NULL) {
			etk::UString tmpString(ent->d_name);
			if (DT_REG == ent->d_type) {
				if (false == tmpString.StartWith(".") || true==ShowHidenFile) {
					myListFile->AddElement(tmpString);
				}
			} else if (DT_DIR == ent->d_type) {
				//EWOL_DEBUG("    find Folder : \"" << tmpString << "\"(" << tmpString.Size() << ") ?= \"" << ent->d_name << "\"(" << strlen(ent->d_name) );
				if (tmpString != "." && tmpString != "..") {
					if (false == tmpString.StartWith(".") || true==ShowHidenFile) {
						myListFolder->AddElement(tmpString);
					}
				}
			}
		}
		closedir(dir);
	} else {
		EWOL_ERROR("could not open directory : \"" << m_folder << "\"");
	}
	myListFile->EndGenerating();
	myListFolder->EndGenerating();
	MarkToReedraw();
}


etk::UString ewol::ColorChooser::GetCompleateFileName(void)
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
void ewol::ColorChooser::OnObjectRemove(ewol::EObject * removeObject)
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


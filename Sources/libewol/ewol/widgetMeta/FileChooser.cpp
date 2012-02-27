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
#include <ewol/widget/Button.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/WidgetManager.h>
//#include <etk/Vector.h>
#include <etk/VectorType.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ewol::FileChooser(FolderList)"


#include <ewol/ewol.h>

void SortList(etk::VectorType<etk::UString *> &m_listDirectory)
{
	etk::VectorType<etk::UString *> tmpList = m_listDirectory;
	m_listDirectory.Clear();
	for(int32_t iii=0; iii<tmpList.Size(); iii++) {
		
		int32_t findPos = 0;
		for(int32_t jjj=0; jjj<m_listDirectory.Size(); jjj++) {
			//EWOL_DEBUG("compare : \""<<*tmpList[iii] << "\" and \"" << *m_listDirectory[jjj] << "\"");
			if (*tmpList[iii] > *m_listDirectory[jjj]) {
				findPos = jjj+1;
			}
		}
		//EWOL_DEBUG("position="<<findPos);
		m_listDirectory.Insert(findPos, tmpList[iii]);
	}
}



const char * const ewolEventFileChooserSelectFolder   = "ewol-event-file-chooser-Select-Folder";

class FileChooserFolderList : public ewol::List
{
	private:
		etk::VectorType<etk::UString *> m_listDirectory;
		int32_t                        m_selectedLine;
	public:
		FileChooserFolderList(void)
		{
			AddEventId(ewolEventFileChooserSelectFolder);
			m_selectedLine = -1;
		};
		~FileChooserFolderList(void)
		{
			ClearElements();
		};
		
		void AddElement(etk::UString element)
		{
			etk::UString* tmpEmement = new etk::UString(element);
			m_listDirectory.PushBack(tmpEmement);
		}
		void EndGenerating(void)
		{
			SortList(m_listDirectory);
			MarkToReedraw();
		}
		
		void ClearElements(void) {
			for (int32_t iii=0; iii<m_listDirectory.Size(); iii++) {
				if (NULL != m_listDirectory[iii]) {
					delete(m_listDirectory[iii]);
					m_listDirectory[iii] = NULL;
				}
			}
			m_listDirectory.Clear();
			MarkToReedraw();
		}
		
		etk::UString GetSelectedLine(void)
		{
			etk::UString tmpVal = "";
			if (m_selectedLine >= 0) {
				tmpVal = *(m_listDirectory[m_selectedLine]);
			}
			return tmpVal;
		}
		
		
		virtual color_ts GetBasicBG(void) {
			color_ts bg;
			bg.red = 0.0;
			bg.green = 0.0;
			bg.blue = 0.0;
			bg.alpha = 0.2;
			return bg;
		}
		
		uint32_t GetNuberOfColomn(void) {
			return 1;
		};
		bool GetTitle(int32_t colomn, etk::UString &myTitle, color_ts &fg, color_ts &bg) {
			myTitle = "title";
			return true;
		};
		uint32_t GetNuberOfRaw(void) {
			return m_listDirectory.Size();
		};
		bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, color_ts &fg, color_ts &bg) {
			if (raw >= 0 && raw < m_listDirectory.Size()) {
				myTextToWrite = *(m_listDirectory[raw]);
			} else {
				myTextToWrite = "ERROR";
			}
			fg.red = 0.0;
			fg.green = 0.0;
			fg.blue = 0.0;
			fg.alpha = 1.0;
			if (raw % 2) {
				bg.red = 1.0;
				bg.green = 1.0;
				bg.blue = 1.0;
				bg.alpha = 1.0;
			} else {
				bg.red = 0.5;
				bg.green = 0.5;
				bg.blue = 0.5;
				bg.alpha = 1.0;
			}
			if (m_selectedLine == raw) {
				bg.red = 0.6;
				bg.green = 0.6;
				bg.blue = 1.0;
				bg.alpha = 1.0;
			}
			return true;
		};
		
		bool OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent, int32_t colomn, int32_t raw, etkFloat_t x, etkFloat_t y) {
			if (typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE) {
				EWOL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
				if (1 == IdInput) {
					if (raw > m_listDirectory.Size() ) {
						m_selectedLine = -1;
					} else {
						m_selectedLine = raw;
					}
					// need to regenerate the display of the list : 
					MarkToReedraw();
					return true;
				}
			}
			if (typeEvent == ewol::EVENT_INPUT_TYPE_DOUBLE) {
				EWOL_INFO("Event Double on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
				if (1 == IdInput) {
					if (m_selectedLine >=0 ) {
						// generate event extern : 
						GenerateEventId(ewolEventFileChooserSelectFolder);
						return true;
					}
				}
			}
			return false;
		}
	
};

#undef __class__
#define __class__	"ewol::FileChooser(FileList)"

const char * const ewolEventFileChooserSelectFile   = "ewol-event-file-chooser-Select-File";
const char * const ewolEventFileChooserValidateFile   = "ewol-event-file-chooser-Validate-File";

class FileChooserFileList : public ewol::List
{
	private:
		etk::VectorType<etk::UString *> m_listFile;
		int32_t                        m_selectedLine;
	public:
		FileChooserFileList(void)
		{
			m_selectedLine = -1;
			AddEventId(ewolEventFileChooserSelectFile);
			AddEventId(ewolEventFileChooserValidateFile);
		};
		~FileChooserFileList(void)
		{
			ClearElements();
		};
		virtual color_ts GetBasicBG(void) {
			color_ts bg;
			bg.red = 0.0;
			bg.green = 0.0;
			bg.blue = 0.0;
			bg.alpha = 0.2;
			return bg;
		}
		void AddElement(etk::UString element)
		{
			etk::UString* tmpEmement = new etk::UString(element);
			m_listFile.PushBack(tmpEmement);
		}
		void EndGenerating(void)
		{
			SortList(m_listFile);
			MarkToReedraw();
		}
		void ClearElements(void) {
			for (int32_t iii=0; iii<m_listFile.Size(); iii++) {
				if (NULL != m_listFile[iii]) {
					delete(m_listFile[iii]);
					m_listFile[iii] = NULL;
				}
			}
			m_listFile.Clear();
			MarkToReedraw();
		}
		
		etk::UString GetSelectedLine(void)
		{
			etk::UString tmpVal = "";
			if (m_selectedLine >= 0) {
				tmpVal = *(m_listFile[m_selectedLine]);
			}
			return tmpVal;
		}
		
		
		uint32_t GetNuberOfColomn(void) {
			return 1;
		};
		bool GetTitle(int32_t colomn, etk::UString &myTitle, color_ts &fg, color_ts &bg) {
			myTitle = "title";
			return true;
		};
		uint32_t GetNuberOfRaw(void) {
			return m_listFile.Size();
		};
		bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, color_ts &fg, color_ts &bg) {
			if (raw >= 0 && raw < m_listFile.Size()) {
				myTextToWrite = *(m_listFile[raw]);
			} else {
				myTextToWrite = "ERROR";
			}
			fg.red = 0.0;
			fg.green = 0.0;
			fg.blue = 0.0;
			fg.alpha = 1.0;
			if (raw % 2) {
				bg.red = 0.9;
				bg.green = 1.0;
				bg.blue = 1.0;
				bg.alpha = 1.0;
			} else {
				bg.red = 0.7;
				bg.green = 0.7;
				bg.blue = 0.7;
				bg.alpha = 1.0;
			}
			if (m_selectedLine == raw) {
				bg.red = 0.6;
				bg.green = 0.6;
				bg.blue = 1.0;
				bg.alpha = 1.0;
			}
			return true;
		};
		
		bool OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent, int32_t colomn, int32_t raw, etkFloat_t x, etkFloat_t y) {
			if (typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE) {
				EWOL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
				if (1 == IdInput) {
					if (raw > m_listFile.Size() ) {
						m_selectedLine = -1;
					} else {
						m_selectedLine = raw;
					}
					// need to regenerate the display of the list : 
					MarkToReedraw();
					if (m_selectedLine >=0 ) {
						// generate event extern : 
						GenerateEventId(ewolEventFileChooserSelectFile);
						return true;
					}
					return true;
				}
			}
			if (typeEvent == ewol::EVENT_INPUT_TYPE_DOUBLE) {
				EWOL_INFO("Event Double on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
				if (1 == IdInput) {
					if (m_selectedLine >=0 ) {
						// generate event extern : 
						GenerateEventId(ewolEventFileChooserValidateFile);
						return true;
					}
				}
			}
			return false;
			return false;
		}
	
};


#undef __class__
#define __class__	"ewol::FileChooser"


extern const char * const ewolEventFileChooserCancel          = "ewol-event-file-chooser-cancel";
extern const char * const ewolEventFileChooserValidate        = "ewol-event-file-chooser-validate";
extern const char * const ewolEventFileChooserHidenFileChange = "ewol-event-file-chooser-Show/Hide-hiden-Files";
extern const char * const ewolEventFileChooserEntryFolder     = "ewol-event-file-chooser-modify-entry-folder";
extern const char * const ewolEventFileChooserEntryFile       = "ewol-event-file-chooser-modify-entry-file";


ewol::FileChooser::FileChooser(void)
{
	/*
	AddEventId(ewolEventFileChooserCancel);
	AddEventId(ewolEventFileChooserValidate);
	
	m_widgetTitleId = -1;
	m_widgetValidateId = -1;
	m_widgetCancelId = -1;
	m_widgetCurrentFolderId = -1;
	m_widgetCurrentFileNameId = -1;
	m_widgetListFolderId = -1;
	m_widgetListFileId = -1;
	m_widgetCheckBoxId = -1;
	m_hasSelectedFile = false;
	
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	ewol::Button * myButton = NULL;
	ewol::Entry * myEntry = NULL;
	ewol::Spacer * mySpacer = NULL;
	FileChooserFileList * myListFile = NULL;
	FileChooserFolderList * myListFolder = NULL;
	ewol::Label * myLabel = NULL;
	ewol::CheckBox * myCheckBox = NULL;
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
		
		myLabel = new ewol::Label("File chooser ...");
			m_widgetTitleId = myLabel->GetWidgetId();
			mySizerVert->SubWidgetAdd(myLabel);
		
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			myLabel = new ewol::Label("Folder : ");
				myLabel->SetFillY(true);
				mySizerHori->SubWidgetAdd(myLabel);
			myEntry = new ewol::Entry(m_folder);
				m_widgetCurrentFolderId = myEntry->GetWidgetId();
				myEntry->ExternLinkOnEvent(ewolEventEntryModify, GetWidgetId(), ewolEventFileChooserEntryFolder);
				myEntry->SetExpendX(true);
				myEntry->SetFillX(true);
				myEntry->SetWidth(200);
				mySizerHori->SubWidgetAdd(myEntry);
		
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			myLabel = new ewol::Label("File Name : ");
				myLabel->SetFillY(true);
				mySizerHori->SubWidgetAdd(myLabel);
			myEntry = new ewol::Entry(m_file);
				m_widgetCurrentFileNameId = myEntry->GetWidgetId();
				myEntry->ExternLinkOnEvent(ewolEventEntryModify, GetWidgetId(), ewolEventFileChooserEntryFile);
				myEntry->SetExpendX(true);
				myEntry->SetFillX(true);
				myEntry->SetWidth(200);
				mySizerHori->SubWidgetAdd(myEntry);
		
		mySizerHori = new ewol::SizerHori();
			mySizerVert->SubWidgetAdd(mySizerHori);
			mySpacer = new ewol::Spacer();
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			myListFolder = new FileChooserFolderList();
				m_widgetListFolderId = myListFolder->GetWidgetId();
				myListFolder->ExternLinkOnEvent(ewolEventFileChooserSelectFolder, GetWidgetId(), ewolEventFileChooserSelectFolder);
				myListFolder->SetExpendY(true);
				myListFolder->SetFillY(true);
				mySizerHori->SubWidgetAdd(myListFolder);
			mySpacer = new ewol::Spacer();
				mySpacer->SetSize(2);
				mySizerHori->SubWidgetAdd(mySpacer);
			myListFile = new FileChooserFileList();
				m_widgetListFileId = myListFile->GetWidgetId();
				myListFile->ExternLinkOnEvent(ewolEventFileChooserSelectFile,   GetWidgetId(), ewolEventFileChooserSelectFile);
				myListFile->ExternLinkOnEvent(ewolEventFileChooserValidateFile, GetWidgetId(), ewolEventFileChooserValidateFile);
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
			myCheckBox = new ewol::CheckBox("Show hiden files");
				m_widgetCheckBoxId = myCheckBox->GetWidgetId();
				myCheckBox->ExternLinkOnEvent(ewolEventCheckBoxClicked, GetWidgetId(), ewolEventFileChooserHidenFileChange);
				myCheckBox->SetValue(false);
				mySizerHori->SubWidgetAdd(myCheckBox);
			mySpacer = new ewol::Spacer();
				mySpacer->SetExpendX(true);
				mySizerHori->SubWidgetAdd(mySpacer);
			myButton = new ewol::Button("Open");
				m_widgetValidateId = myButton->GetWidgetId();
				myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ewolEventFileChooserValidate);
				mySizerHori->SubWidgetAdd(myButton);
			myButton = new ewol::Button("Cancel");
				m_widgetCancelId = myButton->GetWidgetId();
				myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ewolEventFileChooserCancel);
				mySizerHori->SubWidgetAdd(myButton);
	
	// set the default Folder properties:
	UpdateCurrentFolder();
	*/
}


ewol::FileChooser::~FileChooser(void)
{
	
}


void ewol::FileChooser::SetTitle(etk::UString label)
{
	/*
	ewol::Label * tmpWidget = dynamic_cast<ewol::Label*>(ewol::widgetManager::Get(m_widgetTitleId));
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetLabel(label);
	*/
}

void ewol::FileChooser::SetValidateLabel(etk::UString label)
{
	/*
	ewol::Button * tmpWidget = dynamic_cast<ewol::Button*>(ewol::widgetManager::Get(m_widgetValidateId));
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetLabel(label);
	*/
}

void ewol::FileChooser::SetCancelLabel(etk::UString label)
{
	/*
	ewol::Button * tmpWidget = dynamic_cast<ewol::Button*>(ewol::widgetManager::Get(m_widgetCancelId));
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetLabel(label);
	*/
}

void ewol::FileChooser::SetFolder(etk::UString folder)
{
	/*
	m_folder = folder;
	UpdateCurrentFolder();
	*/
}

void ewol::FileChooser::SetFileName(etk::UString filename)
{
	/*
	m_file = filename;
	ewol::Entry * tmpWidget = dynamic_cast<ewol::Entry*>(ewol::widgetManager::Get(m_widgetCurrentFileNameId));
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetValue(filename);
	*/
}



bool ewol::FileChooser::OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * data, etkFloat_t x, etkFloat_t y)
{
	/*
	EWOL_INFO("Receive Event from the LIST ... : widgetid=" << widgetID << "\"" << generateEventId << "\" ==> data=\"" << data << "\"" );
	if (ewolEventFileChooserEntryFolder == generateEventId) {
		//==> change the folder name
		// TODO : Change the folder, if it exit ...
		return true;
	} else if (ewolEventFileChooserEntryFile == generateEventId) {
		//==> change the file name
		ewol::Entry * tmpWidget = dynamic_cast<ewol::Entry*>(ewol::widgetManager::Get(m_widgetCurrentFileNameId));
		if (NULL != tmpWidget) {
			m_file = tmpWidget->GetValue();
		}
		// TODO : Remove file selection
		return true;
	} else if (ewolEventFileChooserCancel == generateEventId) {
		//==> Auto remove ...
		bool tmppp = GenEventInputExternal(generateEventId, x, y);
		ewol::RmPopUp(GetWidgetId());
		return tmppp;
	} else if (ewolEventFileChooserHidenFileChange == generateEventId) {
		// regenerate the display ...
		UpdateCurrentFolder();
		return true;
	} else if (ewolEventFileChooserSelectFolder == generateEventId) {
		//==> this is an internal event ...
		FileChooserFolderList * myListFolder = dynamic_cast<FileChooserFolderList *>(ewol::widgetManager::Get(m_widgetListFolderId));
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
		return true;
	} else if (ewolEventFileChooserSelectFile == generateEventId) {
		m_hasSelectedFile = true;
		FileChooserFileList * myListFile     = dynamic_cast<FileChooserFileList *>(ewol::widgetManager::Get(m_widgetListFileId));
		etk::UString file = myListFile->GetSelectedLine();
		SetFileName(file);
		GenEventInputExternal(generateEventId, x, y);
	} else if(    ewolEventFileChooserValidateFile == generateEventId
	           || (ewolEventFileChooserValidate == generateEventId && true == m_hasSelectedFile) ) {
		// select the File ==> generate a validate
		bool tmppp = GenEventInputExternal(ewolEventFileChooserValidate, x, y);
		ewol::RmPopUp(GetWidgetId());
		return tmppp;
	}
	*/
	return false;
};



void ewol::FileChooser::UpdateCurrentFolder(void)
{
	/*
	FileChooserFileList * myListFile     = dynamic_cast<FileChooserFileList *>(ewol::widgetManager::Get(m_widgetListFileId));
	FileChooserFolderList * myListFolder = dynamic_cast<FileChooserFolderList *>(ewol::widgetManager::Get(m_widgetListFolderId));
	ewol::Entry * myEntry                = dynamic_cast<ewol::Entry *>(ewol::widgetManager::Get(m_widgetCurrentFolderId));
	ewol::CheckBox * myhidenFiles        = dynamic_cast<ewol::CheckBox *>(ewol::widgetManager::Get(m_widgetCheckBoxId));
	
	myListFile->ClearElements();
	myListFolder->ClearElements();
	bool ShowHidenFile = true;
	if (NULL != myhidenFiles) {
		ShowHidenFile = myhidenFiles->GetValue();
	} else {
		EWOL_ERROR("Can not get the hiden property of the file choozer...");
	}
	
	
	myEntry->SetValue(m_folder);
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
	*/
}


etk::UString ewol::FileChooser::GetCompleateFileName(void)
{
	/*
	
	etk::UString tmpString = m_folder;
	tmpString += "/";
	tmpString += m_file;
	return tmpString;
	*/
}



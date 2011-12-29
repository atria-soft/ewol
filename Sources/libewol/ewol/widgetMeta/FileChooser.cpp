/**
 *******************************************************************************
 * @file ewol/widgetMeta/FileChooser.cpp
 * @brief ewol File chooser complex widget system (Sources)
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
#include <ewol/WidgetManager.h>
//#include <etk/Vector.h>
#include <etk/VectorType.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ewol::FileChooser(FolderList)"

class FileChooserFolderList : public ewol::List
{
	private:
		//etk::Vector<etk::String> m_listDirectory;
		etk::VectorType<etk::String * > m_listDirectory;
	public:
		FileChooserFolderList(void)
		{
			DIR *dir;
			struct dirent *ent;
			dir = opendir ("/");
			if (dir != NULL) {
				/* print all the files and directories within directory */
				while ((ent = readdir (dir)) != NULL) {
					etk::String * tmpString = new etk::String(ent->d_name);
					m_listDirectory.PushBack(tmpString);
					EWOL_INFO("file : " << *tmpString);
				}
				closedir (dir);
			} else {
				EWOL_ERROR("could not open directory");
			}
		};
		~FileChooserFolderList(void) { };
		virtual color_ts GetBasicBG(void) {
			color_ts bg;
			bg.red = 1.0;
			bg.green = 0.0;
			bg.blue = 0.0;
			bg.alpha = 1.0;
			return bg;
		}
		
		uint32_t GetNuberOfColomn(void) {
			return 1;
		};
		bool GetTitle(int32_t colomn, etk::String &myTitle, color_ts &fg, color_ts &bg) {
			myTitle = "title";
			return true;
		};
		uint32_t GetNuberOfRaw(void) {
			return m_listDirectory.Size();
		};
		bool GetElement(int32_t colomn, int32_t raw, etk::String &myTextToWrite, color_ts &fg, color_ts &bg) {
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
			return true;
		};
		
		bool OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent, int32_t colomn, int32_t raw, etkFloat_t x, etkFloat_t y) {
			if (typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE) {
				EWOL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
			}
			return false;
		}
	
};

#undef __class__
#define __class__	"ewol::FileChooser(FileList)"

class FileChooserFileList : public ewol::List
{
	public:
		FileChooserFileList(void) { };
		~FileChooserFileList(void) { };
		virtual color_ts GetBasicBG(void) {
			color_ts bg;
			bg.red = 1.0;
			bg.green = 0.0;
			bg.blue = 0.0;
			bg.alpha = 1.0;
			return bg;
		}
		
		uint32_t GetNuberOfColomn(void) {
			return 1;
		};
		bool GetTitle(int32_t colomn, etk::String &myTitle, color_ts &fg, color_ts &bg) {
			myTitle = "title";
			return true;
		};
		uint32_t GetNuberOfRaw(void) {
			return 3;
		};
		bool GetElement(int32_t colomn, int32_t raw, etk::String &myTextToWrite, color_ts &fg, color_ts &bg) {
			switch (raw) {
				case 0:
					myTextToWrite = "File 1.cpp";
					break;
				case 1:
					myTextToWrite = "File 2.h";
					break;
				case 2:
					myTextToWrite = "Makefile";
					break;
				default:
					myTextToWrite = "ERROR";
					break;
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
			return true;
		};
		
		bool OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent, int32_t colomn, int32_t raw, etkFloat_t x, etkFloat_t y) {
			if (typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE) {
				EWOL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
			}
			return false;
		}
	
};


#undef __class__
#define __class__	"ewol::FileChooser"


const char * const ewolEventFileChooserCancel   = "ewol event file chooser cancel";
const char * const ewolEventFileChooserValidate = "ewol event file chooser validate";
const char * const ewolEventFileChooserFolderUp = "ewol event file chooser folderUP";


ewol::FileChooser::FileChooser(void)
{
	AddEventId(ewolEventFileChooserCancel);
	AddEventId(ewolEventFileChooserValidate);
	AddEventId(ewolEventFileChooserFolderUp);
	
	m_widgetTitleId = -1;
	m_widgetValidateId = -1;
	m_widgetCancelId = -1;
	m_widgetCurrentFolderId = -1;
	m_widgetListFolderId = -1;
	m_widgetListFileId = -1;
	
	
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	ewol::Button * myButton = NULL;
	ewol::Entry * myEntry = NULL;
	ewol::Spacer * mySpacer = NULL;
	FileChooserFileList * myListFile = NULL;
	FileChooserFolderList * myListFolder = NULL;
	ewol::Label * myLabel = NULL;
	
	mySizerVert = new ewol::SizerVert();
		// set it in the pop-up-system : 
		SubWidgetSet(mySizerVert);
		
		myLabel = new ewol::Label("File chooser ...");
			m_widgetTitleId = myLabel->GetWidgetId();
			mySizerVert->SubWidgetAdd(myLabel);
		
		mySizerHori = new ewol::SizerHori();
			mySizerHori->LockExpendContamination(true);
			mySizerVert->SubWidgetAdd(mySizerHori);
			myButton = new ewol::Button("<-");
				myButton->ExternLinkOnEvent("ewol Button Pressed", GetWidgetId(), ewolEventFileChooserFolderUp );
				mySizerHori->SubWidgetAdd(myButton);
			myEntry = new ewol::Entry("~/");
				m_widgetCurrentFolderId = myEntry->GetWidgetId();
				myEntry->SetExpendX(true);
				myEntry->SetFillX(true);
				myEntry->SetWidth(200);
				mySizerHori->SubWidgetAdd(myEntry);
			
		mySizerHori = new ewol::SizerHori();
			mySizerHori->LockExpendContamination(true);
			mySizerVert->SubWidgetAdd(mySizerHori);
			myListFolder = new FileChooserFolderList();
				m_widgetListFolderId = myListFolder->GetWidgetId();
				//myList->SetExpendX(true);
				myListFolder->SetExpendY(true);
				myListFolder->SetFillY(true);
				mySizerHori->SubWidgetAdd(myListFolder);
			myListFile = new FileChooserFileList();
				m_widgetListFileId = myListFile->GetWidgetId();
				myListFile->SetExpendY(true);
				myListFile->SetFillX(true);
				myListFile->SetExpendY(true);
				myListFile->SetFillY(true);
				mySizerHori->SubWidgetAdd(myListFile);
			
		mySizerHori = new ewol::SizerHori();
			mySizerHori->LockExpendContamination(true);
			mySizerVert->SubWidgetAdd(mySizerHori);
			mySpacer = new ewol::Spacer();
				mySpacer->SetExpendX(true);
				mySizerHori->SubWidgetAdd(mySpacer);
			myButton = new ewol::Button("Open");
				m_widgetValidateId = myButton->GetWidgetId();
				myButton->ExternLinkOnEvent("ewol Button Pressed", GetWidgetId(), ewolEventFileChooserValidate);
				mySizerHori->SubWidgetAdd(myButton);
			myButton = new ewol::Button("Cancel");
				m_widgetCancelId = myButton->GetWidgetId();
				myButton->ExternLinkOnEvent("ewol Button Pressed", GetWidgetId(), ewolEventFileChooserCancel);
				mySizerHori->SubWidgetAdd(myButton);
}


ewol::FileChooser::~FileChooser(void)
{
	
}


void ewol::FileChooser::SetTitle(etk::String label)
{
	ewol::Label * tmpWidget = (ewol::Label*)ewol::widgetManager::Get(m_widgetTitleId);
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetLabel(label);
}

void ewol::FileChooser::SetValidateLabel(etk::String label)
{
	ewol::Button * tmpWidget = (ewol::Button*)ewol::widgetManager::Get(m_widgetValidateId);
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetLabel(label);
}

void ewol::FileChooser::SetCancelLabel(etk::String label)
{
	ewol::Button * tmpWidget = (ewol::Button*)ewol::widgetManager::Get(m_widgetCancelId);
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetLabel(label);
}

void ewol::FileChooser::SetFolder(etk::String folder)
{
	m_folder = folder;
}



bool ewol::FileChooser::OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y)
{
	EWOL_INFO("Receive Event from the BT ... : widgetid=" << widgetID << "\"" << generateEventId << "\" ==> internalEvent=\"" << eventExternId << "\"" );
	if (ewolEventFileChooserCancel == eventExternId) {
		//==> Auto remove ...
		
	}
	return GenEventInputExternal(eventExternId, x, y);
};
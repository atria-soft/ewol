/**
 *******************************************************************************
 * @file ewol/widget/ListFile.cpp
 * @brief ewol File lister widget system (Sources)
 * @author Edouard DUPIN
 * @date 12/07/2012
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

#include <ewol/widget/ListFileSystem.h>
#include <etk/tool.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ListFileSystem"

extern const char * const ewolEventFSFileSelect     = "ewol-event-file-system-file-select";
extern const char * const ewolEventFSFileValidate   = "ewol-event-file-system-file-validate";
extern const char * const ewolEventFSFolderSelect   = "ewol-event-file-system-folder-select";
extern const char * const ewolEventFSFolderValidate = "ewol-event-file-system-folder-validate";

//!< EObject name :
extern const char * const TYPE_EOBJECT_WIDGET_FILE_LIST = "FileChooserFileList";



static void SortElementList(etk::VectorType<ewol::elementFS *> &list)
{
	etk::VectorType<ewol::elementFS *> tmpList = list;
	list.Clear();
	for(int32_t iii=0; iii<tmpList.Size(); iii++) {
		if (NULL != tmpList[iii]) {
			int32_t findPos = 0;
			for(int32_t jjj=0; jjj<list.Size(); jjj++) {
				//EWOL_DEBUG("compare : \""<<*tmpList[iii] << "\" and \"" << *m_listDirectory[jjj] << "\"");
				if (list[jjj]!=NULL) {
					if (tmpList[iii]->m_name > list[jjj]->m_name) {
						findPos = jjj+1;
					}
				}
			}
			//EWOL_DEBUG("position="<<findPos);
			list.Insert(findPos, tmpList[iii]);
		}
	}
}



ewol::ListFileSystem::ListFileSystem(void)
{
	m_selectedLine = -1;
	m_showFile = true;
	m_showTemporaryFile = true;
	m_showHidden = true;
	m_showFolder = true;
	m_folder = "/";
	AddEventId(ewolEventFSFileSelect);
	AddEventId(ewolEventFSFileValidate);
	AddEventId(ewolEventFSFolderSelect);
	AddEventId(ewolEventFSFolderValidate);
	SetMouseLimit(2);
};

ewol::ListFileSystem::~ListFileSystem(void)
{
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
};

color_ts ewol::ListFileSystem::GetBasicBG(void) {
	color_ts bg(0x00000010);
	return bg;
}


void ewol::ListFileSystem::RegenerateView(void)
{
	// clean the list of files : 
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	m_list.Clear();
	m_originScrooled.x = 0;
	m_originScrooled.y = 0;
	
	// regenerate the nex list :
	ewol::elementFS* tmpEmement = NULL;
	if (true==m_showFolder) {
		// the "." permit to reload the curent folder
		tmpEmement = new ewol::elementFS(".", ewol::EFS_FOLDER);
		if (NULL != tmpEmement) {
			m_list.PushBack(tmpEmement);
		}
		tmpEmement = NULL;
		// the ".." permit to show the upper folder (but not availlable for the "/" folder
		if (m_folder != "/") {
			tmpEmement = new ewol::elementFS("..", ewol::EFS_FOLDER);
			if (NULL != tmpEmement) {
				m_list.PushBack(tmpEmement);
			}
			tmpEmement = NULL;
		}
	}
	DIR *dir = NULL;
	struct dirent *ent = NULL;
	dir = opendir(m_folder.Utf8Data());
	if (dir != NULL) {
		// for each element in the drectory...
		while ((ent = readdir(dir)) != NULL) {
			if(    true==m_showFile
			    && DT_REG==ent->d_type) {
				if (ent->d_name != NULL) {
					etk::UString tmpString(ent->d_name);
					if(    false==tmpString.StartWith(".")
					    || true ==m_showHidden) {
						tmpEmement = new ewol::elementFS(tmpString, ewol::EFS_FILE);
						if (NULL != tmpEmement) {
							m_list.PushBack(tmpEmement);
						}
						tmpEmement = NULL;
					}
				}
			}
			if(    true==m_showFolder
			    && DT_DIR==ent->d_type) {
				if (ent->d_name != NULL) {
					etk::UString tmpString(ent->d_name);
					//EWOL_DEBUG("    find Folder : \"" << tmpString << "\"(" << tmpString.Size() << ") ?= \"" << ent->d_name << "\"(" << strlen(ent->d_name) );
					if (tmpString != "." && tmpString != "..") {
						if(    false==tmpString.StartWith(".")
						    || true ==m_showHidden) {
							tmpEmement = new ewol::elementFS(tmpString, ewol::EFS_FOLDER);
							if (NULL != tmpEmement) {
								m_list.PushBack(tmpEmement);
							}
							tmpEmement = NULL;
						}
					}
				}
			}
		}
		closedir(dir);
	} else {
		EWOL_ERROR("could not open directory : \"" << m_folder << "\"");
	}
	
	// reorder the files
	SortElementList(m_list);
	// request a redraw ...
	MarkToReedraw();
}

void ewol::ListFileSystem::SetShowHiddenFiles(bool state)
{
	m_showHidden = state;
	RegenerateView();
}

void ewol::ListFileSystem::SetShowTemporaryFiles(bool state)
{
	m_showTemporaryFile = state;
	RegenerateView();
}

void ewol::ListFileSystem::SetShowFiles(bool state)
{
	m_showFile = state;
	RegenerateView();
}

void ewol::ListFileSystem::SetShowFolder(bool state)
{
	m_showFolder = state;
	RegenerateView();
}


void ewol::ListFileSystem::SetFolder(etk::UString newFolder)
{
	m_folder = newFolder;
	RegenerateView();
}


etk::UString ewol::ListFileSystem::GetFolder(void)
{
	return m_folder;
}


etk::UString ewol::ListFileSystem::GetSelect(void)
{
	etk::UString tmpVal = "";
	if (m_selectedLine >= 0) {
		if (m_list[m_selectedLine] != NULL) {
			tmpVal = m_list[m_selectedLine]->m_name;
		}
	}
	return tmpVal;
}

// select the specific file
void ewol::ListFileSystem::SetSelect( etk::UString data) {
	// remove selected line
	m_selectedLine = -1;
	// search the coresponding file :
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL!=m_list[iii]) {
			if (m_list[iii]->m_name == data) {
				// we find the line :
				m_selectedLine = iii;
				break;
			}
		}
	}
	MarkToReedraw();
}

uint32_t ewol::ListFileSystem::GetNuberOfColomn(void) {
	return 1;
};
bool ewol::ListFileSystem::GetTitle(int32_t colomn, etk::UString &myTitle, color_ts &fg, color_ts &bg) {
	myTitle = "title";
	return true;
};
uint32_t ewol::ListFileSystem::GetNuberOfRaw(void) {
	return m_list.Size();
};
bool ewol::ListFileSystem::GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, color_ts &fg, color_ts &bg) {
	if (raw >= 0 && raw < m_list.Size() && NULL != m_list[raw]) {
		myTextToWrite = m_list[raw]->m_name;
	} else {
		myTextToWrite = "ERROR";
	}
	fg = etk::color::color_Black;
	if (raw % 2) {
		bg = 0xFFFFFF00;
	} else {
		bg = 0xBFBFBFFF;
	}
	if (m_selectedLine == raw) {
		bg = 0x8F8FFFFF;
	}
	return true;
};

bool ewol::ListFileSystem::OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent, int32_t colomn, int32_t raw, float x, float y) {
	if (typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE) {
		EWOL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
		if (1 == IdInput) {
			if (raw > m_list.Size() ) {
				m_selectedLine = -1;
			} else {
				m_selectedLine = raw;
			}
			// need to regenerate the display of the list : 
			MarkToReedraw();
			if (m_selectedLine >=0 ) {
				// generate event extern : 
				switch(m_list[m_selectedLine]->m_type)
				{
					case ewol::EFS_FILE :
						GenerateEventId(ewolEventFSFileSelect, m_list[m_selectedLine]->m_name);
						break;
					case ewol::EFS_FOLDER :
						GenerateEventId(ewolEventFSFolderSelect, m_list[m_selectedLine]->m_name);
						break;
					default:
						EWOL_ERROR("Can not generate event on an unknow type");
						break;
				}
			}
			return true;
		}
	}
	if (typeEvent == ewol::EVENT_INPUT_TYPE_DOUBLE) {
		EWOL_INFO("Event Double on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
		if (1 == IdInput) {
			if (m_selectedLine >=0 ) {
				switch(m_list[m_selectedLine]->m_type)
				{
					case ewol::EFS_FILE :
						GenerateEventId(ewolEventFSFileValidate, m_list[m_selectedLine]->m_name);
						break;
					case ewol::EFS_FOLDER :
						GenerateEventId(ewolEventFSFolderValidate, m_list[m_selectedLine]->m_name);
						break;
					default:
						EWOL_ERROR("Can not generate event on an unknow type");
						break;
				}
				return true;
			}
		}
	}
	return false;
}

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::ListFileSystem::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << TYPE_EOBJECT_WIDGET_FILE_LIST << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == TYPE_EOBJECT_WIDGET_FILE_LIST) {
		return true;
	} else {
		if(true == ewol::List::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << TYPE_EOBJECT_WIDGET_FILE_LIST << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::ListFileSystem::GetObjectType(void)
{
	return TYPE_EOBJECT_WIDGET_FILE_LIST;
}


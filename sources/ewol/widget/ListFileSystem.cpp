/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/ListFileSystem.h>
#include <etk/tool.h>

#include <etk/os/FSNode.h>


#undef __class__
#define __class__	"ListFileSystem"

extern const char * const ewolEventFSFileSelect     = "ewol-event-file-system-file-select";
extern const char * const ewolEventFSFileValidate   = "ewol-event-file-system-file-validate";
extern const char * const ewolEventFSFolderSelect   = "ewol-event-file-system-folder-select";
extern const char * const ewolEventFSFolderValidate = "ewol-event-file-system-folder-validate";


widget::ListFileSystem::ListFileSystem(void)
{
	m_selectedLine = -1;
	m_showFile = true;
	m_showTemporaryFile = true;
	m_showHidden = true;
	m_showFolder = true;
	#if defined(__TARGET_OS__Windows)
		m_folder = "c:/";
	#else
		m_folder = "/";
	#endif
	AddEventId(ewolEventFSFileSelect);
	AddEventId(ewolEventFSFileValidate);
	AddEventId(ewolEventFSFolderSelect);
	AddEventId(ewolEventFSFolderValidate);
	SetMouseLimit(1);
};

widget::ListFileSystem::~ListFileSystem(void)
{
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
};

draw::Color widget::ListFileSystem::GetBasicBG(void) {
	draw::Color bg(0x00000010);
	return bg;
}


void widget::ListFileSystem::RegenerateView(void)
{
	// clean the list of files : 
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	
	m_selectedLine = -1;
	m_list.Clear();
	m_originScrooled.x = 0;
	m_originScrooled.y = 0;
	etk::FSNode tmpFolder(m_folder);
	// Get the new list : 
	m_list = tmpFolder.FolderGetSubList(m_showHidden, m_showFolder, m_showFile, m_showTemporaryFile);
	// request a redraw ...
	MarkToRedraw();
}

void widget::ListFileSystem::SetShowHiddenFiles(bool state)
{
	m_showHidden = state;
	RegenerateView();
}

void widget::ListFileSystem::SetShowTemporaryFiles(bool state)
{
	m_showTemporaryFile = state;
	RegenerateView();
}

void widget::ListFileSystem::SetShowFiles(bool state)
{
	m_showFile = state;
	RegenerateView();
}

void widget::ListFileSystem::SetShowFolder(bool state)
{
	m_showFolder = state;
	RegenerateView();
}


void widget::ListFileSystem::SetFolder(etk::UString newFolder)
{
	m_folder = newFolder;
	RegenerateView();
}


etk::UString widget::ListFileSystem::GetFolder(void)
{
	return m_folder;
}


etk::UString widget::ListFileSystem::GetSelect(void)
{
	etk::UString tmpVal = "";
	if (m_selectedLine >= 0) {
		if (m_list[m_selectedLine] != NULL) {
			tmpVal = m_list[m_selectedLine]->GetNameFile();
		}
	}
	return tmpVal;
}

// select the specific file
void widget::ListFileSystem::SetSelect( etk::UString data) {
	// remove selected line
	m_selectedLine = -1;
	// search the coresponding file :
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL!=m_list[iii]) {
			if (m_list[iii]->GetNameFile() == data) {
				// we find the line :
				m_selectedLine = iii;
				break;
			}
		}
	}
	MarkToRedraw();
}

uint32_t widget::ListFileSystem::GetNuberOfColomn(void) {
	return 1;
};
bool widget::ListFileSystem::GetTitle(int32_t colomn, etk::UString &myTitle, draw::Color &fg, draw::Color &bg) {
	myTitle = "title";
	return true;
};
uint32_t widget::ListFileSystem::GetNuberOfRaw(void)
{
	int32_t offset = 0;
	if (true == m_showFolder) {
		offset = 2;
	}
	return m_list.Size() + offset;
};
bool widget::ListFileSystem::GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, draw::Color &fg, draw::Color &bg)
{
	int32_t offset = 0;
	if (true == m_showFolder) {
		offset = 2;
		if (raw==0) {
			myTextToWrite = ".";
		} else if (raw==1) {
			myTextToWrite = "..";
		}
	}
	if(    raw-offset >= 0
	    && raw-offset < m_list.Size()
	    && NULL != m_list[raw-offset]) {
		/*if (etk::FSN_FILE == m_list[raw-offset]->GetNodeType()) {
			myTextToWrite = m_list[raw-offset]->GetRight().GetRight();
			myTextToWrite += " ";
			myTextToWrite += m_list[raw-offset]->GetNameFile();
		} else */{
			myTextToWrite = m_list[raw-offset]->GetNameFile();
		}
	}
	fg = draw::color::black;
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


bool widget::ListFileSystem::OnItemEvent(int32_t IdInput, ewol::keyEvent::status_te typeEvent, int32_t colomn, int32_t raw, float x, float y)
{
	int32_t offset = 0;
	if (true == m_showFolder) {
		offset = 2;
	}
	if (typeEvent == ewol::keyEvent::statusSingle) {
		EWOL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
		if (1 == IdInput) {
			int32_t previousRaw = m_selectedLine;
			if (raw > m_list.Size()+offset ) {
				m_selectedLine = -1;
			} else {
				m_selectedLine = raw;
			}
			if (previousRaw != m_selectedLine) {
				if(    true == m_showFolder
				    && m_selectedLine ==0) {
					// "." folder
					GenerateEventId(ewolEventFSFolderSelect, ".");
				} else if (    true == m_showFolder
				            && m_selectedLine ==1) {
					// ".." folder
					GenerateEventId(ewolEventFSFolderSelect, "..");
				} else if(    m_selectedLine-offset >=0
				           && m_selectedLine-offset < m_list.Size()
				           && NULL != m_list[m_selectedLine-offset] ) {
					// generate event extern : 
					switch(m_list[m_selectedLine-offset]->GetNodeType())
					{
						case etk::FSN_FILE :
							GenerateEventId(ewolEventFSFileSelect, m_list[m_selectedLine-offset]->GetNameFile());
							break;
						case etk::FSN_FOLDER :
							GenerateEventId(ewolEventFSFolderSelect, m_list[m_selectedLine-offset]->GetNameFile());
							break;
						default:
							EWOL_ERROR("Can not generate event on an unknow type");
							break;
					}
				}
			} else {
				if(    true == m_showFolder
				    && m_selectedLine ==0) {
					// "." folder
					GenerateEventId(ewolEventFSFolderValidate, ".");
				} else if (    true == m_showFolder
				            && m_selectedLine ==1) {
					// ".." folder
					GenerateEventId(ewolEventFSFolderValidate, "..");
				} else if(    m_selectedLine-offset >=0
				           && m_selectedLine-offset < m_list.Size()
				           && NULL != m_list[m_selectedLine-offset] ) {
					switch(m_list[m_selectedLine-offset]->GetNodeType())
					{
						case etk::FSN_FILE :
							GenerateEventId(ewolEventFSFileValidate, m_list[m_selectedLine-offset]->GetNameFile());
							break;
						case etk::FSN_FOLDER :
							GenerateEventId(ewolEventFSFolderValidate, m_list[m_selectedLine-offset]->GetNameFile());
							break;
						default:
							EWOL_ERROR("Can not generate event on an unknow type");
							break;
					}
				}
			}
			// need to regenerate the display of the list : 
			MarkToRedraw();
			return true;
		}
	}
	return false;
}

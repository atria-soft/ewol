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

etk::Color<> widget::ListFileSystem::GetBasicBG(void) {
	return etk::Color<>(0x00000010);
}


void widget::ListFileSystem::RegenerateView(void)
{
	// clean the list of files : 
	for (esize_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	
	m_selectedLine = -1;
	m_list.Clear();
	m_originScrooled.setValue(0,0);
	etk::FSNode tmpFolder(m_folder);
	// Get the new list : 
	m_list = tmpFolder.FolderGetSubList(m_showHidden, m_showFolder, m_showFile, m_showTemporaryFile);
	// request a redraw ...
	MarkToRedraw();
}

void widget::ListFileSystem::SetShowHiddenFiles(bool _state)
{
	m_showHidden = _state;
	RegenerateView();
}

void widget::ListFileSystem::SetShowTemporaryFiles(bool _state)
{
	m_showTemporaryFile = _state;
	RegenerateView();
}

void widget::ListFileSystem::SetShowFiles(bool _state)
{
	m_showFile = _state;
	RegenerateView();
}

void widget::ListFileSystem::SetShowFolder(bool _state)
{
	m_showFolder = _state;
	RegenerateView();
}


void widget::ListFileSystem::SetFolder(etk::UString _newFolder)
{
	m_folder = _newFolder;
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
void widget::ListFileSystem::SetSelect( etk::UString _data) {
	// remove selected line
	m_selectedLine = -1;
	// search the coresponding file :
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL!=m_list[iii]) {
			if (m_list[iii]->GetNameFile() == _data) {
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
bool widget::ListFileSystem::GetTitle(int32_t _colomn, etk::UString &_myTitle, etk::Color<>& _fg, etk::Color<>& _bg) {
	_myTitle = "title";
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
bool widget::ListFileSystem::GetElement(int32_t _colomn, int32_t _raw, etk::UString& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg)
{
	int32_t offset = 0;
	if (true == m_showFolder) {
		offset = 2;
		if (_raw==0) {
			_myTextToWrite = ".";
		} else if (_raw==1) {
			_myTextToWrite = "..";
		}
	}
	if(    _raw-offset >= 0
	    && _raw-offset < m_list.Size()
	    && NULL != m_list[_raw-offset]) {
		/*if (etk::FSN_FILE == m_list[raw-offset]->GetNodeType()) {
			myTextToWrite = m_list[raw-offset]->GetRight().GetRight();
			myTextToWrite += " ";
			myTextToWrite += m_list[raw-offset]->GetNameFile();
		} else */{
			_myTextToWrite = m_list[_raw-offset]->GetNameFile();
		}
	}
	_fg = etk::color::black;
	if (_raw % 2) {
		_bg = 0xFFFFFF00;
	} else {
		_bg = 0xBFBFBFFF;
	}
	if (m_selectedLine == _raw) {
		_bg = 0x8F8FFFFF;
	}
	return true;
};


bool widget::ListFileSystem::OnItemEvent(int32_t _IdInput, ewol::keyEvent::status_te _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y)
{
	int32_t offset = 0;
	if (true == m_showFolder) {
		offset = 2;
	}
	if (_typeEvent == ewol::keyEvent::statusSingle) {
		EWOL_INFO("Event on List : IdInput=" << _IdInput << " colomn=" << _colomn << " raw=" << _raw );
		if (1 == _IdInput) {
			int32_t previousRaw = m_selectedLine;
			if (_raw > m_list.Size()+offset ) {
				m_selectedLine = -1;
			} else {
				m_selectedLine = _raw;
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


/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/ListFileSystem.hpp>
#include <etk/tool.hpp>
#include <etk/os/FSNode.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ListFileSystem);

ewol::widget::ListFileSystem::ListFileSystem() :
  signalFileSelect(this, "file-select", ""),
  signalFileValidate(this, "file-validate", ""),
  signalFolderSelect(this, "folder-select", ""),
  signalFolderValidate(this, "folder-validate", ""),
  propertyPath(this, "path",
                     "/",
                     "Path to display",
                     &ewol::widget::ListFileSystem::onChangePropertyPath),
  propertyFile(this, "select",
                     "",
                     "selection af a specific file",
                     &ewol::widget::ListFileSystem::onChangePropertyFile),
  propertyShowFile(this, "show-file",
                         true,
                         "display files",
                         &ewol::widget::ListFileSystem::onChangePropertyShowFile),
  propertyShowFolder(this, "show-folder",
                           true,
                           "display folders",
                           &ewol::widget::ListFileSystem::onChangePropertyShowFolder),
  propertyShowHidden(this, "show-hidden",
                           true,
                           "Show the hidden element (file, folder, ...)",
                           &ewol::widget::ListFileSystem::onChangePropertyShowHidden),
  propertyFilter(this, "filter",
                       "",
                       "regex to filter files ...",
                       &ewol::widget::ListFileSystem::onChangePropertyFilter),
  m_selectedLine(-1) {
	addObjectType("ewol::widget::ListFileSystem");
	#if defined(__TARGET_OS__Windows)
		propertyPath.setDirectCheck("c:/");
	#endif
	m_colorProperty = ewol::resource::ColorFile::create("{ewol}THEME:COLOR:ListFileSystem.json");
	if (m_colorProperty != null) {
		m_colorIdText = m_colorProperty->request("text");
		m_colorIdBackground1 = m_colorProperty->request("background1");
		m_colorIdBackground2 = m_colorProperty->request("background2");
		m_colorIdBackgroundSelected = m_colorProperty->request("selected");
	}
	setMouseLimit(1);
}

ewol::widget::ListFileSystem::~ListFileSystem() {
	clearList();
}

void ewol::widget::ListFileSystem::clearList() {
	for (auto &it : m_list) {
		if (it != null) {
			ETK_DELETE(etk::FSNode, it);
			it = null;
		}
	}
}

etk::Color<> ewol::widget::ListFileSystem::getBasicBG() {
	return m_colorProperty->get(m_colorIdBackground1);
}


void ewol::widget::ListFileSystem::regenerateView() {
	clearList();
	m_selectedLine = -1;
	m_list.clear();
	m_originScrooled.setValue(0,0);
	etk::FSNode tmpFolder(*propertyPath);
	// get the list:
	m_list = tmpFolder.folderGetSubList(*propertyShowHidden, *propertyShowFolder, *propertyShowFile, *propertyFilter);
	// request a redraw ...
	markToRedraw();
}

etk::String ewol::widget::ListFileSystem::getSelect() const {
	etk::String tmpVal = "";
	if (m_selectedLine >= 0) {
		if (m_list[m_selectedLine] != null) {
			tmpVal = m_list[m_selectedLine]->getNameFile();
		}
	}
	return tmpVal;
}

// select the specific file
void ewol::widget::ListFileSystem::setSelect(const etk::String& _data) {
	// remove selected line
	m_selectedLine = -1;
	// search the coresponding file :
	for (size_t iii=0; iii<m_list.size(); ++iii) {
		if (m_list[iii] != null) {
			if (m_list[iii]->getNameFile() == _data) {
				// we find the line :
				m_selectedLine = iii;
				break;
			}
		}
	}
	markToRedraw();
}

ivec2 ewol::widget::ListFileSystem::getMatrixSize() const {
	int32_t offset = 0;
	if (*propertyShowFolder == true) {
		if (propertyPath.get() == "/") {
			offset = 1;
		} else {
			offset = 2;
		}
	}
	return ivec2(1, m_list.size() + offset);
}

fluorine::Variant ewol::widget::ListFileSystem::getData(int32_t _role, const ivec2& _pos) {
	switch (_role) {
		case ListRole::Text:
			{
				int32_t offset = 0;
				if (*propertyShowFolder == true) {
					if (*propertyPath == "/") {
						offset = 1;
					} else {
						offset = 2;
					}
					if (_pos.y() == 0) {
						return ".";
					} else if (    _pos.y() == 1
					            && propertyPath.get() != "/") {
						return "..";
					}
				}
				if(    _pos.y()-offset >= 0
				    && _pos.y()-offset < (int32_t)m_list.size()
				    && m_list[_pos.y()-offset] != null) {
					EWOL_VERBOSE("get filename for : '" << *m_list[_pos.y()-offset] << ":'" << m_list[_pos.y()-offset]->getNameFile() << "'");
					return m_list[_pos.y()-offset]->getNameFile();
				}
			}
			return "<<<ERROR>>>";
		case ListRole::FgColor:
			return m_colorProperty->get(m_colorIdText);
		case ListRole::BgColor:
			if (m_selectedLine == _pos.y()) {
				return m_colorProperty->get(m_colorIdBackgroundSelected);
			}
			if (_pos.y() % 2) {
				return m_colorProperty->get(m_colorIdBackground1);
			}
			return m_colorProperty->get(m_colorIdBackground2);
	}
	return fluorine::Variant();
}

bool ewol::widget::ListFileSystem::onItemEvent(const ewol::event::Input& _event,
                                               const ivec2& _pos,
                                               const vec2& _mousePosition) {
	int32_t offset = 0;
	if (*propertyShowFolder == true) {
		if (*propertyPath == "/") {
			offset = 1;
		} else {
			offset = 2;
		}
	}
	if (_event.getStatus() == gale::key::status::pressSingle) {
		EWOL_VERBOSE("Event on List : IdInput=" << _event.getId() << " _pos=" << _pos );
		if (1 == _event.getId()) {
			int32_t previousRaw = m_selectedLine;
			if (_pos.y() > (int32_t)m_list.size()+offset ) {
				m_selectedLine = -1;
			} else {
				m_selectedLine = _pos.y();
			}
			if (previousRaw != m_selectedLine) {
				if(    *propertyShowFolder == true
				    && m_selectedLine == 0) {
					// "." folder
					signalFolderSelect.emit(".");
				} else if (    *propertyShowFolder == true
				            && m_selectedLine == 1) {
					// ".." folder
					signalFolderSelect.emit("..");
				} else if(    m_selectedLine-offset  >= 0
				           && m_selectedLine-offset < (int32_t)m_list.size()
				           && null != m_list[m_selectedLine-offset] ) {
					// generate event extern : 
					switch(m_list[m_selectedLine-offset]->getNodeType()) {
						case etk::typeNode_file :
							signalFileSelect.emit(m_list[m_selectedLine-offset]->getNameFile());
							break;
						case etk::typeNode_folder :
							signalFolderSelect.emit(m_list[m_selectedLine-offset]->getNameFile());
							break;
						default:
							EWOL_ERROR("Can not generate event on an unknow type");
							break;
					}
				}
			} else {
				if(    *propertyShowFolder == true
				    && m_selectedLine == 0) {
					// "." folder
					signalFolderValidate.emit(".");
				} else if (    *propertyShowFolder == true
				            && m_selectedLine == 1) {
					// ".." folder
					signalFolderValidate.emit("..");
				} else if(    m_selectedLine-offset >= 0
				           && m_selectedLine-offset < (int32_t)m_list.size()
				           && null != m_list[m_selectedLine-offset] ) {
					switch(m_list[m_selectedLine-offset]->getNodeType()) {
						case etk::typeNode_file :
							signalFileValidate.emit(m_list[m_selectedLine-offset]->getNameFile());
							break;
						case etk::typeNode_folder :
							signalFolderValidate.emit(m_list[m_selectedLine-offset]->getNameFile());
							break;
						default:
							EWOL_ERROR("Can not generate event on an unknow type");
							break;
					}
				}
			}
			// need to regenerate the display of the list : 
			markToRedraw();
			return true;
		}
	}
	return false;
}

void ewol::widget::ListFileSystem::onChangePropertyPath() {
	regenerateView();
}

void ewol::widget::ListFileSystem::onChangePropertyFile() {
	setSelect(propertyFile);
}

void ewol::widget::ListFileSystem::onChangePropertyShowFile() {
	regenerateView();
}

void ewol::widget::ListFileSystem::onChangePropertyShowFolder() {
	regenerateView();
}

void ewol::widget::ListFileSystem::onChangePropertyShowHidden() {
	regenerateView();
}

void ewol::widget::ListFileSystem::onChangePropertyFilter() {
	regenerateView();
}


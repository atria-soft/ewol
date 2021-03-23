/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/ListFileSystem.hpp>
#include <etk/tool.hpp>
#include <etk/path/fileSystem.hpp>
#include <etk/algorithm.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ListFileSystem);

ewol::widget::ListFileSystem::ListFileSystem() :
  signalFileSelect(this, "file-select", ""),
  signalFileValidate(this, "file-validate", ""),
  signalFolderSelect(this, "folder-select", ""),
  signalFolderValidate(this, "folder-validate", ""),
  propertyPath(this, "path",
                     etk::Path("/"),
                     "Path to display",
                     ewol::widget::ListFileSystem::onChangePropertyPath),
  propertyFile(this, "select",
                     etk::Path(),
                     "selection af a specific file",
                     ewol::widget::ListFileSystem::onChangePropertyFile),
  propertyShowFile(this, "show-file",
                         true,
                         "display files",
                         ewol::widget::ListFileSystem::onChangePropertyShowFile),
  propertyShowFolder(this, "show-folder",
                           true,
                           "display folders",
                           ewol::widget::ListFileSystem::onChangePropertyShowFolder),
  propertyShowHidden(this, "show-hidden",
                           true,
                           "Show the hidden element (file, folder, ...)",
                           ewol::widget::ListFileSystem::onChangePropertyShowHidden),
  propertyFilter(this, "filter",
                       "",
                       "regex to filter files ...",
                       ewol::widget::ListFileSystem::onChangePropertyFilter),
  this.selectedLine(-1) {
	addObjectType("ewol::widget::ListFileSystem");
	#if defined(__TARGET_OS__Windows)
		propertyPath.setDirectCheck("c:/");
	#endif
	this.colorProperty = ewol::resource::ColorFile::create("THEME_COLOR:///ListFileSystem.json?lib=ewol");
	if (this.colorProperty != null) {
		this.colorIdText = this.colorProperty.request("text");
		this.colorIdBackground1 = this.colorProperty.request("background1");
		this.colorIdBackground2 = this.colorProperty.request("background2");
		this.colorIdBackgroundSelected = this.colorProperty.request("selected");
	}
	setMouseLimit(2);
}

ewol::widget::ListFileSystem::~ListFileSystem() {
	clearList();
}

void ewol::widget::ListFileSystem::clearList() {
	this.list.clear();
}

etk::Color<> ewol::widget::ListFileSystem::getBasicBG() {
	return this.colorProperty.get(this.colorIdBackground1);
}

static boolean localSort( etk::Path _left,  etk::Path _right) {
	return _left.getString().toUpper() <= _right.getString().toUpper();
}

void ewol::widget::ListFileSystem::regenerateView() {
	clearList();
	this.selectedLine = -1;
	this.list.clear();
	this.originScrooled.setValue(0,0);
	uint flags = 0;
	if (*propertyShowHidden == true) {
		flags |= etk::path::LIST_HIDDEN;
	}
	if (*propertyShowFolder == true) {
		flags |= etk::path::LIST_FOLDER;
	}
	if (*propertyShowFile == true) {
		flags |= etk::path::LIST_FILE;
	}
	this.list = etk::path::list(*propertyPath, flags);
	Log.error("Lsit of element: " + this.list.size() );
	// Sort the list:
	etk::algorithm::quickSort(this.list, localSort);
	// request a redraw ...
	markToRedraw();
}

etk::Path ewol::widget::ListFileSystem::getSelect()  {
	String tmpVal = "";
	if (this.selectedLine >= 0) {
		tmpVal = this.list[this.selectedLine].getFileName();
	}
	return tmpVal;
}

// select the specific file
void ewol::widget::ListFileSystem::setSelect( etk::Path _data) {
	// remove selected line
	this.selectedLine = -1;
	// search the coresponding file :
	for (int iii=0; iii<this.list.size(); ++iii) {
		if (this.list[iii] == _data) {
			// we find the line :
			this.selectedLine = iii;
			break;
		}
	}
	markToRedraw();
}

Vector2i ewol::widget::ListFileSystem::getMatrixSize()  {
	int offset = 0;
	if (*propertyShowFolder == true) {
		if (propertyPath.get() == "/") {
			offset = 1;
		} else {
			offset = 2;
		}
	}
	return Vector2i(1, this.list.size() + offset);
}

fluorine::Variant ewol::widget::ListFileSystem::getData(int _role,  Vector2i _pos) {
	switch (_role) {
		case ListRole::Text:
			{
				int offset = 0;
				if (*propertyShowFolder == true) {
					if (*propertyPath == "/") {
						offset = 1;
					} else {
						offset = 2;
					}
					if (_pos.y() == 0) {
						return ".";
					} else if (    _pos.y() == 1
					            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM propertyPath.get() != "/") {
						return "..";
					}
				}
				if(    _pos.y()-offset >= 0
				    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _pos.y()-offset < (int)this.list.size()) {
					Log.verbose("get filename for : '" + this.list[_pos.y()-offset] + ":'" + this.list[_pos.y()-offset].getFileName() + "'");
					return this.list[_pos.y()-offset].getFileName();
				}
			}
			return "+<ERROR>>>";
		case ListRole::FgColor:
			return this.colorProperty.get(this.colorIdText);
		case ListRole::BgColor:
			if (this.selectedLine == _pos.y()) {
				return this.colorProperty.get(this.colorIdBackgroundSelected);
			}
			if (_pos.y() % 2) {
				return this.colorProperty.get(this.colorIdBackground1);
			}
			return this.colorProperty.get(this.colorIdBackground2);
	}
	return fluorine::Variant();
}

boolean ewol::widget::ListFileSystem::onItemEvent( ewol::event::Input _event,
                                                Vector2i _pos,
                                                Vector2f _mousePosition) {
	int offset = 0;
	if (*propertyShowFolder == true) {
		if (*propertyPath == "/") {
			offset = 1;
		} else {
			offset = 2;
		}
	}
	if (    _event.getStatus() == KeyStatus::pressSingle
	     || _event.getStatus() == KeyStatus::pressDouble) {
		Log.verbose("Event on List : IdInput=" + _event.getId() + " _pos=" + _pos );
		if (1 == _event.getId()) {
			if (_pos.y() > (int)this.list.size()+offset ) {
				this.selectedLine = -1;
			} else {
				this.selectedLine = _pos.y();
			}
			if(    *propertyShowFolder == true
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.selectedLine == 0) {
				// "." folder
				if (_event.getStatus() == KeyStatus::pressSingle) {
					signalFolderSelect.emit(*propertyPath);
				} else {
					signalFolderValidate.emit(*propertyPath);
				}
			} else if (    *propertyShowFolder == true
			            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.selectedLine == 1) {
				// ".." folder
				if (_event.getStatus() == KeyStatus::pressSingle) {
					signalFolderSelect.emit(propertyPath.getParent());
				} else {
					signalFolderValidate.emit(propertyPath.getParent());
				}
			} else if(    this.selectedLine-offset  >= 0
			           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.selectedLine-offset < (int)this.list.size() ) {
				// generate event extern:
				if(etk::path::isDirectory(this.list[this.selectedLine-offset])) {
					if (_event.getStatus() == KeyStatus::pressSingle) {
						signalFolderSelect.emit(this.list[this.selectedLine-offset]);
					} else {
						signalFolderValidate.emit(this.list[this.selectedLine-offset]);
					}
				} else {
					if (_event.getStatus() == KeyStatus::pressSingle) {
						signalFileSelect.emit(this.list[this.selectedLine-offset]);
					} else {
						signalFileValidate.emit(this.list[this.selectedLine-offset]);
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
	Log.warning("Change Path: " + *propertyPath + " selected File=" + *propertyFile);;
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


/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include "BasicTree.hpp"
#include <etk/tool.hpp>
#include <etk/os/FSNode.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(appl::widget::BasicTree);

appl::widget::BasicTree::BasicTree() {
	addObjectType("appl::widget::BasicTree");
	setMouseLimit(1);
	m_tree = NodeElement::create("root");
	for (size_t iii=0; iii<10; ++iii) {
		auto elem_iii = NodeElement::create("elem_" + etk::toString(iii));
		m_tree->addChild(elem_iii);
		for (size_t jjj=0; jjj<iii; ++jjj) {
			auto elem_iii_jjj = NodeElement::create("elem_" + etk::toString(iii) + "____" + etk::toString(jjj));
			elem_iii->addChild(elem_iii_jjj);
		}
	}
	m_flatTree.setRoot(m_tree);
}

appl::widget::BasicTree::~BasicTree() {
	
}

etk::Color<> appl::widget::BasicTree::getBasicBG() {
	return etk::Color<>(0xAF,0xAF,0xAF,0xFF);
}


ivec2 appl::widget::BasicTree::getMatrixSize() const {
	return ivec2(4, m_flatTree.size());
}

fluorine::Variant appl::widget::BasicTree::getData(int32_t _role, const ivec2& _pos) {
	auto elem = m_flatTree[_pos.y()];
	etk::String value = elem->getData();
	switch (_role) {
		case ewol::widget::ListRole::Text:
			if (_pos.x() == 0) {
				return value;
			}
			if (_pos.x() == 1) {
				//return etk::toString(countToRoot);
			}
			return "value: " + etk::toString(_pos);
		case ewol::widget::ListRole::DistanceToRoot:
			return uint_t(elem->countToRoot());
		case ewol::widget::ListRole::Icon:
			if (elem->countToRoot() == 0) {
				return "{ewol}THEME:GUI:Home.svg";
			}
			if (elem->countToRoot() == 1) {
				return "{ewol}THEME:GUI:Folder.svg";
			}
			if (elem->countToRoot() == 2) {
				return "{ewol}THEME:GUI:File.svg";
			}
			return "";
		case ewol::widget::ListRole::FgColor:
			return etk::Color<>(0,0,0,0xFF);
		case ewol::widget::ListRole::BgColor:
			return fluorine::Variant();
	}
	return fluorine::Variant();
}

bool appl::widget::BasicTree::onItemEvent(int32_t _IdInput,
                                               enum gale::key::status _typeEvent,
                                               const ivec2& _pos,
                                               const vec2& _mousePosition) {
	/*
	int32_t offset = 0;
	if (*propertyShowFolder == true) {
		if (*propertyPath == "/") {
			offset = 1;
		} else {
			offset = 2;
		}
	}
	if (_typeEvent == gale::key::status::pressSingle) {
		EWOL_VERBOSE("Event on List : IdInput=" << _IdInput << " _pos=" << _pos );
		if (1 == _IdInput) {
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
	*/
	return false;
}

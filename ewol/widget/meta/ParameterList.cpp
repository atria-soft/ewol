/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/meta/ParameterList.hpp>
#include <etk/tool.hpp>

#include <ewol/widget/List.hpp>

#include <ewol/compositing/Drawing.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/widget/Manager.hpp>
#include <etranslate/etranslate.hpp>

ewol::widget::ParameterList::ParameterList() :
  signalSelect(this, "select", "") {
	addObjectType("ewol::widget::ParameterList");
	
	m_idSelected = -1;
	m_paddingSizeX = 2;
	#ifdef __TARGET_OS__Android
		m_paddingSizeY = 10;
	#else
		m_paddingSizeY = 2;
	#endif
}

void ewol::widget::ParameterList::init() {
	ewol::widget::WidgetScrolled::init();
	propertyCanFocus.set(true);
}

ewol::widget::ParameterList::~ParameterList() {
	//clean all the object
	m_listOObject.clear();
	menuClear();
}

void ewol::widget::ParameterList::calculateMinMaxSize() {
	/*int32_t fontId = getDefaultFontId();
	int32_t minWidth = ewol::getWidth(fontId, m_label);
	int32_t minHeight = ewol::getHeight(fontId);
	m_minSize.x = 3+minWidth;
	m_minSize.y = 3+minHeight;
	*/
	m_minSize.setValue(150, 150);
}

void ewol::widget::ParameterList::addOObject(const ememory::SharedPtr<ewol::Compositing>& _newObject, int32_t _pos) {
	if (_newObject == nullptr) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (_pos < 0 || (size_t)_pos >= m_listOObject.size() ) {
		m_listOObject.push_back(_newObject);
	} else {
		m_listOObject.insert(m_listOObject.begin()+_pos, _newObject);
	}
}

void ewol::widget::ParameterList::clearOObjectList() {
	m_listOObject.clear();
}

void ewol::widget::ParameterList::onDraw() {
	for (auto &it : m_listOObject) {
		if (it != nullptr) {
			it->draw();
		}
	}
	WidgetScrolled::onDraw();
}

void ewol::widget::ParameterList::onRegenerateDisplay() {
	if (needRedraw() == true) {
		// clean the object list ...
		clearOObjectList();
		//EWOL_DEBUG("OnRegenerateDisplay(" << m_size.x << "," << m_size.y << ")");
		
		int32_t tmpOriginX = 0;
		int32_t tmpOriginY = 0;
		/*
		if (true == m_userFill.x) {
			tmpOriginX = 0;
		}
		if (true == m_userFill.y) {
			tmpOriginY = 0;
		}*/
		tmpOriginX += m_paddingSizeX;
		tmpOriginY += m_paddingSizeY;
	
		/*
		int32_t fontId = getDefaultFontId();
		//int32_t minWidth = ewol::getWidth(fontId, m_label);
		int32_t minHeight = ewol::getHeight(fontId);
		*/
		// TODO : Rework this ...
		int32_t minHeight=20;
	
		//uint32_t nbColomn = getNuberOfColomn();
		int32_t nbRaw    = m_list.size();
		// For the scrooling windows
		m_maxSize.setValue(m_size.x(),
		                   (minHeight + 2*m_paddingSizeY) * nbRaw );
		
		
		std::vector<int32_t> listSizeColomn;
		
		// set background color :
		ememory::SharedPtr<ewol::compositing::Drawing> tmpDraw = ememory::makeShared<ewol::compositing::Drawing>();
		if (tmpDraw == nullptr) {
			return;
		}
		tmpDraw->setColor(etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF));
		tmpDraw->setPos(vec3(0,0,0) );
		tmpDraw->rectangleWidth(vec3(m_size.x(), m_size.y(), 0) );
		
		uint32_t displayableRaw = m_size.y() / (minHeight + 2*m_paddingSizeY) +2;
		
		int32_t startRaw = m_originScrooled.y() / (minHeight + 2*m_paddingSizeY);
		
		if (startRaw >= nbRaw-1 ) {
			startRaw = nbRaw - 1;
		}
		if (startRaw<0) {
			startRaw = 0;
		}
		// calculate the real position ...
		tmpOriginY = m_size.y() - (-m_originScrooled.y() + (startRaw+1)*(minHeight + 2*m_paddingSizeY));
		
		for (int32_t iii=startRaw; iii<nbRaw && iii<(int32_t)(startRaw+displayableRaw); iii++) {
			std::string myTextToWrite = "???";
			etk::Color<> fg(0x00, 0x00, 0x00, 0xFF);
			if (m_list[iii] != nullptr) {
				myTextToWrite = TRANSLATE(m_list[iii]->m_label);
			}
			
			ememory::SharedPtr<ewol::compositing::Text> tmpText = ememory::makeShared<ewol::compositing::Text>();
			
			vec3 textPos;
			textPos.setX((int32_t)tmpOriginX);
			if (m_list[iii]->m_group == false) {
				textPos.setX(textPos.x() + minHeight);
			}
			textPos.setY((int32_t)(tmpOriginY + m_paddingSizeY));
			tmpText->setPos(textPos);
			tmpText->print(myTextToWrite);
			
			addOObject(tmpText);
			tmpOriginY -= minHeight + 2* m_paddingSizeY;
		}
		addOObject(tmpDraw, 0);
		
		// call the herited class...
		ewol::widget::WidgetScrolled::onRegenerateDisplay();
	}
}


bool ewol::widget::ParameterList::onEventInput(const ewol::event::Input& _event) {
	if (true == WidgetScrolled::onEventInput(_event)) {
		keepFocus();
		// nothing to do ... done on upper widet ...
		return true;
	}
	if (_event.getId() == 1 && _event.getStatus() == gale::key::status::pressSingle) {
		vec2 relativePos = relativePosition(_event.getPos());
		// corection for the openGl abstraction
		relativePos.setY(m_size.y() - relativePos.y());
		// TODO : Rework this ...
		/*
		int32_t fontId = getDefaultFontId();
		//int32_t minWidth = ewol::getWidth(fontId, m_label.c_str());
		int32_t minHeight = ewol::getHeight(fontId);
		*/
		int32_t minHeight = 20;
		int32_t rawID = (relativePos.y()+m_originScrooled.y()) / (minHeight + 2*m_paddingSizeY);
		// generate an event on a rawId if the element request change and Select it ...
		if (rawID >= 0 && (size_t)rawID < m_list.size()) {
			if (m_list[rawID]!=nullptr) {
				if (m_list[rawID]->m_refId >= 0) {
					signalSelect.emit(m_list[rawID]->m_refId);
					m_idSelected = rawID;
					markToRedraw();
					return true;
				}
			}
		}
	}
	return false;
}

void ewol::widget::ParameterList::onGetFocus() {
	EWOL_DEBUG("Ewol::List get focus");
}

void ewol::widget::ParameterList::onLostFocus() {
	EWOL_DEBUG("Ewol::List Lost focus");
}

void ewol::widget::ParameterList::menuAdd(std::string& _label, int32_t _refId, std::string& _image) {
	ememory::SharedPtr<ewol::widget::elementPL> tmpEmement = ememory::makeShared<widget::elementPL>(_label, _refId, _image, false);
	if (tmpEmement == nullptr) {
		EWOL_ERROR("Can not allocacte menu parameter");
		return;
	}
	m_list.push_back(tmpEmement);
	if (m_idSelected == -1 && _label != "---" && _refId>0) {
		m_idSelected = m_list.size()-1;
	}
	markToRedraw();
}

void ewol::widget::ParameterList::menuAddGroup(std::string& _label) {
	std::string image = "";
	ememory::SharedPtr<ewol::widget::elementPL> tmpEmement = ememory::makeShared<widget::elementPL>(_label, -1, image, true);
	if (tmpEmement == nullptr) {
		EWOL_ERROR("Can not allocacte menu parameter");
		return;
	}
	m_list.push_back(tmpEmement);
	markToRedraw();
}

void ewol::widget::ParameterList::menuClear() {
	m_idSelected = -1;
	m_list.clear();
}

void ewol::widget::ParameterList::menuSeparator() {
	if (m_list.size()>0) {
		std::string label = "";
		std::string image = "";
		menuAdd(label, -1, image);
	}
}


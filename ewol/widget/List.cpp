/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/List.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Text.h>
#include <ewol/widget/Manager.h>

ewol::widget::List::List() {
	addObjectType("ewol::widget::List");
	m_paddingSizeX = 2;
	#ifdef __TARGET_OS__Android
		m_paddingSizeY = 10;
	#else
		m_paddingSizeY = 2;
	#endif
	m_nbVisibleRaw = 0;
	propertyCanFocus.setDirectCheck(true);
}

ewol::widget::List::~List() {
	//clean all the object
	for (size_t iii=0; iii<m_listOObject.size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = nullptr;
	}
	m_listOObject.clear();
}

void ewol::widget::List::setRawVisible(int32_t _id) {
	EWOL_DEBUG("Set Raw visible : " << _id);
	if (_id<0) {
		return;
	}
	if (_id == m_displayStartRaw) {
		// nothing to do ...
		return;
	}
	if (_id < m_displayStartRaw) {
		m_displayStartRaw = _id-2;
	} else {
		if (m_displayStartRaw + m_nbVisibleRaw < _id) {
			m_displayStartRaw = _id - m_nbVisibleRaw + 2;
		}
	}
	if (m_displayStartRaw > (int32_t)getNuberOfRaw()) {
		m_displayStartRaw = getNuberOfRaw()-2;
	}
	if (m_displayStartRaw<0) {
		m_displayStartRaw = 0;
	}
	EWOL_DEBUG("Set start raw : " << m_displayStartRaw);
	markToRedraw();
}

void ewol::widget::List::calculateMinMaxSize() {
	/*int32_t fontId = getDefaultFontId();
	int32_t minWidth = ewol::getWidth(fontId, m_label);
	int32_t minHeight = ewol::getHeight(fontId);
	m_minSize.x = 3+minWidth;
	m_minSize.y = 3+minHeight;
	*/
	m_minSize.setValue(200, 150);
}

void ewol::widget::List::addOObject(ewol::Compositing* _newObject, int32_t _pos) {
	if (nullptr == _newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (_pos < 0 || (size_t)_pos >= m_listOObject.size() ) {
		m_listOObject.push_back(_newObject);
	} else {
		m_listOObject.insert(m_listOObject.begin()+_pos, _newObject);
	}
}

void ewol::widget::List::clearOObjectList() {
	for (size_t iii=0; iii<m_listOObject.size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = nullptr;
	}
	m_listOObject.clear();
}

void ewol::widget::List::onDraw() {
	for (size_t iii=0; iii<m_listOObject.size(); iii++) {
		if (nullptr != m_listOObject[iii]) {
			m_listOObject[iii]->draw();
		}
	}
	WidgetScrolled::onDraw();
}

void ewol::widget::List::onRegenerateDisplay() {
	if (true == needRedraw()) {
		
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
		// TODO : remove this ...
		int32_t minHeight = 25;
	
		uint32_t nbColomn = getNuberOfColomn();
		int32_t nbRaw    = getNuberOfRaw();
		// For the scrooling windows
		m_maxSize = ivec2(m_size.x(),
		                  (minHeight + 2*m_paddingSizeY) * nbRaw );
		
		
		std::vector<int32_t> listSizeColomn;
		
		ewol::compositing::Drawing * BGOObjects = new ewol::compositing::Drawing();
		etk::Color<> basicBG = getBasicBG();
		BGOObjects->setColor(basicBG);
		BGOObjects->setPos(vec3(0, 0, 0) );
		BGOObjects->rectangleWidth(vec3(m_size.x(), m_size.y(), 0) );
		
		int32_t startRaw = m_originScrooled.y() / (minHeight + 2*m_paddingSizeY);
		
		if (startRaw >= nbRaw-1 ) {
			startRaw = nbRaw - 1;
		}
		if (startRaw<0) {
			startRaw = 0;
		}
		// We display only compleate lines ...
		//EWOL_DEBUG("Request drawing list : " << startRaw << "-->" << (startRaw+displayableRaw) << " in " << nbRaw << "raws ; start display : " << m_originScrooled.y << "  == > " << tmpOriginY << " line size=" << minHeight + 2*m_paddingSizeY );
		
		/*clipping_ts drawClipping;
		drawClipping.x = 0;
		drawClipping.y = 0;
		drawClipping.w = m_size.x - (2*m_paddingSizeX);
		drawClipping.h = m_size.y;
		*/
		// remove all the positions :
		m_lineSize.clear();
		int32_t displayPositionY = m_size.y();
		int32_t displayPositionX = 0;
		ivec2 tmpRegister(startRaw, displayPositionY);
		// add the default position raw :
		m_lineSize.push_back(tmpRegister);
		
		for (size_t jjj=0; jjj<nbColomn && displayPositionX < m_size.x() ; jjj++) {
			int32_t sizeColom = 0;
			displayPositionY = m_size.y();
			m_nbVisibleRaw = 0;
			for (int32_t iii=startRaw; iii<nbRaw && displayPositionY >= 0; iii++) {
				m_nbVisibleRaw++;
				std::string myTextToWrite;
				etk::Color<> fg;
				etk::Color<> bg;
				getElement(jjj, iii, myTextToWrite, fg, bg);
				
				ewol::compositing::Text * tmpText = new ewol::compositing::Text();
				if (nullptr != tmpText) {
					// get font size : 
					int32_t tmpFontHeight = tmpText->calculateSize(char32_t('A')).y();
					displayPositionY-=(tmpFontHeight+m_paddingSizeY);
					
					BGOObjects->setColor(bg);
					BGOObjects->setPos(vec3(displayPositionX, displayPositionY, 0) );
					BGOObjects->rectangleWidth(vec3(m_size.x()-displayPositionX, tmpFontHeight+2*m_paddingSizeY, 0));
					
					// get the maximum size of the colomn :
					vec3 textSize = tmpText->calculateSize(myTextToWrite);
					sizeColom = std::max(sizeColom, (int32_t)textSize.x());
					
					tmpText->setColor(fg);
					tmpText->setPos(vec3(tmpOriginX + displayPositionX, displayPositionY, 0) );
					tmpText->print(myTextToWrite);
					addOObject(tmpText);
					// madding move ...
					displayPositionY -= m_paddingSizeY;
					
					// add the raw position to remember it ...
					tmpRegister.setX(tmpRegister.x()+1);
					tmpRegister.setY(displayPositionY);
					m_lineSize.push_back(tmpRegister);
					//EWOL_DEBUG("List indexation:" << tmpRegister);
				}
			}
			displayPositionX += sizeColom;
			tmpOriginX += m_paddingSizeX*2*2;
		}
		//m_lineSize.push_back(tmpOriginY);
		addOObject(BGOObjects, 0);
		
		// call the herited class...
		WidgetScrolled::onRegenerateDisplay();
	}
}

bool ewol::widget::List::onEventInput(const ewol::event::Input& _event) {
	vec2 relativePos = relativePosition(_event.getPos());
	
	if (true == WidgetScrolled::onEventInput(_event)) {
		keepFocus();
		// nothing to do ... done on upper widet ...
		return true;
	}
	// parse all the loged row position to find the good one...
	int32_t rawID = -1;
	for (size_t iii=0; iii<m_lineSize.size()-1; iii++) {
		if(    relativePos.y()<m_lineSize[iii].y()
		    && relativePos.y() >= m_lineSize[iii+1].y() ) {
			// we find the raw :
			rawID = m_lineSize[iii].x();
			break;
		}
	}
	
	//EWOL_DEBUG("List event : idInput=" << IdInput << " typeEvent=" << typeEvent << "  raw=" << rawID << " pos=" << pos << "");
	bool isUsed = onItemEvent(_event.getId(), _event.getStatus(), 0, rawID, _event.getPos().x(), _event.getPos().y());
	if (true == isUsed) {
		// TODO : this generate bugs ... I did not understand why ..
		//ewol::WidgetSharedManager::focusKeep(this);
	}
	return isUsed;
}

void ewol::widget::List::onGetFocus() {
	EWOL_DEBUG("Ewol::List get focus");
}

void ewol::widget::List::onLostFocus() {
	EWOL_DEBUG("Ewol::List Lost focus");
}

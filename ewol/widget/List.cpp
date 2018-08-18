/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/List.hpp>

#include <ewol/compositing/Drawing.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/widget/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::List);

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
	m_limitScrolling = vec2(1, 0.5);
}

ewol::widget::List::~List() {
	//clean all the object
	for (size_t iii=0; iii<m_listOObject.size(); iii++) {
		ETK_DELETE(ewol::Compositing, m_listOObject[iii]);
		m_listOObject[iii] = null;
	}
	m_listOObject.clear();
}
/*
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
	ivec2 matrixSize = getMatrixSize();
	if (m_displayStartRaw > matrixSize.y()) {
		m_displayStartRaw = matrixSize.y()-2;
	}
	if (m_displayStartRaw<0) {
		m_displayStartRaw = 0;
	}
	EWOL_DEBUG("Set start raw : " << m_displayStartRaw);
	markToRedraw();
}
*/

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
	if (_newObject == null) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (    _pos < 0
	     || (size_t)_pos >= m_listOObject.size() ) {
		m_listOObject.pushBack(_newObject);
	} else {
		m_listOObject.insert(m_listOObject.begin()+_pos, _newObject);
	}
}

void ewol::widget::List::clearOObjectList() {
	for (size_t iii=0; iii<m_listOObject.size(); iii++) {
		ETK_DELETE(ewol::Compositing, m_listOObject[iii]);
		m_listOObject[iii] = null;
	}
	m_listOObject.clear();
}

void ewol::widget::List::onDraw() {
	for (size_t iii=0; iii<m_listOObject.size(); iii++) {
		if (m_listOObject[iii] != null) {
			m_listOObject[iii]->draw();
		}
	}
	WidgetScrolled::onDraw();
}

void ewol::widget::List::onRegenerateDisplay() {
	if (needRedraw() == true) {
		// clean the object list ...
		clearOObjectList();
		// -------------------------------------------------------
		// -- Calculate the size of each element
		// -------------------------------------------------------
		ivec2 matrixSize = getMatrixSize();
		m_listSizeX.clear();
		m_listSizeX.resize(matrixSize.x(), 0);
		m_listSizeY.clear();
		m_listSizeY.resize(matrixSize.y(), 0);
		for (int_t yyy=0; yyy<matrixSize.y(); ++yyy) {
			for (int_t xxx=0; xxx<matrixSize.x(); ++xxx) {
				ivec2 pos(xxx, yyy);
				vec2 elementSize = calculateElementSize(pos);
				if (elementSize.x() > m_listSizeX[xxx]) {
					m_listSizeX[xxx] = elementSize.x();
				}
				if (elementSize.y() > m_listSizeY[yyy]) {
					m_listSizeY[yyy] = elementSize.y();
				}
			}
		}
		// -------------------------------------------------------
		// -- Fill property applyence
		// -------------------------------------------------------
		if (propertyFill->x() == true) {
			int32_t fullSize = 0;
			for (auto &size: m_listSizeX) {
				fullSize += size;
			}
			if (fullSize < m_size.x() ) {
				// need to expand all elements:
				int32_t residualAdd = (m_size.x() - fullSize) / matrixSize.x();
				if (residualAdd != 0) {
					for (auto &size: m_listSizeX) {
						size += residualAdd;
					}
				}
			}
		}
		if (propertyFill->y() == true) {
			int32_t fullSize = 0;
			for (auto &size: m_listSizeY) {
				fullSize += size;
			}
			if (fullSize < m_size.y() ) {
				// need to expand all elements:
				int32_t residualAdd = (m_size.y() - fullSize) / matrixSize.y();
				if (residualAdd != 0) {
					for (auto &size: m_listSizeY) {
						size += residualAdd;
					}
				}
			}
		}
		// -------------------------------------------------------
		// -- Calculate the start position size of each element
		// -------------------------------------------------------
		etk::Vector<int32_t> listStartPosX;
		etk::Vector<int32_t> listStartPosY;
		int32_t lastPositionX = 0;
		for (auto &size: m_listSizeX) {
			listStartPosX.pushBack(lastPositionX);
			lastPositionX += size;
		}
		int32_t lastPositionY = 0;
		for (auto &size: m_listSizeY) {
			lastPositionY += size;
			listStartPosY.pushBack(lastPositionY);
		}
		// -------------------------------------------------------
		// -- Update the scroolBar
		// -------------------------------------------------------
		m_maxSize = ivec2(lastPositionX, lastPositionY);
		// -------------------------------------------------------
		// -- Clean the background
		// -------------------------------------------------------
		drawBackground();
		// -------------------------------------------------------
		// -- Draw each element
		// -------------------------------------------------------
		for (int_t yyy=0; yyy<matrixSize.y(); ++yyy) {
			float startYposition = m_size.y() + m_originScrooled.y() - listStartPosY[yyy];
			if (startYposition + m_listSizeY[yyy] < 0) {
				// ==> element out of range ==> nothing to display
				break;
			}
			if (startYposition > m_size.y()) {
				// ==> element out of range ==> nothing to display
				continue;
			}
			for (int_t xxx=0; xxx<matrixSize.x(); ++xxx) {
				float startXposition = -m_originScrooled.x() + listStartPosX[xxx];
				//EWOL_ERROR("display start: " << startXposition);
				if (startXposition + m_listSizeX[xxx] < 0) {
					// ==> element out of range ==> nothing to display
					continue;
				}
				if (startXposition > m_size.x()) {
					// ==> element out of range ==> nothing to display
					break;
				}
				drawElement(ivec2(xxx, yyy),
				            vec2(startXposition, startYposition),
				            vec2(m_listSizeX[xxx], m_listSizeY[yyy]));
			}
		}
		// -------------------------------------------------------
		// -- Draw Scrooling widget
		// -------------------------------------------------------
		WidgetScrolled::onRegenerateDisplay();
	}
}

ivec2 ewol::widget::List::getMatrixSize() const {
	return ivec2(1,0);
}

vec2 ewol::widget::List::calculateElementSize(const ivec2& _pos) {
	ewol::compositing::Text tmpText;
	etk::String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	vec3 textSize = tmpText.calculateSize(myTextToWrite);
	ivec2 count = getMatrixSize();
	return vec2(textSize.x(),
	            textSize.y() + m_paddingSizeY*3
	            );
}

void ewol::widget::List::drawBackground() {
	ewol::compositing::Drawing * BGOObjects = ETK_NEW(ewol::compositing::Drawing);
	if (BGOObjects != null) {
		addOObject(BGOObjects);
		etk::Color<> basicBG = getBasicBG();
		BGOObjects->setColor(basicBG);
		BGOObjects->setPos(vec3(0, 0, 0) );
		BGOObjects->rectangleWidth(m_size);
	}
}

void ewol::widget::List::drawElement(const ivec2& _pos, const vec2& _start, const vec2& _size) {
	etk::String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	etk::Color<> fg = getData(ListRole::FgColor, _pos).getSafeColor();
	auto backgroundVariant = getData(ListRole::BgColor, _pos);
	if (backgroundVariant.isColor() == true) {
		etk::Color<> bg = backgroundVariant.getColor();
		ewol::compositing::Drawing * BGOObjects = ETK_NEW(ewol::compositing::Drawing);
		if (BGOObjects != null) {
			addOObject(BGOObjects);
			BGOObjects->setColor(bg);
			BGOObjects->setPos(vec3(_start.x(), _start.y(), 0) );
			BGOObjects->rectangleWidth(_size);
		}
	}
	if (myTextToWrite != "") {
		ewol::compositing::Text * tmpText = ETK_NEW(ewol::compositing::Text);
		if (tmpText != null) {
			addOObject(tmpText);
			int32_t displayPositionY = _start.y() + m_paddingSizeY;
			tmpText->setColor(fg);
			tmpText->setPos(vec3(_start.x() + m_paddingSizeX, displayPositionY, 0) );
			tmpText->print(myTextToWrite);;
		}
	}
}

bool ewol::widget::List::onEventInput(const ewol::event::Input& _event) {
	vec2 relativePos = relativePosition(_event.getPos());
	if (WidgetScrolled::onEventInput(_event) == true) {
		keepFocus();
		// nothing to do ... done on upper widet ...
		return true;
	}
	relativePos = vec2(relativePos.x(),m_size.y() - relativePos.y()) + m_originScrooled;
	// Find the colomn and the row
	ivec2 pos{-1,-1};
	int32_t offset = 0;
	for (size_t iii=0; iii<m_listSizeY.size()-1; iii++) {
		int32_t previous = offset;
		offset += m_listSizeY[iii];
		if(    relativePos.y() < offset
		    && relativePos.y() >= previous ) {
			pos.setY(iii);
			break;
		}
	}
	offset = 0;
	for (size_t iii=0; iii<m_listSizeX.size()-1; iii++) {
		int32_t previous = offset;
		offset += m_listSizeX[iii];
		if(    relativePos.x() < offset
		    && relativePos.x() >= previous ) {
			pos.setX(iii);
			break;
		}
	}
	bool isUsed = onItemEvent(_event.getId(), _event.getStatus(), pos, _event.getPos());
	if (isUsed == true) {
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

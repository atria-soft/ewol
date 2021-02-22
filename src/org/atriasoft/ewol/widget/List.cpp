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
	m_limitScrolling = Vector2f(1, 0.5);
}


void ewol::widget::List::init() {
	ewol::widget::WidgetScrolled::init();
	addComposeElemnent("drawing", ememory::makeShared<ewol::compositing::Drawing>());
	addComposeElemnent("text", ememory::makeShared<ewol::compositing::Text>());
}


ewol::widget::List::~List() {
}

void ewol::widget::List::addComposeElemnent(const etk::String& _name, const ememory::SharedPtr<ewol::Compositing>& _element) {
	m_compositingElements.set(_name, _element);
	m_listOObject.pushBack(_element);
}

void ewol::widget::List::clearComposeElemnent() {
	for (auto &it: m_compositingElements) {
		it.second->clear();
	}
}

void ewol::widget::List::removeComposeElemnent() {
	m_compositingElements.clear();
}

ememory::SharedPtr<ewol::Compositing> ewol::widget::List::getComposeElemnent(const etk::String& _name) {
	return m_compositingElements[_name];
}
/*
void ewol::widget::List::setRawVisible(int32_t _id) {
	Log.debug("Set Raw visible : " << _id);
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
	Vector2i matrixSize = getMatrixSize();
	if (m_displayStartRaw > matrixSize.y()) {
		m_displayStartRaw = matrixSize.y()-2;
	}
	if (m_displayStartRaw<0) {
		m_displayStartRaw = 0;
	}
	Log.debug("Set start raw : " << m_displayStartRaw);
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
		clearComposeElemnent();
		// -------------------------------------------------------
		// -- Calculate the size of each element
		// -------------------------------------------------------
		Vector2i matrixSize = getMatrixSize();
		m_listSizeX.clear();
		m_listSizeX.resize(matrixSize.x(), 0);
		m_listSizeY.clear();
		m_listSizeY.resize(matrixSize.y(), 0);
		for (int_t yyy=0; yyy<matrixSize.y(); ++yyy) {
			for (int_t xxx=0; xxx<matrixSize.x(); ++xxx) {
				Vector2i pos(xxx, yyy);
				Vector2f elementSize = calculateElementSize(pos);
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
		/*
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
		*/
		// -------------------------------------------------------
		// -- Calculate the start position size of each element
		// -------------------------------------------------------
		List<int32_t> listStartPosX;
		List<int32_t> listStartPosY;
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
		m_maxSize = Vector2i(lastPositionX, lastPositionY);
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
				//Log.error("display start: " << startXposition);
				if (startXposition + m_listSizeX[xxx] < 0) {
					// ==> element out of range ==> nothing to display
					continue;
				}
				if (startXposition > m_size.x()) {
					// ==> element out of range ==> nothing to display
					break;
				}
				drawElement(Vector2i(xxx, yyy),
				            Vector2f(startXposition, startYposition),
				            Vector2f(m_listSizeX[xxx], m_listSizeY[yyy]));
			}
		}
		// -------------------------------------------------------
		// -- Draw Scrooling widget
		// -------------------------------------------------------
		WidgetScrolled::onRegenerateDisplay();
	}
}

Vector2i ewol::widget::List::getMatrixSize() const {
	return Vector2i(1,0);
}

Vector2f ewol::widget::List::calculateElementSize(const Vector2i& _pos) {
	auto tmpText = ememory::staticPointerCast<ewol::compositing::Text>(getComposeElemnent("text"));
	etk::String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	Vector3f textSize = tmpText->calculateSize(myTextToWrite);
	Vector2i count = getMatrixSize();
	return Vector2f(textSize.x(),
	            textSize.y() + m_paddingSizeY*3
	            );
}

void ewol::widget::List::drawBackground() {
	auto BGOObjects = ememory::staticPointerCast<ewol::compositing::Drawing>(getComposeElemnent("drawing"));
	if (BGOObjects != null) {
		etk::Color<> basicBG = getBasicBG();
		BGOObjects->setColor(basicBG);
		BGOObjects->setPos(Vector3f(0, 0, 0) );
		BGOObjects->rectangleWidth(m_size);
	}
}

void ewol::widget::List::drawElement(const Vector2i& _pos, const Vector2f& _start, const Vector2f& _size) {
	etk::String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	etk::Color<> fg = getData(ListRole::FgColor, _pos).getSafeColor();
	auto backgroundVariant = getData(ListRole::BgColor, _pos);
	if (backgroundVariant.isColor() == true) {
		etk::Color<> bg = backgroundVariant.getColor();
		auto BGOObjects = ememory::staticPointerCast<ewol::compositing::Drawing>(getComposeElemnent("drawing"));
		if (BGOObjects != null) {
			BGOObjects->setColor(bg);
			BGOObjects->setPos(Vector3f(_start.x(), _start.y(), 0) );
			BGOObjects->rectangleWidth(_size);
		}
	}
	if (myTextToWrite != "") {
		auto tmpText = ememory::staticPointerCast<ewol::compositing::Text>(getComposeElemnent("text"));
		if (tmpText != null) {
			int32_t displayPositionY = _start.y() + m_paddingSizeY;
			tmpText->setColor(fg);
			tmpText->setPos(Vector3f(_start.x() + m_paddingSizeX, displayPositionY, 0) );
			tmpText->print(myTextToWrite);;
		}
	}
}

bool ewol::widget::List::onEventInput(const ewol::event::Input& _event) {
	Vector2f relativePos = relativePosition(_event.getPos());
	if (WidgetScrolled::onEventInput(_event) == true) {
		keepFocus();
		// nothing to do ... done on upper widet ...
		return true;
	}
	if (m_listSizeY.size() == 0) {
		return false;
	}
	relativePos = Vector2f(relativePos.x(),m_size.y() - relativePos.y()) + m_originScrooled;
	// Find the colomn and the row
	Vector2i pos{0,0};
	float_t offsetY = 0;
	for (size_t iii=0; iii<m_listSizeY.size()-1; iii++) {
		int32_t previous = offsetY;
		offsetY += m_listSizeY[iii];
		if (    relativePos.y() < offsetY
		     && relativePos.y() >= previous ) {
			pos.setY(iii);
			offsetY = previous;
			break;
		}
		if (    iii == m_listSizeY.size()-2
		     && relativePos.y() >= offsetY ) {
			pos.setY(iii+1);
			break;
		}
	}
	float_t offsetX = 0;
	for (size_t iii=0; iii<m_listSizeX.size()-1; iii++) {
		int32_t previous = offsetX;
		offsetX += m_listSizeX[iii];
		if (    relativePos.x() < offsetX
		     && relativePos.x() >= previous ) {
			pos.setX(iii);
			offsetX = previous;
			break;
		}
		if (    iii == m_listSizeX.size()-2
		     && relativePos.x() >= offsetX ) {
			pos.setX(iii+1);
			break;
		}
	}
	Vector2f posInternalMouse = relativePos - Vector2f(offsetX, offsetY);
	bool isUsed = onItemEvent(_event, pos, posInternalMouse);
	if (isUsed == true) {
		// TODO : this generate bugs ... I did not understand why ..
		//ewol::WidgetSharedManager::focusKeep(this);
	}
	return isUsed;
}

void ewol::widget::List::onGetFocus() {
	Log.debug("Ewol::List get focus");
}

void ewol::widget::List::onLostFocus() {
	Log.debug("Ewol::List Lost focus");
}

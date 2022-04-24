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
	this.paddingSizeX = 2;
	#ifdef __TARGET_OS__Android
		this.paddingSizeY = 10;
	#else
		this.paddingSizeY = 2;
	#endif
	this.nbVisibleRaw = 0;
	propertyCanFocus.setDirectCheck(true);
	this.limitScrolling = Vector2f(1, 0.5);
}


void ewol::widget::List::init() {
	ewol::widget::WidgetScrolled::init();
	addComposeElemnent("drawing", ememory::make<ewol::compositing::Drawing>());
	addComposeElemnent("text", ememory::make<ewol::compositing::Text>());
}


ewol::widget::List::~List() {
}

void ewol::widget::List::addComposeElemnent( String _name,  ememory::Ptr<ewol::Compositing> _element) {
	this.compositingElements.set(_name, _element);
	this.listOObject.pushBack(_element);
}

void ewol::widget::List::clearComposeElemnent() {
	for (auto it: this.compositingElements) {
		it.second.clear();
	}
}

void ewol::widget::List::removeComposeElemnent() {
	this.compositingElements.clear();
}

ememory::Ptr<ewol::Compositing> ewol::widget::List::getComposeElemnent( String _name) {
	return this.compositingElements[_name];
}
/*
void ewol::widget::List::setRawVisible(int _id) {
	Log.debug("Set Raw visible : " + _id);
	if (_id<0) {
		return;
	}
	if (_id == this.displayStartRaw) {
		// nothing to do ...
		return;
	}
	if (_id < this.displayStartRaw) {
		this.displayStartRaw = _id-2;
	} else {
		if (this.displayStartRaw + this.nbVisibleRaw < _id) {
			this.displayStartRaw = _id - this.nbVisibleRaw + 2;
		}
	}
	Vector2i matrixSize = getMatrixSize();
	if (this.displayStartRaw > matrixSize.y()) {
		this.displayStartRaw = matrixSize.y()-2;
	}
	if (this.displayStartRaw<0) {
		this.displayStartRaw = 0;
	}
	Log.debug("Set start raw : " + this.displayStartRaw);
	markToRedraw();
}
*/

void ewol::widget::List::calculateMinMaxSize() {
	/*int fontId = getDefaultFontId();
	int minWidth = ewol::getWidth(fontId, this.label);
	int minHeight = ewol::getHeight(fontId);
	this.minSize.x = 3+minWidth;
	this.minSize.y = 3+minHeight;
	*/
	this.minSize.setValue(200, 150);
}

void ewol::widget::List::onDraw() {
	for (int iii=0; iii<this.listOObject.size(); iii++) {
		if (this.listOObject[iii] != null) {
			this.listOObject[iii].draw();
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
		this.listSizeX.clear();
		this.listSizeX.resize(matrixSize.x(), 0);
		this.listSizeY.clear();
		this.listSizeY.resize(matrixSize.y(), 0);
		for (int_t yyy=0; yyy<matrixSize.y(); ++yyy) {
			for (int_t xxx=0; xxx<matrixSize.x(); ++xxx) {
				Vector2i pos(xxx, yyy);
				Vector2f elementSize = calculateElementSize(pos);
				if (elementSize.x() > this.listSizeX[xxx]) {
					this.listSizeX[xxx] = elementSize.x();
				}
				if (elementSize.y() > this.listSizeY[yyy]) {
					this.listSizeY[yyy] = elementSize.y();
				}
			}
		}
		// -------------------------------------------------------
		// -- Fill property applyence
		// -------------------------------------------------------
		if (propertyFill.x() == true) {
			int fullSize = 0;
			for (auto size: this.listSizeX) {
				fullSize += size;
			}
			if (fullSize < this.size.x() ) {
				// need to expand all elements:
				int residualAdd = (this.size.x() - fullSize) / matrixSize.x();
				if (residualAdd != 0) {
					for (auto size: this.listSizeX) {
						size += residualAdd;
					}
				}
			}
		}
		/*
		if (propertyFill.y() == true) {
			int fullSize = 0;
			for (auto size: this.listSizeY) {
				fullSize += size;
			}
			if (fullSize < this.size.y() ) {
				// need to expand all elements:
				int residualAdd = (this.size.y() - fullSize) / matrixSize.y();
				if (residualAdd != 0) {
					for (auto size: this.listSizeY) {
						size += residualAdd;
					}
				}
			}
		}
		*/
		// -------------------------------------------------------
		// -- Calculate the start position size of each element
		// -------------------------------------------------------
		List<int> listStartPosX;
		List<int> listStartPosY;
		int lastPositionX = 0;
		for (auto size: this.listSizeX) {
			listStartPosX.pushBack(lastPositionX);
			lastPositionX += size;
		}
		int lastPositionY = 0;
		for (auto size: this.listSizeY) {
			lastPositionY += size;
			listStartPosY.pushBack(lastPositionY);
		}
		// -------------------------------------------------------
		// -- Update the scroolBar
		// -------------------------------------------------------
		this.maxSize = Vector2i(lastPositionX, lastPositionY);
		// -------------------------------------------------------
		// -- Clean the background
		// -------------------------------------------------------
		drawBackground();
		// -------------------------------------------------------
		// -- Draw each element
		// -------------------------------------------------------
		for (int_t yyy=0; yyy<matrixSize.y(); ++yyy) {
			float startYposition = this.size.y() + this.originScrooled.y() - listStartPosY[yyy];
			if (startYposition + this.listSizeY[yyy] < 0) {
				// ==> element out of range ==> nothing to display
				break;
			}
			if (startYposition > this.size.y()) {
				// ==> element out of range ==> nothing to display
				continue;
			}
			for (int_t xxx=0; xxx<matrixSize.x(); ++xxx) {
				float startXposition = -this.originScrooled.x() + listStartPosX[xxx];
				//Log.error("display start: " + startXposition);
				if (startXposition + this.listSizeX[xxx] < 0) {
					// ==> element out of range ==> nothing to display
					continue;
				}
				if (startXposition > this.size.x()) {
					// ==> element out of range ==> nothing to display
					break;
				}
				drawElement(Vector2i(xxx, yyy),
				            Vector2f(startXposition, startYposition),
				            Vector2f(this.listSizeX[xxx], this.listSizeY[yyy]));
			}
		}
		// -------------------------------------------------------
		// -- Draw Scrooling widget
		// -------------------------------------------------------
		WidgetScrolled::onRegenerateDisplay();
	}
}

Vector2i ewol::widget::List::getMatrixSize()  {
	return Vector2i(1,0);
}

Vector2f ewol::widget::List::calculateElementSize( Vector2i _pos) {
	auto tmpText = ememory::staticPointerCast<ewol::compositing::Text>(getComposeElemnent("text"));
	String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	Vector3f textSize = tmpText.calculateSize(myTextToWrite);
	Vector2i count = getMatrixSize();
	return Vector2f(textSize.x(),
	            textSize.y() + this.paddingSizeY*3
	            );
}

void ewol::widget::List::drawBackground() {
	auto BGOObjects = ememory::staticPointerCast<ewol::compositing::Drawing>(getComposeElemnent("drawing"));
	if (BGOObjects != null) {
		etk::Color<> basicBG = getBasicBG();
		BGOObjects.setColor(basicBG);
		BGOObjects.setPos(Vector3f(0, 0, 0) );
		BGOObjects.rectangleWidth(this.size);
	}
}

void ewol::widget::List::drawElement( Vector2i _pos,  Vector2f _start,  Vector2f _size) {
	String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	etk::Color<> fg = getData(ListRole::FgColor, _pos).getSafeColor();
	auto backgroundVariant = getData(ListRole::BgColor, _pos);
	if (backgroundVariant.isColor() == true) {
		etk::Color<> bg = backgroundVariant.getColor();
		auto BGOObjects = ememory::staticPointerCast<ewol::compositing::Drawing>(getComposeElemnent("drawing"));
		if (BGOObjects != null) {
			BGOObjects.setColor(bg);
			BGOObjects.setPos(Vector3f(_start.x(), _start.y(), 0) );
			BGOObjects.rectangleWidth(_size);
		}
	}
	if (myTextToWrite != "") {
		auto tmpText = ememory::staticPointerCast<ewol::compositing::Text>(getComposeElemnent("text"));
		if (tmpText != null) {
			int displayPositionY = _start.y() + this.paddingSizeY;
			tmpText.setColor(fg);
			tmpText.setPos(Vector3f(_start.x() + this.paddingSizeX, displayPositionY, 0) );
			tmpText.print(myTextToWrite);;
		}
	}
}

boolean ewol::widget::List::onEventInput( ewol::event::Input _event) {
	Vector2f relativePos = relativePosition(_event.getPos());
	if (WidgetScrolled::onEventInput(_event) == true) {
		keepFocus();
		// nothing to do ... done on upper widet ...
		return true;
	}
	if (this.listSizeY.size() == 0) {
		return false;
	}
	relativePos = Vector2f(relativePos.x(),this.size.y() - relativePos.y()) + this.originScrooled;
	// Find the colomn and the row
	Vector2i pos{0,0};
	float_t offsetY = 0;
	for (int iii=0; iii<this.listSizeY.size()-1; iii++) {
		int previous = offsetY;
		offsetY += this.listSizeY[iii];
		if (    relativePos.y() < offsetY
		     && relativePos.y() >= previous ) {
			pos.setY(iii);
			offsetY = previous;
			break;
		}
		if (    iii == this.listSizeY.size()-2
		     && relativePos.y() >= offsetY ) {
			pos.setY(iii+1);
			break;
		}
	}
	float_t offsetX = 0;
	for (int iii=0; iii<this.listSizeX.size()-1; iii++) {
		int previous = offsetX;
		offsetX += this.listSizeX[iii];
		if (    relativePos.x() < offsetX
		     && relativePos.x() >= previous ) {
			pos.setX(iii);
			offsetX = previous;
			break;
		}
		if (    iii == this.listSizeX.size()-2
		     && relativePos.x() >= offsetX ) {
			pos.setX(iii+1);
			break;
		}
	}
	Vector2f posInternalMouse = relativePos - Vector2f(offsetX, offsetY);
	boolean isUsed = onItemEvent(_event, pos, posInternalMouse);
	if (isUsed == true) {
		// TODO : this generate bugs ... I did not understand why ..
		//WidgetManager::focusKeep(this);
	}
	return isUsed;
}

void ewol::widget::List::onGetFocus() {
	Log.debug("Ewol::List get focus");
}

void ewol::widget::List::onLostFocus() {
	Log.debug("Ewol::List Lost focus");
}

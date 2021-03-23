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
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ParameterList);

ewol::widget::ParameterList::ParameterList() :
  signalSelect(this, "select", "") {
	addObjectType("ewol::widget::ParameterList");
	
	this.idSelected = -1;
	this.paddingSizeX = 2;
	#ifdef __TARGET_OS__Android
		this.paddingSizeY = 10;
	#else
		this.paddingSizeY = 2;
	#endif
}

void ewol::widget::ParameterList::init() {
	ewol::widget::WidgetScrolled::init();
	propertyCanFocus.set(true);
}

ewol::widget::ParameterList::~ParameterList() {
	//clean all the object
	this.listOObject.clear();
	menuClear();
}

void ewol::widget::ParameterList::calculateMinMaxSize() {
	/*int fontId = getDefaultFontId();
	int minWidth = ewol::getWidth(fontId, this.label);
	int minHeight = ewol::getHeight(fontId);
	this.minSize.x = 3+minWidth;
	this.minSize.y = 3+minHeight;
	*/
	this.minSize.setValue(150, 150);
}

void ewol::widget::ParameterList::addOObject( ememory::Ptr<ewol::Compositing> _newObject, int _pos) {
	if (_newObject == null) {
		Log.error("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (_pos < 0 || (int)_pos >= this.listOObject.size() ) {
		this.listOObject.pushBack(_newObject);
	} else {
		this.listOObject.insert(this.listOObject.begin()+_pos, _newObject);
	}
}

void ewol::widget::ParameterList::clearOObjectList() {
	this.listOObject.clear();
}

void ewol::widget::ParameterList::onDraw() {
	for (auto it : this.listOObject) {
		if (it != null) {
			it.draw();
		}
	}
	WidgetScrolled::onDraw();
}

void ewol::widget::ParameterList::onRegenerateDisplay() {
	if (needRedraw() == true) {
		// clean the object list ...
		clearOObjectList();
		//Log.debug("OnRegenerateDisplay(" + this.size.x + "," + this.size.y + ")");
		
		int tmpOriginX = 0;
		int tmpOriginY = 0;
		/*
		if (true == this.userFill.x) {
			tmpOriginX = 0;
		}
		if (true == this.userFill.y) {
			tmpOriginY = 0;
		}*/
		tmpOriginX += this.paddingSizeX;
		tmpOriginY += this.paddingSizeY;
	
		/*
		int fontId = getDefaultFontId();
		//int minWidth = ewol::getWidth(fontId, this.label);
		int minHeight = ewol::getHeight(fontId);
		*/
		// TODO : Rework this ...
		int minHeight=20;
	
		//uint nbColomn = getNuberOfColomn();
		int nbRaw    = this.list.size();
		// For the scrooling windows
		this.maxSize.setValue(this.size.x(),
		                   (minHeight + 2*this.paddingSizeY) * nbRaw );
		
		
		List<int> listSizeColomn;
		
		// set background color :
		ememory::Ptr<ewol::compositing::Drawing> tmpDraw = ememory::make<ewol::compositing::Drawing>();
		if (tmpDraw == null) {
			return;
		}
		tmpDraw.setColor(etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF));
		tmpDraw.setPos(Vector3f(0,0,0) );
		tmpDraw.rectangleWidth(Vector3f(this.size.x(), this.size.y(), 0) );
		
		uint displayableRaw = this.size.y() / (minHeight + 2*this.paddingSizeY) +2;
		
		int startRaw = this.originScrooled.y() / (minHeight + 2*this.paddingSizeY);
		
		if (startRaw >= nbRaw-1 ) {
			startRaw = nbRaw - 1;
		}
		if (startRaw<0) {
			startRaw = 0;
		}
		// calculate the real position ...
		tmpOriginY = this.size.y() - (-this.originScrooled.y() + (startRaw+1)*(minHeight + 2*this.paddingSizeY));
		
		for (int iii=startRaw; iii<nbRaw LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM iii<(int)(startRaw+displayableRaw); iii++) {
			String myTextToWrite = "???";
			etk::Color<> fg(0x00, 0x00, 0x00, 0xFF);
			if (this.list[iii] != null) {
				myTextToWrite = TRANSLATE(this.list[iii].this.label);
			}
			
			ememory::Ptr<ewol::compositing::Text> tmpText = ememory::make<ewol::compositing::Text>();
			
			Vector3f textPos;
			textPos.setX((int)tmpOriginX);
			if (this.list[iii].this.group == false) {
				textPos.setX(textPos.x() + minHeight);
			}
			textPos.setY((int)(tmpOriginY + this.paddingSizeY));
			tmpText.setPos(textPos);
			tmpText.print(myTextToWrite);
			
			addOObject(tmpText);
			tmpOriginY -= minHeight + 2* this.paddingSizeY;
		}
		addOObject(tmpDraw, 0);
		
		// call the herited class...
		ewol::widget::WidgetScrolled::onRegenerateDisplay();
	}
}


boolean ewol::widget::ParameterList::onEventInput( ewol::event::Input _event) {
	if (true == WidgetScrolled::onEventInput(_event)) {
		keepFocus();
		// nothing to do ... done on upper widet ...
		return true;
	}
	if (_event.getId() == 1 LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::pressSingle) {
		Vector2f relativePos = relativePosition(_event.getPos());
		// corection for the openGl abstraction
		relativePos.setY(this.size.y() - relativePos.y());
		// TODO : Rework this ...
		/*
		int fontId = getDefaultFontId();
		//int minWidth = ewol::getWidth(fontId, this.label.c_str());
		int minHeight = ewol::getHeight(fontId);
		*/
		int minHeight = 20;
		int rawID = (relativePos.y()+this.originScrooled.y()) / (minHeight + 2*this.paddingSizeY);
		// generate an event on a rawId if the element request change and Select it ...
		if (rawID >= 0 LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (int)rawID < this.list.size()) {
			if (this.list[rawID]!=null) {
				if (this.list[rawID].this.refId >= 0) {
					signalSelect.emit(this.list[rawID].this.refId);
					this.idSelected = rawID;
					markToRedraw();
					return true;
				}
			}
		}
	}
	return false;
}

void ewol::widget::ParameterList::onGetFocus() {
	Log.debug("Ewol::List get focus");
}

void ewol::widget::ParameterList::onLostFocus() {
	Log.debug("Ewol::List Lost focus");
}

void ewol::widget::ParameterList::menuAdd(String _label, int _refId, String _image) {
	ememory::Ptr<ewol::widget::elementPL> tmpEmement = ememory::make<widget::elementPL>(_label, _refId, _image, false);
	if (tmpEmement == null) {
		Log.error("Can not allocacte menu parameter");
		return;
	}
	this.list.pushBack(tmpEmement);
	if (this.idSelected == -1 LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _label != "---" LOMLOMLOMLOMLOM _refId>0) {
		this.idSelected = this.list.size()-1;
	}
	markToRedraw();
}

void ewol::widget::ParameterList::menuAddGroup(String _label) {
	String image = "";
	ememory::Ptr<ewol::widget::elementPL> tmpEmement = ememory::make<widget::elementPL>(_label, -1, image, true);
	if (tmpEmement == null) {
		Log.error("Can not allocacte menu parameter");
		return;
	}
	this.list.pushBack(tmpEmement);
	markToRedraw();
}

void ewol::widget::ParameterList::menuClear() {
	this.idSelected = -1;
	this.list.clear();
}

void ewol::widget::ParameterList::menuSeparator() {
	if (this.list.size()>0) {
		String label = "";
		String image = "";
		menuAdd(label, -1, image);
	}
}


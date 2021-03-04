/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Label.hpp>

#include <ewol/compositing/Text.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/ewol.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Label);

// TODO : Remove the label name in the ructor ...
ewol::widget::Label::Label() :
  signalPressed(this, "pressed", ""),
  propertyAutoTranslate(this, "auto-translate",
                              true,
                              "Translate the String with the marker _{T:xxxxxx}",
                              ewol::widget::Label::onChangePropertyAutoTranslate),
  propertyValue(this, "value",
                      "",
                      "displayed value string",
                      ewol::widget::Label::onChangePropertyValue),
  propertyFontSize(this, "font-size",
                      0,
                      "default font size (0=> system default)",
                      ewol::widget::Label::onChangePropertyFontSize),
  this.value(U""),
  this.colorProperty(null),
  this.colorDefaultFgText(-1),
  this.colorDefaultBgText(-1){
	addObjectType("ewol::widget::Label");
	this.colorProperty = ewol::resource::ColorFile::create(etk::Uri("THEME_COLOR:///Label.json?lib=ewol"));
	if (this.colorProperty != null) {
		this.colorDefaultFgText = this.colorProperty.request("foreground");
		this.colorDefaultBgText = this.colorProperty.request("background");
	}
	setMouseLimit(1);
	propertyCanFocus.setDirectCheck(false);
}

ewol::widget::Label::~Label() {
	
}

void ewol::widget::Label::init() {
	Widget::init();
	// Force update the value of internal display
	onChangePropertyValue();
}


void ewol::widget::Label::calculateMinMaxSize() {
	Vector2f tmpMax = propertyMaxSize.getPixel();
	Vector2f tmpMin = propertyMinSize.getPixel();
	//Log.debug("[" + getId() + "] {" + getObjectType() + "} tmpMax : " + tmpMax);
	if (tmpMax.x() <= 999999) {
		this.text.setTextAlignement(0, tmpMax.x()-4, ewol::compositing::alignLeft);
		//Log.debug("[" + getId() + "] {" + getObjectType() + "}     forcez Alignement ");
	}
	Vector3f minSize = this.text.calculateSizeDecorated(this.value);
	//Log.debug("[" + getId() + "] {" + getObjectType() + "} minSize : " + minSize);
	
	this.minSize.setX(etk::avg(tmpMin.x(), 4 + minSize.x(), tmpMax.x()));
	this.minSize.setY(etk::avg(tmpMin.y(), 4 + minSize.y(), tmpMax.y()));
	Log.verbose("[" + getId() + "] {" + getObjectType() + "} Result min size : " + tmpMin + " < " + this.minSize + " < " << tmpMax);
}

void ewol::widget::Label::onDraw() {
	this.text.draw();
}

void ewol::widget::Label::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	this.text.clear();
	int paddingSize = 2;
	
	Vector2f tmpMax = propertyMaxSize.getPixel();
	// to know the size of one line : 
	Vector3f minSize = this.text.calculateSize(Character('A'));
	
	//minSize.setX(etk::max(minSize.x(), this.minSize.x()));
	//minSize.setY(etk::max(minSize.y(), this.minSize.y()));
	if (tmpMax.x() <= 999999) {
		this.text.setTextAlignement(0, tmpMax.x()-2*paddingSize, ewol::compositing::alignLeft);
	}
	Vector3f curentTextSize = this.text.calculateSizeDecorated(this.value);
	
	Vector2i localSize = this.minSize;
	
	// no change for the text orogin : 
	Vector3f tmpTextOrigin((this.size.x() - this.minSize.x()) / 2.0,
	                   (this.size.y() - this.minSize.y()) / 2.0,
	                   0);
	
	if (propertyFill.x() == true) {
		localSize.setX(this.size.x());
		tmpTextOrigin.setX(0);
	}
	if (propertyFill.y() == true) {
		localSize.setY(this.size.y());
		tmpTextOrigin.setY(this.size.y() - 2*paddingSize - curentTextSize.y());
	}
	tmpTextOrigin += Vector3f(paddingSize, paddingSize, 0);
	localSize -= Vector2f(2*paddingSize,2*paddingSize);
	
	tmpTextOrigin.setY( tmpTextOrigin.y() + (this.minSize.y()-2*paddingSize) - minSize.y());
	
	Vector2f textPos(tmpTextOrigin.x(), tmpTextOrigin.y());
	
	Vector3f drawClippingPos(paddingSize, paddingSize, -0.5);
	Vector3f drawClippingSize((this.size.x() - paddingSize),
	                      (this.size.y() - paddingSize),
	                      1);
	
	// clean the element
	this.text.reset();
	if (propertyFontSize.get() != 0) {
		this.text.setFontSize(propertyFontSize.get());
	}
	if (this.colorProperty != null) {
		this.text.setDefaultColorFg(this.colorProperty.get(this.colorDefaultFgText));
		this.text.setDefaultColorBg(this.colorProperty.get(this.colorDefaultBgText));
	}
	this.text.setPos(tmpTextOrigin);
	Log.verbose("[" + getId() + "] {" + this.value + "} display at pos : " + tmpTextOrigin);
	this.text.setTextAlignement(tmpTextOrigin.x(), tmpTextOrigin.x()+localSize.x(), ewol::compositing::alignLeft);
	this.text.setClipping(drawClippingPos, drawClippingSize);
	this.text.printDecorated(this.value);
}

boolean ewol::widget::Label::onEventInput( ewol::event::Input _event) {
	//Log.debug("Event on Label ...");
	if (_event.getId() == 1) {
		if (KeyStatus::pressSingle == _event.getStatus()) {
			// nothing to do ...
			signalPressed.emit();
			return true;
		}
	}
	return false;
}

boolean ewol::widget::Label::loadXML( exml::Element _node) {
	if (_node.exist() == false) {
		return false;
	}
	Widget::loadXML(_node);
	// get internal data : 
	Log.debug("Load label:" + _node.getText());
	propertyValue.set(_node.getText());
	return true;
}

void ewol::widget::Label::onChangePropertyValue() {
	if (*propertyAutoTranslate == true) {
		this.value = etk::toUString(etranslate::get(*propertyValue));
	} else {
		this.value = etk::toUString(*propertyValue);
	}
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Label::onChangePropertyFontSize() {
	onChangePropertyValue();
}

void ewol::widget::Label::onChangePropertyAutoTranslate() {
	onChangePropertyValue();
}

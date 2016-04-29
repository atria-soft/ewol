/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/Label.h>

#include <ewol/compositing/Text.h>
#include <ewol/widget/Manager.h>
#include <ewol/ewol.h>

// TODO : Remove the label name in the constructor ...
ewol::widget::Label::Label() :
  signalPressed(this, "pressed", ""),
  propertyAutoTranslate(this, "auto-translate",
                              true,
                              "Translate the String with the marker _{T:xxxxxx}",
                              &ewol::widget::Label::onChangePropertyAutoTranslate),
  propertyValue(this, "value",
                      "",
                      "displayed value string",
                      &ewol::widget::Label::onChangePropertyValue),
  m_value(U""),
  m_colorProperty(nullptr),
  m_colorDefaultFgText(-1),
  m_colorDefaultBgText(-1){
	addObjectType("ewol::widget::Label");
	m_colorProperty = ewol::resource::ColorFile::create("{ewol}THEME:COLOR:Label.json");
	if (m_colorProperty != nullptr) {
		m_colorDefaultFgText = m_colorProperty->request("foreground");
		m_colorDefaultBgText = m_colorProperty->request("background");
	}
	setMouseLimit(1);
	propertyCanFocus.setDirectCheck(false);
}

ewol::widget::Label::~Label() {
	
}

void ewol::widget::Label::init() {
	ewol::Widget::init();
	// Force update the value of internal display
	onChangePropertyValue();
}


void ewol::widget::Label::calculateMinMaxSize() {
	vec2 tmpMax = propertyMaxSize->getPixel();
	vec2 tmpMin = propertyMinSize->getPixel();
	//EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "} tmpMax : " << tmpMax);
	if (tmpMax.x() <= 999999) {
		m_text.setTextAlignement(0, tmpMax.x()-4, ewol::compositing::alignLeft);
		//EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "}     forcez Alignement ");
	}
	vec3 minSize = m_text.calculateSizeDecorated(m_value);
	//EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "} minSize : " << minSize);
	
	m_minSize.setX(std::avg(tmpMin.x(), 4 + minSize.x(), tmpMax.x()));
	m_minSize.setY(std::avg(tmpMin.y(), 4 + minSize.y(), tmpMax.y()));
	EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} Result min size : " << tmpMin << " < " << m_minSize << " < " << tmpMax);
}

void ewol::widget::Label::onDraw() {
	m_text.draw();
}

void ewol::widget::Label::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	m_text.clear();
	int32_t paddingSize = 2;
	
	vec2 tmpMax = propertyMaxSize->getPixel();
	// to know the size of one line : 
	vec3 minSize = m_text.calculateSize(char32_t('A'));
	
	//minSize.setX(std::max(minSize.x(), m_minSize.x()));
	//minSize.setY(std::max(minSize.y(), m_minSize.y()));
	if (tmpMax.x() <= 999999) {
		m_text.setTextAlignement(0, tmpMax.x()-2*paddingSize, ewol::compositing::alignLeft);
	}
	vec3 curentTextSize = m_text.calculateSizeDecorated(m_value);
	
	ivec2 localSize = m_minSize;
	
	// no change for the text orogin : 
	vec3 tmpTextOrigin((m_size.x() - m_minSize.x()) / 2.0,
	                   (m_size.y() - m_minSize.y()) / 2.0,
	                   0);
	
	if (propertyFill->x() == true) {
		localSize.setX(m_size.x());
		tmpTextOrigin.setX(0);
	}
	if (propertyFill->y() == true) {
		localSize.setY(m_size.y());
		tmpTextOrigin.setY(m_size.y() - 2*paddingSize - curentTextSize.y());
	}
	tmpTextOrigin += vec3(paddingSize, paddingSize, 0);
	localSize -= vec2(2*paddingSize,2*paddingSize);
	
	tmpTextOrigin.setY( tmpTextOrigin.y() + (m_minSize.y()-2*paddingSize) - minSize.y());
	
	vec2 textPos(tmpTextOrigin.x(), tmpTextOrigin.y());
	
	vec3 drawClippingPos(paddingSize, paddingSize, -0.5);
	vec3 drawClippingSize((m_size.x() - paddingSize),
	                      (m_size.y() - paddingSize),
	                      1);
	
	// clean the element
	m_text.reset();
	if (m_colorProperty != nullptr) {
		m_text.setDefaultColorFg(m_colorProperty->get(m_colorDefaultFgText));
		m_text.setDefaultColorBg(m_colorProperty->get(m_colorDefaultBgText));
	}
	m_text.setPos(tmpTextOrigin);
	EWOL_VERBOSE("[" << getId() << "] {" << m_value << "} display at pos : " << tmpTextOrigin);
	m_text.setTextAlignement(tmpTextOrigin.x(), tmpTextOrigin.x()+localSize.x(), ewol::compositing::alignLeft);
	m_text.setClipping(drawClippingPos, drawClippingSize);
	m_text.printDecorated(m_value);
}

bool ewol::widget::Label::onEventInput(const ewol::event::Input& _event) {
	//EWOL_DEBUG("Event on Label ...");
	if (_event.getId() == 1) {
		if (gale::key::status::pressSingle == _event.getStatus()) {
			// nothing to do ...
			signalPressed.emit();
			return true;
		}
	}
	return false;
}

bool ewol::widget::Label::loadXML(const exml::Element& _node) {
	if (_node.exist() == false) {
		return false;
	}
	ewol::Widget::loadXML(_node);
	// get internal data : 
	EWOL_DEBUG("Load label:" << _node.getText());
	propertyValue.set(_node.getText());
	return true;
}

void ewol::widget::Label::onChangePropertyValue() {
	if (*propertyAutoTranslate == true) {
		m_value = etk::to_u32string(ewol::translate::get(*propertyValue));
	} else {
		m_value = etk::to_u32string(*propertyValue);
	}
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Label::onChangePropertyAutoTranslate() {
	onChangePropertyValue();
}

/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/CheckBox.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventCheckBoxClicked    = "ewol CheckBox Clicked";

#undef __class__
#define __class__	"CheckBox"

static ewol::Widget* Create(void) {
	return new ewol::widget::CheckBox();
}

void ewol::widget::CheckBox::init(ewol::WidgetManager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&Create);
}

ewol::widget::CheckBox::CheckBox(const std::string& _newLabel) {
	addObjectType("ewol::widget::CheckBox");
	m_label = _newLabel;
	addEventId(ewolEventCheckBoxClicked);
	m_textColorFg = etk::color::black;
	m_textColorBg = etk::color::white;
	m_value = false;
	setCanHaveFocus(true);
	setMouseLimit(1);
}


ewol::widget::CheckBox::~CheckBox(void) {
	
}


void ewol::widget::CheckBox::calculateMinMaxSize(void) {
	vec3 minSize = m_oObjectText.calculateSize(m_label);
	float boxSize = etk_max(20, minSize.y()) + 5;
	m_minSize.setX(boxSize+minSize.x());
	m_minSize.setY(etk_max(boxSize, minSize.y())+3);
	markToRedraw();
}


void ewol::widget::CheckBox::setLabel(std::string _newLabel) {
	m_label = _newLabel;
	markToRedraw();
}

void ewol::widget::CheckBox::setValue(bool _val) {
	if (m_value == _val) {
		return;
	}
	m_value = _val;
	markToRedraw();
}

bool ewol::widget::CheckBox::getValue(void) {
	return m_value;
}

void ewol::widget::CheckBox::onDraw(void) {
	m_oObjectDecoration.draw();
	m_oObjectText.draw();
}

void ewol::widget::CheckBox::onRegenerateDisplay(void) {
	if (true == needRedraw()) {
		m_oObjectDecoration.clear();
		m_oObjectText.clear();
		
		vec3 minSize = m_oObjectText.calculateSize(m_label);
		float boxSize = etk_max(20, minSize.y()) + 5;
		//int32_t fontWidth = ewol::getWidth(fontId, m_label.c_str());
		int32_t posy = (m_size.y() - minSize.y() - 6)/2 + 3;
		//int32_t posx = (m_size.x - fontWidth - 6)/2 + 25;
		
		
		vec3 textPos(boxSize+5, posy, 0);
		m_oObjectText.setPos(textPos);
		m_oObjectText.print(m_label);
		
		m_oObjectDecoration.setColor(m_textColorBg);
		m_oObjectDecoration.setPos(vec3(2.5f, 2.5f, 0.0f) );
		m_oObjectDecoration.rectangleWidth(vec3(boxSize, boxSize, 0.0f) );
		if (m_value) {
			m_oObjectDecoration.setColor(m_textColorFg);
			m_oObjectDecoration.setPos(vec3(2.5f, 2.5f, 0.0f) );
			m_oObjectDecoration.setThickness(3);
			m_oObjectDecoration.lineRel(vec3(boxSize, boxSize, 0.0f) );
		}
	}
}

bool ewol::widget::CheckBox::onEventInput(const ewol::EventInput& _event) {
	//EWOL_DEBUG("Event on checkbox ...");
	if (1 == _event.getId()) {
		if (ewol::keyEvent::statusSingle == _event.getStatus()) {
			if(true == m_value) {
				m_value = false;
				generateEventId(ewolEventCheckBoxClicked, "false");
			} else {
				m_value = true;
				generateEventId(ewolEventCheckBoxClicked, "true");
			}
			keepFocus();
			markToRedraw();
			return true;
		}
	}
	return false;
}

bool ewol::widget::CheckBox::onEventEntry(const ewol::EventEntry& _event) {
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    _event.getType() == ewol::keyEvent::keyboardChar
	    && _event.getStatus() == ewol::keyEvent::statusDown
	    && (    _event.getChar() == '\r'
	         || _event.getChar() == ' ')
	       ) {
		if(true == m_value) {
			m_value = false;
			generateEventId(ewolEventCheckBoxClicked, "false");
		} else {
			m_value = true;
			generateEventId(ewolEventCheckBoxClicked, "true");
		}
		markToRedraw();
		return true;
	}
	return false;
}


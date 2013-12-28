/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Label.h>

#include <ewol/compositing/Text.h>
#include <ewol/widget/Manager.h>
#include <ewol/ewol.h>

#undef __class__
#define __class__ "Label"

const char * const ewol::widget::Label::eventPressed = "pressed";

static ewol::Widget* create(void) {
	return new ewol::widget::Label();
}

void ewol::widget::Label::init(ewol::widget::Manager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}

ewol::widget::Label::Label(std::string _newLabel) {
	addObjectType("ewol::widget::Label");
	m_label = _newLabel;
	addEventId(eventPressed);
	setCanHaveFocus(false);
}

void ewol::widget::Label::calculateMinMaxSize(void) {
	vec2 tmpMax = m_userMaxSize.getPixel();
	//EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "} tmpMax : " << tmpMax);
	if (tmpMax.x() <= 999999) {
		m_text.setTextAlignement(0, tmpMax.x()-4, ewol::compositing::Text::alignLeft);
		//EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "}     forcez Alignement ");
	}
	vec3 minSize = m_text.calculateSizeDecorated(m_label);
	//EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "} minSize : " << minSize);
	
	m_minSize.setX(etk_min(4 + minSize.x(), tmpMax.x()));
	m_minSize.setY(etk_min(4 + minSize.y(), tmpMax.y()));
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Result min size : " <<  m_minSize);
}

void ewol::widget::Label::setLabel(const std::string& _newLabel) {
	m_label = _newLabel;
	markToRedraw();
	requestUpdateSize();
}

std::string ewol::widget::Label::getLabel(void) {
	return m_label;
}

void ewol::widget::Label::onDraw(void) {
	m_text.draw();
}

void ewol::widget::Label::onRegenerateDisplay(void) {
	if (true == needRedraw()) {
		m_text.clear();
		int32_t paddingSize = 2;
		
		vec2 tmpMax = m_userMaxSize.getPixel();
		// to know the size of one line : 
		vec3 minSize = m_text.calculateSize(char32_t('A'));
		if (tmpMax.x() <= 999999) {
			m_text.setTextAlignement(0, tmpMax.x()-2*paddingSize, ewol::compositing::Text::alignLeft);
		}
		vec3 curentTextSize = m_text.calculateSizeDecorated(m_label);
		
		ivec2 localSize = m_minSize;
		
		// no change for the text orogin : 
		vec3 tmpTextOrigin((m_size.x() - m_minSize.x()) / 2.0,
		                   (m_size.y() - m_minSize.y()) / 2.0,
		                   0);
		
		if (true == m_userFill.x()) {
			localSize.setX(m_size.x());
			tmpTextOrigin.setX(0);
		}
		if (true == m_userFill.y()) {
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
		m_text.setPos(tmpTextOrigin);
		m_text.setTextAlignement(tmpTextOrigin.x(), tmpTextOrigin.x()+localSize.x(), ewol::compositing::Text::alignLeft);
		m_text.setClipping(drawClippingPos, drawClippingSize);
		m_text.printDecorated(m_label);
	}
}

bool ewol::widget::Label::onEventInput(const ewol::event::Input& _event) {
	//EWOL_DEBUG("Event on Label ...");
	if (1 == _event.getId()) {
		if (ewol::key::statusSingle == _event.getStatus()) {
			// nothing to do ...
			generateEventId(eventPressed);
			return true;
		}
	}
	return false;
}

bool ewol::widget::Label::loadXML(exml::Element* _node) {
	if (NULL == _node) {
		return false;
	}
	ewol::Widget::loadXML(_node);
	// get internal data : 
	EWOL_DEBUG("Load label:" << _node->getText());
	setLabel(_node->getText());
	return true;
}


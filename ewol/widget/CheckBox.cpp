/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Manager.h>

// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_SELECTED  (2)

#undef __class__
#define __class__	"CheckBox"


ewol::widget::CheckBox::CheckBox() :
  signalPressed(*this, "pressed", "CheckBox is pressed"),
  signalDown(*this, "down", "CheckBox is DOWN"),
  signalUp(*this, "up", "CheckBox is UP"),
  signalEnter(*this, "enter", "The cursor enter inside the CheckBox"),
  signalValue(*this, "value", "CheckBox value change"),
  propertyValue(*this, "value", false, "Basic value of the widget"),
  propertyShape(*this, "shape", "", "The display name for config file"),
  m_mouseHover(false),
  m_buttonPressed(false),
  m_selectableAreaPos(0,0),
  m_selectableAreaSize(0,0),
  m_shaperIdSize(-1),
  m_shaperIdSizeInsize(-1) {
	addObjectType("ewol::widget::CheckBox");
	// shaper satatus update:
	CheckStatus();
	// Limit event at 1:
	setMouseLimit(1);
}


void ewol::widget::CheckBox::init(const std::string& _shaperName) {
	ewol::widget::Container2::init();
	propertyCanFocus.set(true);
	propertyShape.set(_shaperName);
	m_shaperIdSize = m_shaper.requestConfig("box-size");
	m_shaperIdSizeInsize = m_shaper.requestConfig("box-inside");
}

ewol::widget::CheckBox::~CheckBox() {
	
}

void ewol::widget::CheckBox::onChangeSize() {
	ewol::Padding padding = m_shaper.getPadding();
	float boxSize = m_shaper.getConfigNumber(m_shaperIdSize);
	padding.setXLeft(padding.xLeft()*2.0f + boxSize);
	ewol::Padding ret = onChangeSizePadded(padding);
	EWOL_DEBUG(" configuring : padding=" << padding << " boxSize=" << boxSize << "");
	m_selectableAreaPos = vec2(ret.xLeft()/*-boxSize*/, ret.yButtom());
	m_selectableAreaSize = m_size - (m_selectableAreaPos + vec2(ret.xRight(), ret.yTop()));
}

void ewol::widget::CheckBox::calculateMinMaxSize() {
	ewol::Padding padding = m_shaper.getPadding();
	float boxSize = m_shaper.getConfigNumber(m_shaperIdSize);
	padding.setXLeft(padding.xLeft()*2.0f + boxSize);
	calculateMinMaxSizePadded(padding);
	if (m_minSize.y() < padding.y()+boxSize) {
		m_minSize.setY(padding.y()+boxSize);
	}
}

void ewol::widget::CheckBox::onDraw() {
	// draw the shaaper (if needed indeed)
	m_shaper.draw();
}

void ewol::widget::CheckBox::onRegenerateDisplay() {
	ewol::widget::Container2::onRegenerateDisplay();
	if (needRedraw() == false) {
		return;
	}
	ewol::Padding padding = m_shaper.getPadding();
	float boxSize = m_shaper.getConfigNumber(m_shaperIdSize);
	float boxInside = m_shaper.getConfigNumber(m_shaperIdSizeInsize);
	m_shaper.clear();
	EWOL_DEBUG(" configuring : boxSize=" << boxSize << " boxInside=" << boxInside << "");
	vec2 origin(m_selectableAreaPos + vec2(0, (m_selectableAreaSize.y() - (boxSize+padding.y()))*0.5f));
	vec2 size = vec2(boxSize+padding.x(), boxSize+padding.y());
	
	vec2 origin2 = m_selectableAreaPos + vec2((boxSize-boxInside)*0.5f, (m_selectableAreaSize.y() - (boxInside+padding.y()))*0.5f);
	vec2 size2 = vec2(boxInside+padding.x(), boxInside+padding.y());
	m_shaper.setShape(vec2ClipInt32(origin),
	                   vec2ClipInt32(size),
	                   vec2ClipInt32(origin2+vec2(padding.xLeft(),padding.yButtom()) ),
	                   vec2ClipInt32(size2-vec2(padding.x(),padding.y()) ));
}

bool ewol::widget::CheckBox::onEventInput(const ewol::event::Input& _event) {
	EWOL_VERBOSE("Event on BT : " << _event);
	
	bool previousHoverState = m_mouseHover;
	if(    gale::key::status_leave == _event.getStatus()
	    || gale::key::status_abort == _event.getStatus()) {
		m_mouseHover = false;
		m_buttonPressed = false;
	} else {
		vec2 relativePos = relativePosition(_event.getPos());
		// prevent error from ouside the button
		if(    relativePos.x() < m_selectableAreaPos.x()
		    || relativePos.y() < m_selectableAreaPos.y()
		    || relativePos.x() > m_selectableAreaPos.x() + m_selectableAreaSize.x()
		    || relativePos.y() > m_selectableAreaPos.y() + m_selectableAreaSize.y() ) {
			m_mouseHover = false;
			m_buttonPressed = false;
		} else {
			m_mouseHover = true;
		}
	}
	bool previousPressed = m_buttonPressed;
	EWOL_VERBOSE("Event on BT ... mouse hover : " << m_mouseHover);
	if (m_mouseHover == true) {
		if (_event.getId() == 1) {
			if(gale::key::status_down == _event.getStatus()) {
				EWOL_VERBOSE(*propertyName << " : Generate event : " << signalDown);
				signalDown.emit();
				m_buttonPressed = true;
				markToRedraw();
			}
			if(gale::key::status_up == _event.getStatus()) {
				EWOL_VERBOSE(*propertyName << " : Generate event : " << signalUp);
				signalUp.emit();
				m_buttonPressed = false;
				markToRedraw();
			}
			if(gale::key::status_single == _event.getStatus()) {
				// inverse value :
				propertyValue.set((*propertyValue)?false:true);
				EWOL_VERBOSE(*propertyName << " : Generate event : " << signalPressed);
				signalPressed.emit();
				EWOL_VERBOSE(*propertyName << " : Generate event : " << signalValue << " val=" << propertyValue );
				signalValue.emit(*propertyValue);
				markToRedraw();
			}
		}
	}
	if(    m_mouseHover != previousHoverState
	    || m_buttonPressed != previousPressed) {
		CheckStatus();
	}
	return m_mouseHover;
}


bool ewol::widget::CheckBox::onEventEntry(const ewol::event::Entry& _event) {
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    _event.getType() == gale::key::keyboard_char
	    && _event.getStatus() == gale::key::status_down
	    && _event.getChar() == '\r') {
		signalEnter.emit();
		return true;
	}
	return false;
}

void ewol::widget::CheckBox::CheckStatus() {
	if (m_shaper.setState(*propertyValue==true?1:0) == true) {
		markToRedraw();
	}
	if (m_buttonPressed == true) {
		changeStatusIn(STATUS_PRESSED);
		return;
	}
	if (m_mouseHover == true) {
		changeStatusIn(STATUS_HOVER);
		return;
	}
	changeStatusIn(STATUS_UP);
}

void ewol::widget::CheckBox::changeStatusIn(int32_t _newStatusId) {
	if (m_shaper.changeStatusIn(_newStatusId) == true) {
		periodicCallEnable();
		markToRedraw();
	}
}


void ewol::widget::CheckBox::periodicCall(const ewol::event::Time& _event) {
	if (m_shaper.periodicCall(_event) == false) {
		periodicCallDisable();
	}
	markToRedraw();
}

void ewol::widget::CheckBox::onPropertyChangeValue(const eproperty::Ref& _paramPointer) {
	ewol::widget::Container2::onPropertyChangeValue(_paramPointer);
	if (_paramPointer == propertyShape) {
		m_shaper.setSource(*propertyShape);
		markToRedraw();
	} else if (_paramPointer == propertyValue) {
		if (*propertyValue == false) {
			m_idWidgetDisplayed = convertId(0);
		} else {
			m_idWidgetDisplayed = convertId(1);
		}
		CheckStatus();
		markToRedraw();
		m_shaper.setActivateState(*propertyValue==true?1:0);
	}
}

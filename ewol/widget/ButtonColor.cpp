/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/widget/ButtonColor.hpp>

#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/meta/ColorChooser.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/ewol.hpp>
#include <ewol/object/Manager.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ButtonColor);

// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)

ewol::widget::ButtonColor::ButtonColor() :
  signalChange(this, "change", "Button color change value"),
  propertyValue(this, "color", etk::color::black, "Current color", &ewol::widget::ButtonColor::onChangePropertyValue),
  propertyShape(this, "shape", "{ewol}THEME:GUI:Button.json", "shape of the widget", &ewol::widget::ButtonColor::onChangePropertyShape),
  m_widgetContextMenu(null) {
	addObjectType("ewol::widget::ButtonColor");
	changeStatusIn(STATUS_UP);
	// Limit event at 1:
	setMouseLimit(1);
	propertyCanFocus.setDirectCheck(true);
}

void ewol::widget::ButtonColor::init() {
	ewol::Widget::init();
	propertyShape.notifyChange();
	propertyValue.notifyChange();
}

ewol::widget::ButtonColor::~ButtonColor() {
	
}

void ewol::widget::ButtonColor::calculateMinMaxSize() {
	ewol::Padding padding = m_shaper.getPadding();
	etk::String label = propertyValue.getString();
	vec3 minSize = m_text.calculateSize(label);
	m_minSize.setX(padding.x()*2 + minSize.x() + 7);
	m_minSize.setY(padding.y()*2 + minSize.y() );
	markToRedraw();
}



void ewol::widget::ButtonColor::onDraw() {
	m_shaper.draw();
	m_text.draw();
}


void ewol::widget::ButtonColor::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	EWOL_DEBUG("redraw");
	m_text.clear();
	m_shaper.clear();
	
	ewol::Padding padding = m_shaper.getPadding();
	
	etk::String label = propertyValue.getString();
	
	ivec2 localSize = m_minSize;
	
	vec3 tmpOrigin((m_size.x() - m_minSize.x()) / 2.0,
	               (m_size.y() - m_minSize.y()) / 2.0,
	               0);
	// no change for the text orogin : 
	vec3 tmpTextOrigin((m_size.x() - m_minSize.x()) / 2.0,
	                   (m_size.y() - m_minSize.y()) / 2.0,
	                   0);
	
	if (propertyFill->x() == true) {
		localSize.setX(m_size.x());
		tmpOrigin.setX(0);
		tmpTextOrigin.setX(0);
	}
	if (propertyFill->y() == true) {
		localSize.setY(m_size.y());
	}
	tmpOrigin += vec3(padding.xLeft(), padding.yButtom(), 0);
	tmpTextOrigin += vec3(padding.xLeft(), padding.yButtom(), 0);
	localSize -= ivec2(padding.x(), padding.y());
	
	// clean the element
	m_text.reset();
	if(    propertyValue.get().r() < 100
	    || propertyValue.get().g() < 100
	    || propertyValue.get().b() < 100) {
		m_text.setColor(etk::color::white);
	} else {
		m_text.setColor(etk::color::black);
	}
	m_text.setPos(tmpTextOrigin);
	m_text.setColorBg(propertyValue.get());
	m_text.setTextAlignement(tmpTextOrigin.x(), tmpTextOrigin.x()+localSize.x(), ewol::compositing::alignCenter);
	m_text.print(label);
	
	
	if (propertyFill->y() == true) {
		tmpOrigin.setY(padding.yButtom());
	}
	
	// selection area :
	m_selectableAreaPos = vec2(tmpOrigin.x()-padding.xLeft(), tmpOrigin.y()-padding.yButtom());
	m_selectableAreaSize = localSize + vec2(padding.x(),padding.y());
	vec3 tmpp = m_text.calculateSize(label);
	m_shaper.setShape(m_selectableAreaPos,
	                  m_selectableAreaSize,
	                  vec2(tmpTextOrigin.x(), tmpTextOrigin.y()),
	                  vec2(tmpp.x(), tmpp.y()));
}


bool ewol::widget::ButtonColor::onEventInput(const ewol::event::Input& _event) {
	bool previousHoverState = m_mouseHover;
	if(gale::key::status::leave == _event.getStatus()) {
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
	//EWOL_DEBUG("Event on BT ... mouse position : " << m_mouseHover);
	if (true == m_mouseHover) {
		if (1 == _event.getId()) {
			if(gale::key::status::down == _event.getStatus()) {
				m_buttonPressed = true;
				markToRedraw();
			}
			if(gale::key::status::up == _event.getStatus()) {
				m_buttonPressed = false;
				markToRedraw();
			}
			if(gale::key::status::pressSingle == _event.getStatus()) {
				m_buttonPressed = false;
				m_mouseHover = false;
				// create a context menu : 
				m_widgetContextMenu = ewol::widget::ContextMenu::create();
				if (m_widgetContextMenu == null) {
					EWOL_ERROR("Allocation Error");
					return true;
				}
				vec2 tmpPos = m_origin + m_selectableAreaPos + m_selectableAreaSize;
				tmpPos.setX( tmpPos.x() - m_minSize.x()/2.0);
				m_widgetContextMenu->setPositionMark(ewol::widget::ContextMenu::markButtom, tmpPos );
				
				ewol::widget::ColorChooserShared myColorChooser = widget::ColorChooser::create();
				myColorChooser->propertyValue.set(propertyValue.get());
				// set it in the pop-up-system : 
				m_widgetContextMenu->setSubWidget(myColorChooser);
				myColorChooser->signalChange.connect(sharedFromThis(), &ewol::widget::ButtonColor::onCallbackColorChange);
				ewol::widget::WindowsShared currentWindows = getWindows();
				if (currentWindows == null) {
					EWOL_ERROR("Can not get the curent Windows...");
					m_widgetContextMenu.reset();
				} else {
					currentWindows->popUpWidgetPush(m_widgetContextMenu);
				}
				markToRedraw();
			}
		}
	}
	if(    m_mouseHover != previousHoverState
	    || m_buttonPressed != previousPressed) {
		if (m_buttonPressed == true) {
			changeStatusIn(STATUS_PRESSED);
		} else {
			if (m_mouseHover == true) {
				changeStatusIn(STATUS_HOVER);
			} else {
				changeStatusIn(STATUS_UP);
			}
		}
	}
	return m_mouseHover;
}

void ewol::widget::ButtonColor::onCallbackColorChange(const etk::Color<>& _color) {
	propertyValue.set(_color);
}

void ewol::widget::ButtonColor::changeStatusIn(int32_t _newStatusId) {
	if (m_shaper.changeStatusIn(_newStatusId) == true) {
		m_PCH = getObjectManager().periodicCall.connect(this, &ewol::widget::ButtonColor::periodicCall);
		markToRedraw();
	}
}

void ewol::widget::ButtonColor::periodicCall(const ewol::event::Time& _event) {
	if (m_shaper.periodicCall(_event) == false) {
		m_PCH.disconnect();
	}
	markToRedraw();
}

void ewol::widget::ButtonColor::onChangePropertyValue() {
	signalChange.emit(propertyValue);
}

void ewol::widget::ButtonColor::onChangePropertyShape() {
	m_shaper.setSource(propertyShape.get());
	markToRedraw();
}


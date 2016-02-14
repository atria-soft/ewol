/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Manager.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>
#include <ewol/widget/Windows.h>
#include <ewol/Padding.h>

#undef __class__
#define __class__	"ContextMenu"


ewol::widget::ContextMenu::ContextMenu():
  propertyShape(*this, "shape", "", "the display name for config file"),
  propertyArrowPos(*this, "arrow-position", vec2(0,0), "Position of the arrow in the pop-up"),
  propertyArrawBorder(*this, "arrow-mode", markTop, "position of the arrow") {
	addObjectType("ewol::widget::ContextMenu");
	propertyArrawBorder.add(markTop, "top");
	propertyArrawBorder.add(markRight, "right");
	propertyArrawBorder.add(markButtom, "buttom");
	propertyArrawBorder.add(markLeft, "left");
	propertyArrawBorder.add(markNone, "none");
	
	
	m_offset = 20;
	
	
	m_colorBorder = etk::color::black;
	m_colorBorder.setA(0x7F);
	
	setMouseLimit(1);
}

void ewol::widget::ContextMenu::init(const std::string& _shaperName) {
	ewol::widget::Container::init();
	propertyShape.set(_shaperName);
	propertyExpand.set(bvec2(false,false));
}

ewol::widget::ContextMenu::~ContextMenu() {
	
}

void ewol::widget::ContextMenu::onChangeSize() {
	markToRedraw();
	// pop-up fill all the display :
	ewol::Padding padding = m_shaper.getPadding();
	EWOL_VERBOSE("our origin=" << m_origin << " size=" << m_size);
	if (m_subWidget == nullptr) {
		return;
	}
	vec2 subWidgetSize;
	vec2 subWidgetOrigin;
	subWidgetSize = m_subWidget->getCalculateMinSize();
	if (m_subWidget->canExpand().x() == true) {
		subWidgetSize.setX(m_size.x());
	}
	if (m_subWidget->canExpand().y() == true) {
		subWidgetSize.setY(m_size.y());
	}
	int32_t minWidth = 100;
	int32_t maxWidth = 300;
	subWidgetSize.setX((int32_t)std::max(minWidth, (int32_t)subWidgetSize.x()));
	subWidgetSize.setX((int32_t)std::min(maxWidth, (int32_t)subWidgetSize.x()));
	subWidgetSize.setY((int32_t)subWidgetSize.y());
	
	// set config to the Sub-widget
	switch (propertyArrawBorder.get()) {
		case markTop:
			subWidgetOrigin.setX((int32_t)(propertyArrowPos->x() - subWidgetSize.x()/2));
			subWidgetOrigin.setY((int32_t)(propertyArrowPos->y() - m_offset - subWidgetSize.y()));
			break;
		case markButtom:
			subWidgetOrigin.setX((int32_t)(propertyArrowPos->x() - subWidgetSize.x()/2));
			subWidgetOrigin.setY((int32_t)(propertyArrowPos->y() + m_offset));
			break;
		case markRight:
		case markLeft:
		default:
			subWidgetOrigin.setX((int32_t)(m_size.x() - m_origin.x() - subWidgetSize.x())/2 + m_origin.x());
			subWidgetOrigin.setY((int32_t)(m_size.y() - m_origin.y() - subWidgetSize.y())/2 + m_origin.y());
			break;
	}
	// set the widget position at the border of the screen
	subWidgetOrigin.setX( (int32_t)(   std::max(0, (int32_t)(subWidgetOrigin.x()-padding.x()))
	                                 + padding.x()) );
	subWidgetOrigin.setY( (int32_t)(   std::max(0, (int32_t)(subWidgetOrigin.y()-padding.y()))
	                                 + padding.y()) );
	switch (propertyArrawBorder.get()) {
		default:
		case markTop:
		case markButtom:
			if (propertyArrowPos->x() <= m_offset ) {
				subWidgetOrigin.setX(propertyArrowPos->x()+padding.xLeft());
			}
			break;
		case markRight:
		case markLeft:
			if (propertyArrowPos->y() <= m_offset ) {
				subWidgetOrigin.setY(propertyArrowPos->y()+padding.yButtom());
			}
			break;
	}
	EWOL_VERBOSE("       == > sub origin=" << subWidgetOrigin << " size=" << subWidgetSize);
	m_subWidget->setOrigin(subWidgetOrigin);
	m_subWidget->setSize(subWidgetSize);
	m_subWidget->onChangeSize();
}


void ewol::widget::ContextMenu::calculateMinMaxSize() {
	// call main class to calculate the min size...
	ewol::widget::Container::calculateMinMaxSize();
	// add padding of the display
	ewol::Padding padding = m_shaper.getPadding();
	m_minSize += vec2(padding.x(), padding.y());
	//EWOL_DEBUG("CalculateMinSize=>>" << m_minSize);
	markToRedraw();
}


void ewol::widget::ContextMenu::onDraw() {
	m_compositing.draw();
	m_shaper.draw();
}


void ewol::widget::ContextMenu::onRegenerateDisplay() {
	// call upper class :
	ewol::widget::Container::onRegenerateDisplay();
	if (needRedraw() == false) {
		return;
	}
	m_compositing.clear();
	m_shaper.clear();
	ewol::Padding padding = m_shaper.getPadding();
	
	if (m_subWidget == nullptr) {
		return;
	}
	vec2 tmpSize = m_subWidget->getSize();
	vec2 tmpOrigin = m_subWidget->getOrigin();
	
	// display border ...
	m_compositing.setColor(m_colorBorder);
	switch (propertyArrawBorder) {
		case markTop:
			m_compositing.setPos(vec3(propertyArrowPos->x(), propertyArrowPos->y(), 0.0f) );
			m_compositing.addVertex();
			if (propertyArrowPos->x() <= tmpOrigin.x() ) {
				float laking = m_offset - padding.yTop();
				m_compositing.setPos(vec3(propertyArrowPos->x()+laking, propertyArrowPos->y()-laking, 0.0f) );
				m_compositing.addVertex();
				m_compositing.setPos(vec3(propertyArrowPos->x(),        propertyArrowPos->y()-laking, 0.0f) );
				m_compositing.addVertex();
			} else {
				float laking = m_offset - padding.yTop();
				m_compositing.setPos(vec3(propertyArrowPos->x()+laking, propertyArrowPos->y()-laking, 0.0f) );
				m_compositing.addVertex();
				m_compositing.setPos(vec3(propertyArrowPos->x()-laking, propertyArrowPos->y()-laking, 0.0f) );
				m_compositing.addVertex();
			}
			break;
		case markButtom:
			m_compositing.setPos(vec3(propertyArrowPos->x(), propertyArrowPos->y(), 0) );
			m_compositing.addVertex();
			if (propertyArrowPos->x() <= tmpOrigin.x() ) {
				int32_t laking = m_offset - padding.yTop();
				m_compositing.setPos(vec3(propertyArrowPos->x()+laking, propertyArrowPos->y()+laking, 0.0f) );
				m_compositing.addVertex();
				m_compositing.setPos(vec3(propertyArrowPos->x(),        propertyArrowPos->y()+laking, 0.0f) );
				m_compositing.addVertex();
			} else {
				int32_t laking = m_offset - padding.yTop();
				m_compositing.setPos(vec3(propertyArrowPos->x()+laking, propertyArrowPos->y()+laking, 0.0f) );
				m_compositing.addVertex();
				m_compositing.setPos(vec3(propertyArrowPos->x()-laking, propertyArrowPos->y()+laking, 0.0f) );
				m_compositing.addVertex();
			}
			break;
		default:
		case markRight:
		case markLeft:
			EWOL_TODO("later");
			break;
	}
	
	vec2 shaperOrigin = tmpOrigin-vec2(padding.xLeft(), padding.yButtom());
	vec2 shaperSize = tmpSize+vec2(padding.x(), padding.y());
	m_shaper.setShape(vec2ClipInt32(shaperOrigin),
	                  vec2ClipInt32(shaperSize));
}

bool ewol::widget::ContextMenu::onEventInput(const ewol::event::Input& _event) {
	if (_event.getId() > 0) {
		if (ewol::widget::Container::getWidgetAtPos(_event.getPos()) != nullptr) {
			return false;
		}
		if(    _event.getStatus() == gale::key::status_down
		    || _event.getStatus() == gale::key::status_move
		    || _event.getStatus() == gale::key::status_single
		    || _event.getStatus() == gale::key::status_up
		    || _event.getStatus() == gale::key::status_enter
		    || _event.getStatus() == gale::key::status_leave ) {
			// Auto-remove ...
			autoDestroy();
			return true;
		}
	}
	return false;
}

std::shared_ptr<ewol::Widget> ewol::widget::ContextMenu::getWidgetAtPos(const vec2& _pos) {
	std::shared_ptr<ewol::Widget> val = ewol::widget::Container::getWidgetAtPos(_pos);
	if (val != nullptr) {
		return val;
	}
	return std::dynamic_pointer_cast<ewol::Widget>(shared_from_this());
}


void ewol::widget::ContextMenu::onPropertyChangeValue(const eproperty::Ref& _paramPointer) {
	ewol::widget::Container::onPropertyChangeValue(_paramPointer);
	if (_paramPointer == propertyArrowPos) {
		markToRedraw();
	} else if (_paramPointer == propertyArrawBorder) {
		markToRedraw();
	} else if (_paramPointer == propertyShape) {
		m_shaper.setSource(propertyShape.get());
		markToRedraw();
	}
}


void ewol::widget::ContextMenu::setPositionMarkAuto(const vec2& _origin, const vec2& _size) {
	std::shared_ptr<ewol::widget::Windows> windows = getWindows();
	vec2 globalSize = windows->getSize();
	// TODO : Support left and right
	float upperSize = globalSize.y() - (_origin.y() + _size.y());
	float underSize = _origin.y();
	if (underSize >= upperSize) {
		vec2 pos = _origin + _size - vec2(_size.x()*0.5f, 0.0f);
		setPositionMark(ewol::widget::ContextMenu::markButtom, pos);
	} else {
		vec2 pos = _origin + vec2(_size.x()*0.5f, 0.0f);
		setPositionMark(ewol::widget::ContextMenu::markTop, pos);
	}
}
void ewol::widget::ContextMenu::setPositionMark(enum markPosition _position, const vec2& _arrowPos) {
	propertyArrawBorder.set(_position);
	propertyArrowPos.set(_arrowPos);
}


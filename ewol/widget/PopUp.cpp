/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/PopUp.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/object/Manager.hpp>
#include <ewol/ewol.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::PopUp);

static const char* annimationIncrease = "increase";

ewol::widget::PopUp::PopUp() :
  propertyShape(this, "shaper",
                      etk::Uri("THEME_GUI:///PopUp.json?lib=ewol"),
                      "The shaper properties",
                      &ewol::widget::PopUp::onChangePropertyShape),
  propertyLockExpand(this, "lock",
                           bvec2(true,true),
                           "Lock expand contamination",
                           &ewol::widget::PopUp::onChangePropertyLockExpand),
  propertyCloseOutEvent(this, "out-click-remove",
                              false,
                              "Remove the widget if the use click outside") {
	addObjectType("ewol::widget::PopUp");
	
}

void ewol::widget::PopUp::init() {
	ewol::widget::Container::init();
	propertyFill.set(bvec2(false,false));
	propertyShape.notifyChange();
	propertyMinSize.set(gale::Dimension(vec2(80,80),gale::distance::pourcent));
	propertyExpand.set(bvec2(false, false));
}
ewol::widget::PopUp::~PopUp() {
	
}

void ewol::widget::PopUp::onChangeSize() {
	markToRedraw();
	if (m_subWidget == null) {
		return;
	}
	ewol::Padding padding = m_shaper.getPadding();
	vec2 subWidgetSize = m_subWidget->getCalculateMinSize();
	if (m_subWidget->canExpand().x() == true) {
		if (propertyLockExpand->x() == true) {
			subWidgetSize.setX(m_minSize.x());
		} else {
			subWidgetSize.setX(m_size.x()-padding.xLeft());
		}
	}
	if (m_subWidget->canExpand().y() == true) {
		if (propertyLockExpand->y() == true) {
			subWidgetSize.setY(m_minSize.y());
		} else {
			subWidgetSize.setY(m_size.y()-padding.yButtom());
		}
	}
	// limit the size of the element :
	//subWidgetSize.setMin(m_minSize);
	// posiition at a int32_t pos :
	subWidgetSize = vec2ClipInt32(subWidgetSize);
	
	// set config to the Sub-widget
	vec2 subWidgetOrigin = m_origin + (m_size-subWidgetSize)/2.0f;
	subWidgetOrigin = vec2ClipInt32(subWidgetOrigin);
	
	m_subWidget->setOrigin(subWidgetOrigin);
	m_subWidget->setSize(subWidgetSize);
	m_subWidget->onChangeSize();
}

void ewol::widget::PopUp::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (*propertyHide == true){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	if (m_subWidget == null) {
		return;
	}
	if(    m_shaper.getNextDisplayedStatus() == -1
	    && m_shaper.getTransitionStatus() >= 1.0) {
		ewol::DrawProperty prop = _displayProp;
		prop.limit(m_origin, m_size);
		m_subWidget->systemDraw(prop);
	}
}

void ewol::widget::PopUp::onDraw() {
	m_shaper.draw();
}

void ewol::widget::PopUp::onRegenerateDisplay() {
	if (needRedraw() == true) {
		m_shaper.clear();
		ewol::Padding padding = m_shaper.getPadding();
		vec2 tmpSize(0,0);
		bvec2 expand = canExpand();
		bvec2 fill = canFill();
		if (fill.x() == true) {
			tmpSize.setX(m_size.x()-padding.x());
		}
		if (fill.y() == true) {
			tmpSize.setY(m_size.y()-padding.y());
		}
		if (m_subWidget != null) {
			vec2 tmpSize = m_subWidget->getSize();
		}
		tmpSize.setMax(m_minSize);
		vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
		m_shaper.setShape(vec2(0,0),
		                  vec2ClipInt32(m_size),
		                  vec2ClipInt32(tmpOrigin-vec2(padding.xLeft(), padding.yButtom())),
		                  vec2ClipInt32(tmpSize + vec2(padding.x(), padding.y())));
	}
	// SUBwIDGET GENERATION ...
	if (m_subWidget != null) {
		m_subWidget->onRegenerateDisplay();
	}
}

ewol::WidgetShared ewol::widget::PopUp::getWidgetAtPos(const vec2& _pos) {
	ewol::WidgetShared val = ewol::widget::Container::getWidgetAtPos(_pos);
	if (val != null) {
		return val;
	}
	return ememory::dynamicPointerCast<ewol::Widget>(sharedFromThis());
}

void ewol::widget::PopUp::onChangePropertyShape() {
	m_shaper.setSource(*propertyShape);
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::PopUp::onChangePropertyLockExpand() {
	markToRedraw();
	requestUpdateSize();
}

bool ewol::widget::PopUp::onEventInput(const ewol::event::Input& _event) {
	if (_event.getId() == 0) {
		return false;
	}
	if (_event.getStatus() ==  gale::key::status::move) {
		return false;
	}
	if (*propertyCloseOutEvent == true) {
		return false;
	}
	ewol::Padding padding = m_shaper.getPadding();
	vec2 tmpSize(0,0);
	if (m_subWidget != null) {
		vec2 tmpSize = m_subWidget->getSize();
	}
	tmpSize.setMax(m_minSize);
	vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
	
	tmpOrigin -= vec2(padding.xLeft(), padding.yButtom());
	tmpSize += vec2(padding.x(), padding.y());
	vec2 pos = relativePosition(_event.getPos());
	if(    pos.x() < tmpOrigin.x()
	    || pos.y() < tmpOrigin.y()
	    || pos.x() > tmpOrigin.x()+tmpSize.x()
	    || pos.y() > tmpOrigin.y()+tmpSize.y() ) {
		autoDestroy();
		return true;
	}
	return false;
}


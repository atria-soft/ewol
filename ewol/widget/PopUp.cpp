/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/PopUp.h>
#include <ewol/widget/Manager.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>
#include <ewol/ewol.h>

#undef __class__
#define __class__ "PopUp"

static const char* annimationIncrease = "increase";

ewol::widget::PopUp::PopUp() :
  m_shaper(*this, "shaper", "The shaper properties"),
  m_lockExpand(*this, "lock", bvec2(true,true), "Lock expand contamination"),
  m_closeOutEvent(*this, "out-click-remove", false, "Remove the widget if the use click outside") {
	addObjectType("ewol::widget::PopUp");
	// Add annimations :
	addAnnimationType(ewol::Widget::annimationModeEnableAdd, annimationIncrease);
}

void ewol::widget::PopUp::init(const std::string& _shaperName) {
	ewol::widget::Container::init();
	m_userFill.set(bvec2(false,false));
	m_shaper.setString(_shaperName);
	setMinSize(gale::Dimension(vec2(80,80),gale::Dimension::Pourcent));
	m_userExpand.set(bvec2(false, false));
}
ewol::widget::PopUp::~PopUp() {
	
}


void ewol::widget::PopUp::setShaperName(const std::string& _shaperName) {
	m_shaper.setString(_shaperName);
	markToRedraw();
}

void ewol::widget::PopUp::calculateSize(const vec2& _available) {
	#if 0
		//not call basic class ==> change methode to generate basic size ...
		ewol::Widget::calculateSize(_available);
	#else
		vec2 size = _available;
		size.setMax(m_minSize);
		m_size = size;
	#endif
	if (nullptr != m_subWidget) {
		ewol::Padding padding = m_shaper->getPadding();
		vec2 subWidgetSize = m_subWidget->getCalculateMinSize();
		if (true == m_subWidget->canExpand().x()) {
			if (m_lockExpand->x() == true) {
				subWidgetSize.setX(m_minSize.x());
			} else {
				subWidgetSize.setX(m_size.x()-padding.xLeft());
			}
		}
		if (true == m_subWidget->canExpand().y()) {
			if (m_lockExpand->y() == true) {
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
		m_subWidget->calculateSize(subWidgetSize);
	}
	markToRedraw();
}

void ewol::widget::PopUp::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (true == m_hide){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	if (nullptr!=m_subWidget) {
		if(    m_shaper->getNextDisplayedStatus() == -1
		    && m_shaper->getTransitionStatus() >= 1.0) {
			ewol::DrawProperty prop = _displayProp;
			prop.limit(m_origin, m_size);
			m_subWidget->systemDraw(prop);
		}
	}
}

void ewol::widget::PopUp::onDraw() {
	m_shaper->draw();
}

void ewol::widget::PopUp::onRegenerateDisplay() {
	if (true == needRedraw()) {
		m_shaper->clear();
		ewol::Padding padding = m_shaper->getPadding();
		vec2 tmpSize(0,0);
		bvec2 expand = canExpand();
		bvec2 fill = canFill();
		if (fill.x()) {
			tmpSize.setX(m_size.x()-padding.x());
		}
		if (fill.y()) {
			tmpSize.setY(m_size.y()-padding.y());
		}
		if (nullptr != m_subWidget) {
			vec2 tmpSize = m_subWidget->getSize();
		}
		tmpSize.setMax(m_minSize);
		vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
		
		m_shaper->setShape(vec2(0,0),
		                   vec2ClipInt32(m_size),
		                   vec2ClipInt32(tmpOrigin-vec2(padding.xLeft(), padding.yButtom())),
		                   vec2ClipInt32(tmpSize + vec2(padding.x(), padding.y())));
	}
	// SUBwIDGET GENERATION ...
	if (nullptr != m_subWidget) {
		m_subWidget->onRegenerateDisplay();
	}
}

std::shared_ptr<ewol::Widget> ewol::widget::PopUp::getWidgetAtPos(const vec2& _pos) {
	std::shared_ptr<ewol::Widget> val = ewol::widget::Container::getWidgetAtPos(_pos);
	if (nullptr != val) {
		return val;
	}
	return std::dynamic_pointer_cast<ewol::Widget>(shared_from_this());
}

void ewol::widget::PopUp::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::widget::Container::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_shaper) {
		markToRedraw();
		requestUpdateSize();
	} else if (_paramPointer == m_lockExpand) {
		markToRedraw();
		requestUpdateSize();
	} else if (_paramPointer == m_closeOutEvent) {
		// nothing to do ...
	}
}

bool ewol::widget::PopUp::onEventInput(const ewol::event::Input& _event) {
	if (0 != _event.getId()) {
		if (true == m_closeOutEvent) {
			ewol::Padding padding = m_shaper->getPadding();
			vec2 tmpSize(0,0);
			if (nullptr != m_subWidget) {
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
		}
	}
	return false;
}


bool ewol::widget::PopUp::onStartAnnimation(enum ewol::Widget::annimationMode _mode) {
	if (_mode==0) {
		/*
		if (m_annimationTypeStart.get() != annimationIncrease) {
			return false;
		}
		*/
	} else {
		/*
		if (m_annimationTypeStop.get() != annimationIncrease) {
			return false;
		}
		*/
	}
	return false;
	if (true == m_shaper->changeStatusIn(1) ) {
		periodicCallEnable();
		return true;
	}
	return false;
}

void ewol::widget::PopUp::onStopAnnimation() {
	periodicCallDisable();
}

void ewol::widget::PopUp::periodicCall(const ewol::event::Time& _event) {
	if (false == m_shaper->periodicCall(_event) ) {
		stopAnnimation();
	}
	markToRedraw();
}


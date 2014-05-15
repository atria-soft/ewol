/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/WSlider.h>
#include <ewol/ewol.h>


static const char* l_listsladingMode[ewol::widget::WSlider::sladingTransition_count] = {
	"transition vertical",
	"transition horisantal"
};

std::ostream& operator <<(std::ostream& _os, const enum ewol::widget::WSlider::sladingMode _obj) {
	_os << l_listsladingMode[_obj];
	return _os;
}

#undef __class__
#define __class__ "WSlider"

// Event list of properties
const char* const ewol::widget::WSlider::eventStartSlide = "ewol-widget-wslider-event-start-slide";
const char* const ewol::widget::WSlider::eventStopSlide = "ewol-widget-wslider-event-stop-slide";
// Config list of properties
const char* const ewol::widget::WSlider::configMode = "mode";
const char* const ewol::widget::WSlider::configSpeed = "speed";

static ewol::Widget* create() {
	return new ewol::widget::WSlider();
}

void ewol::widget::WSlider::init(ewol::widget::Manager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}

ewol::widget::WSlider::WSlider() :
  m_windowsSources(0),
  m_windowsDestination(0),
  m_windowsRequested(-1),
  m_slidingProgress(1.0f),
  m_transitionSpeed(1.0f),
  m_transitionSlide(sladingTransitionHori) {
	addObjectType("ewol::widget::WSlider");
	addEventId(eventStartSlide);
	addEventId(eventStopSlide);
	// add configuration
	registerConfig(configMode, "list", "vert;hori", "Transition mode of the slider");
	registerConfig(configSpeed, "float", NULL, "Transition speed of the slider");
}

ewol::widget::WSlider::~WSlider() {
	
}


void ewol::widget::WSlider::calculateSize(const vec2& _availlable) {
	//EWOL_DEBUG("Update size");
	ewol::widget::ContainerN::calculateSize(_availlable);
	
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii < (int32_t)m_subWidget.size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->setOrigin(m_origin+m_offset);
				m_subWidget[iii]->calculateSize(m_size);
			}
		}
	} else {
		float factor = -1.0f;
		if (m_windowsSources < m_windowsDestination) {
			factor = 1.0f;
		}
		int32_t iii = m_windowsSources;
		if (iii < (int32_t)m_subWidget.size()) {
			if (NULL != m_subWidget[iii]) {
				if (m_transitionSlide == sladingTransitionHori) {
					m_subWidget[iii]->setOrigin(   vec2(m_origin.x() + factor*(m_size.x()*m_slidingProgress),
					                                    m_origin.y())
					                             + m_offset);
				} else {
					m_subWidget[iii]->setOrigin(   vec2(m_origin.x(),
					                                    m_origin.y() + factor*(m_size.y()*m_slidingProgress))
					                             + m_offset);
				}
				m_subWidget[iii]->calculateSize(m_size);
			}
		}
		iii = m_windowsDestination;
		if (iii < (int32_t)m_subWidget.size()) {
			if (NULL != m_subWidget[iii]) {
				if (m_transitionSlide == sladingTransitionHori) {
					m_subWidget[iii]->setOrigin(   vec2(m_origin.x() + factor*(m_size.x()*m_slidingProgress - m_size.x()),
					                                    m_origin.y())
					                             + m_offset);
				} else {
					m_subWidget[iii]->setOrigin(   vec2(m_origin.x(),
					                                    m_origin.y() + factor*(m_size.y()*m_slidingProgress - m_size.y()))
					                             + m_offset);
				}
				m_subWidget[iii]->calculateSize(m_size);
			}
		}
	}
	markToRedraw();
}

void ewol::widget::WSlider::subWidgetSelectSetVectorId(int32_t _id) {
	if (_id<0) {
		EWOL_ERROR("Can not change to a widget not present : vectID=" << _id);
		return;
	}
	if (_id != m_windowsDestination) {
		m_windowsRequested = _id;
		generateEventId(eventStartSlide);
		periodicCallEnable();
		markToRedraw();
	}
}

void ewol::widget::WSlider::subWidgetSelectSet(int32_t _id) {
	int32_t elementID = -1;
	// search element in the list : 
	for (size_t iii=0 ; iii<m_subWidget.size() ; iii++) {
		if (m_subWidget[iii] != NULL) {
			if (m_subWidget[iii]->getId() == _id) {
				elementID = iii;
				break;
			}
		}
	}
	subWidgetSelectSetVectorId(elementID);
}

void ewol::widget::WSlider::subWidgetSelectSet(ewol::Widget* _widgetPointer) {
	if (_widgetPointer == NULL) {
		EWOL_ERROR("Can not change to a widget NULL");
		return;
	}
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (m_subWidget[iii] != NULL) {
			if (m_subWidget[iii] == _widgetPointer) {
				subWidgetSelectSetVectorId(iii);
				return;
			}
		}
	}
	EWOL_ERROR("Can not change to a widget not present");
}

void ewol::widget::WSlider::subWidgetSelectSet(const std::string& _widgetName) {
	if (_widgetName == "") {
		EWOL_ERROR("Can not change to a widget with no name (input)");
		return;
	}
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (m_subWidget[iii] != NULL) {
			if (m_subWidget[iii]->getName() == _widgetName) {
				subWidgetSelectSetVectorId(iii);
				return;
			}
		}
	}
	EWOL_ERROR("Can not change to a widget not present");
}

void ewol::widget::WSlider::setTransitionMode(enum sladingMode _mode) {
	if (m_transitionSlide != _mode) {
		m_transitionSlide = _mode;
		markToRedraw();
	}
}

void ewol::widget::WSlider::periodicCall(const ewol::event::Time& _event) {
	if (m_slidingProgress >= 1.0) {
		m_windowsSources = m_windowsDestination;
		if(    m_windowsRequested != -1
		    && m_windowsRequested != m_windowsSources) {
			m_windowsDestination = m_windowsRequested;
			m_slidingProgress = 0.0;
		} else {
			// end of periodic :
			periodicCallDisable();
			generateEventId(eventStopSlide);
		}
		m_windowsRequested = -1;
	}
	
	if (m_slidingProgress < 1.0) {
		if (m_windowsRequested != -1 && m_slidingProgress<0.5 ) {
			// invert sources with destination
			int32_t tmppp = m_windowsDestination;
			m_windowsDestination = m_windowsSources;
			m_windowsSources = tmppp;
			m_slidingProgress = 1.0f - m_slidingProgress;
			if (m_windowsRequested == m_windowsDestination) {
				m_windowsRequested = -1;
			}
		}
		m_slidingProgress += _event.getDeltaCall()/m_transitionSpeed;
		m_slidingProgress = etk_avg(0.0f, m_slidingProgress, 1.0f);
	}
	calculateSize(m_size);
	markToRedraw();
}

void ewol::widget::WSlider::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (true == m_hide){
		// widget is hidden ...
		return;
	}
	// note : do not call the widget container  == > overload this one ...
	ewol::Widget::systemDraw(_displayProp);
	
	// subwidget draw
	ewol::DrawProperty prop = _displayProp;
	prop.limit(m_origin, m_size);
	
	if (m_windowsDestination == m_windowsSources) {
		//EWOL_DEBUG("Draw : " << m_windowsDestination);
		int32_t iii = m_windowsDestination;
		if (iii >= 0 || (size_t)iii < m_subWidget.size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->systemDraw(prop);
			}
		}
	} else {
		//EWOL_DEBUG("Draw : " << m_windowsSources << "=>" << m_windowsDestination << "progress=" << ((float)m_slidingProgress/1000.) );
		// draw Sources :
		int32_t iii = m_windowsSources;
		if (iii >= 0 || (size_t)iii < m_subWidget.size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->systemDraw(prop);
			}
		}
		// draw Destination : 
		iii = m_windowsDestination;
		if (iii >= 0 || (size_t)iii < m_subWidget.size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->systemDraw(prop);
			}
		}
	}
}

void ewol::widget::WSlider::onRegenerateDisplay() {
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii >= 0 || (size_t)iii < m_subWidget.size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->onRegenerateDisplay();
			}
		}
	} else {
		int32_t iii = m_windowsSources;
		if (iii >= 0 || (size_t)iii < m_subWidget.size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->onRegenerateDisplay();
			}
		}
		iii = m_windowsDestination;
		if (iii >= 0 || (size_t)iii < m_subWidget.size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->onRegenerateDisplay();
			}
		}
	}
}

bool ewol::widget::WSlider::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::widget::ContainerN::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configMode) {
		enum sladingMode tmpTransition = sladingTransitionHori;
		if(compare_no_case(_conf.getData(), "vert") == true) {
			tmpTransition = sladingTransitionVert;
		} else if(compare_no_case(_conf.getData(), "hori") == true) {
			tmpTransition = sladingTransitionHori;
		}
		setTransitionMode(tmpTransition);
		return true;
	}
	if (_conf.getConfig() == configSpeed) {
		setTransitionSpeed(std::stof(_conf.getData()));
		return true;
	}
	return false;
}

bool ewol::widget::WSlider::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::widget::ContainerN::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configMode) {
		switch(m_transitionSlide){
			default:
			case sladingTransitionHori:
				_result = "hori";
				break;
			case sladingTransitionVert:
				_result = "vert";
				break;
		}
		return true;
	}
	if (_config == configMode) {
		_result = std::to_string(getTransitionSpeed());
		return true;
	}
	return false;
}

ewol::Widget* ewol::widget::WSlider::getWidgetAtPos(const vec2& _pos) {
	if (true == isHide()) {
		return NULL;
	}
	if (m_windowsDestination == m_windowsSources) {
		if (m_windowsDestination < (int64_t)m_subWidget.size()) {
			vec2 tmpSize = m_subWidget[m_windowsDestination]->getSize();
			vec2 tmpOrigin = m_subWidget[m_windowsDestination]->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::Widget * tmpWidget = m_subWidget[m_windowsDestination]->getWidgetAtPos(_pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
				return NULL;
			}
		}
	} else {
		if (m_windowsDestination < (int64_t)m_subWidget.size()) {
			vec2 tmpSize = m_subWidget[m_windowsDestination]->getSize();
			vec2 tmpOrigin = m_subWidget[m_windowsDestination]->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::Widget * tmpWidget = m_subWidget[m_windowsDestination]->getWidgetAtPos(_pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
				return NULL;
			}
		}
		if (m_windowsSources < (int64_t)m_subWidget.size()) {
			vec2 tmpSize = m_subWidget[m_windowsSources]->getSize();
			vec2 tmpOrigin = m_subWidget[m_windowsSources]->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::Widget * tmpWidget = m_subWidget[m_windowsSources]->getWidgetAtPos(_pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
				return NULL;
			}
		}
	}
	return NULL;
}


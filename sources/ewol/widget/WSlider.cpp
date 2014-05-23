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
	registerConfig(configSpeed, "float", nullptr, "Transition speed of the slider");
}

ewol::widget::WSlider::~WSlider() {
	
}


void ewol::widget::WSlider::calculateSize(const vec2& _availlable) {
	//EWOL_DEBUG("Update size");
	ewol::widget::ContainerN::calculateSize(_availlable);
	
	if (m_windowsDestination == m_windowsSources) {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->setOrigin(m_origin+m_offset);
			(*it)->calculateSize(m_size);
		}
	} else {
		float factor = -1.0f;
		if (m_windowsSources < m_windowsDestination) {
			factor = 1.0f;
		}
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsSources);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
		     if (m_transitionSlide == sladingTransitionHori) {
				(*it)->setOrigin(   vec2(m_origin.x() + factor*(m_size.x()*m_slidingProgress),
				                         m_origin.y())
				                  + m_offset);
			} else {
				(*it)->setOrigin(   vec2(m_origin.x(),
				                         m_origin.y() + factor*(m_size.y()*m_slidingProgress))
				                  + m_offset);
			}
			(*it)->calculateSize(m_size);
		}
		it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			if (m_transitionSlide == sladingTransitionHori) {
				(*it)->setOrigin(   vec2(m_origin.x() + factor*(m_size.x()*m_slidingProgress - m_size.x()),
				                         m_origin.y())
				                  + m_offset);
			} else {
				(*it)->setOrigin(   vec2(m_origin.x(),
				                         m_origin.y() + factor*(m_size.y()*m_slidingProgress - m_size.y()))
				                  + m_offset);
			}
			(*it)->calculateSize(m_size);
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
	int32_t elementID = 0;
	// search element in the list : 
	for (auto it : m_subWidget) {
		elementID ++;
		if (it != nullptr) {
			if (it->getId() == _id) {
				break;
			}
		}
	}
	if (elementID < m_subWidget.size()) {
		subWidgetSelectSetVectorId(elementID);
	} else {
		subWidgetSelectSetVectorId(-1);
	}
}

void ewol::widget::WSlider::subWidgetSelectSet(const ewol::object::Shared<ewol::Widget>& _widgetPointer) {
	if (_widgetPointer == nullptr) {
		EWOL_ERROR("Can not change to a widget nullptr");
		return;
	}
	int32_t iii = 0;
	for (auto it : m_subWidget) {
		if (    it != nullptr
		     && it == _widgetPointer) {
			subWidgetSelectSetVectorId(iii);
			return;
		}
		iii++;
	}
	EWOL_ERROR("Can not change to a widget not present");
}

void ewol::widget::WSlider::subWidgetSelectSet(const std::string& _widgetName) {
	if (_widgetName == "") {
		EWOL_ERROR("Can not change to a widget with no name (input)");
		return;
	}
	int32_t iii = 0;
	for (auto it : m_subWidget) {
		if (    it != nullptr
		     && it->getName() == _widgetName) {
			subWidgetSelectSetVectorId(iii);
			return;
		}
		iii++;
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
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->systemDraw(prop);
		}
	} else {
		//EWOL_DEBUG("Draw : " << m_windowsSources << "=>" << m_windowsDestination << "progress=" << ((float)m_slidingProgress/1000.) );
		// draw Sources :
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsSources);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->systemDraw(prop);
		}
		// draw Destination : 
		it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->systemDraw(prop);
		}
	}
}

void ewol::widget::WSlider::onRegenerateDisplay() {
	if (m_windowsDestination == m_windowsSources) {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->onRegenerateDisplay();
		}
	} else {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsSources);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->onRegenerateDisplay();
		}
		it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->onRegenerateDisplay();
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

ewol::object::Shared<ewol::Widget> ewol::widget::WSlider::getWidgetAtPos(const vec2& _pos) {
	if (true == isHide()) {
		return nullptr;
	}
	if (m_windowsDestination == m_windowsSources) {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			vec2 tmpSize = (*it)->getSize();
			vec2 tmpOrigin = (*it)->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::object::Shared<ewol::Widget> tmpWidget = (*it)->getWidgetAtPos(_pos);
				if (nullptr != tmpWidget) {
					return tmpWidget;
				}
				return nullptr;
			}
		}
	} else {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			vec2 tmpSize = (*it)->getSize();
			vec2 tmpOrigin = (*it)->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::object::Shared<ewol::Widget> tmpWidget = (*it)->getWidgetAtPos(_pos);
				if (nullptr != tmpWidget) {
					return tmpWidget;
				}
				return nullptr;
			}
		}
		it = m_subWidget.begin();
		std::advance(it, m_windowsSources);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			vec2 tmpSize = (*it)->getSize();
			vec2 tmpOrigin = (*it)->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::object::Shared<ewol::Widget> tmpWidget = (*it)->getWidgetAtPos(_pos);
				if (nullptr != tmpWidget) {
					return tmpWidget;
				}
				return nullptr;
			}
		}
	}
	return nullptr;
}


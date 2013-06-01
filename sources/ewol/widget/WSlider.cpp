/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/WSlider.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>


static const char* l_listsladingMode[widget::WSlider::sladingTransition_count] = {
	"transition vertical",
	"transition horisantal"
};

etk::CCout& operator <<(etk::CCout& _os, const widget::WSlider::sladingMode_te _obj)
{
	_os << l_listsladingMode[_obj];
	return _os;
}

#undef __class__
#define __class__	"WSlider"



// Event list of properties
const char* const widget::WSlider::eventStartSlide = "ewol-widget-wslider-event-start-slide";
const char* const widget::WSlider::eventStopSlide = "ewol-widget-wslider-event-stop-slide";
// Config list of properties
const char* const widget::WSlider::configMode = "mode";


static ewol::Widget* Create(void)
{
	return new widget::WSlider();
}

void widget::WSlider::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::WSlider::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}

widget::WSlider::WSlider(void) : 
	m_windowsSources(0),
	m_windowsDestination(0),
	m_windowsRequested(-1),
	m_slidingProgress(1.0f),
	m_transitionSpeed(1.0f),
	m_transitionSlide(sladingTransitionHori)
{
	AddEventId(eventStartSlide);
	AddEventId(eventStopSlide);
	// Add configuration
	RegisterConfig(configMode, "list", "vert;hori", "Transition mode of the slider");
}

widget::WSlider::~WSlider(void)
{
	
}


void widget::WSlider::CalculateSize(const vec2& _availlable)
{
	//EWOL_DEBUG("Update Size");
	widget::ContainerN::CalculateSize(_availlable);
	
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SetOrigin(m_origin+m_offset);
				m_subWidget[iii]->CalculateSize(m_size);
			}
		}
	} else {
		float factor = -1.0f;
		if (m_windowsSources < m_windowsDestination) {
			factor = 1.0f;
		}
		int32_t iii = m_windowsSources;
		if (iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				if (m_transitionSlide == sladingTransitionHori) {
					m_subWidget[iii]->SetOrigin(   vec2(m_origin.x() + factor*(m_size.x()*m_slidingProgress),
					                                    m_origin.y())
					                             + m_offset);
				} else {
					m_subWidget[iii]->SetOrigin(   vec2(m_origin.x(),
					                                    m_origin.y() + factor*(m_size.y()*m_slidingProgress))
					                             + m_offset);
				}
				m_subWidget[iii]->CalculateSize(m_size);
			}
		}
		iii = m_windowsDestination;
		if (iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				if (m_transitionSlide == sladingTransitionHori) {
					m_subWidget[iii]->SetOrigin(   vec2(m_origin.x() + factor*(m_size.x()*m_slidingProgress - m_size.x()),
					                                    m_origin.y())
					                             + m_offset);
				} else {
					m_subWidget[iii]->SetOrigin(   vec2(m_origin.x(),
					                                    m_origin.y() + factor*(m_size.y()*m_slidingProgress - m_size.y()))
					                             + m_offset);
				}
				m_subWidget[iii]->CalculateSize(m_size);
			}
		}
	}
	MarkToRedraw();
}

void widget::WSlider::SubWidgetSelectSet(int32_t _id)
{
	if (_id<0 || _id >= m_subWidget.Size()) {
		EWOL_ERROR("Can not change to a widget not present");
	}
	if (_id != m_windowsDestination) {
		m_windowsRequested = _id;
		GenerateEventId(eventStartSlide);
		PeriodicCallEnable();
		MarkToRedraw();
	}
}

void widget::WSlider::SubWidgetSelectSet(ewol::Widget* _widgetPointer)
{
	if (_widgetPointer == NULL) {
		EWOL_ERROR("Can not change to a widget NULL");
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (m_subWidget[iii] != NULL) {
			if (m_subWidget[iii] == _widgetPointer) {
				SubWidgetSelectSet(iii);
				return;
			}
		}
	}
	EWOL_ERROR("Can not change to a widget not present");
}

void widget::WSlider::SubWidgetSelectSet(const etk::UString& _widgetName)
{
	if (_widgetName == "") {
		EWOL_ERROR("Can not change to a widget with no name (input)");
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (m_subWidget[iii] != NULL) {
			if (m_subWidget[iii]->GetName() == _widgetName) {
				SubWidgetSelectSet(iii);
				return;
			}
		}
	}
	EWOL_ERROR("Can not change to a widget not present");
}



void widget::WSlider::SetTransitionMode(widget::WSlider::sladingMode_te _mode)
{
	if (m_transitionSlide != _mode) {
		m_transitionSlide = _mode;
		MarkToRedraw();
	}
}



void widget::WSlider::PeriodicCall(const ewol::EventTime& _event)
{
	if (m_slidingProgress >= 1.0) {
		m_windowsSources = m_windowsDestination;
		if(    m_windowsRequested != -1
		    && m_windowsRequested != m_windowsSources) {
			m_windowsDestination = m_windowsRequested;
			m_slidingProgress = 0.0;
		} else {
			// end of periodic :
			PeriodicCallDisable();
			GenerateEventId(eventStopSlide);
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
		m_slidingProgress += _event.GetDeltaCall()/m_transitionSpeed;
		m_slidingProgress = etk_avg(0.0f, m_slidingProgress, 1.0f);
	}
	CalculateSize(m_size);
	MarkToRedraw();
}


void widget::WSlider::SystemDraw(const ewol::DrawProperty& _displayProp)
{
	if (true==m_hide){
		// widget is hidden ...
		return;
	}
	// note : do not call the widget container ==> overload this one ...
	ewol::Widget::SystemDraw(_displayProp);
	
	// subwidget draw
	ewol::DrawProperty prop = _displayProp;
	prop.Limit(m_origin, m_size);
	
	if (m_windowsDestination == m_windowsSources) {
		//EWOL_DEBUG("Draw : " << m_windowsDestination);
		int32_t iii = m_windowsDestination;
		if (iii>=0 || iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SystemDraw(prop);
			}
		}
	} else {
		//EWOL_DEBUG("Draw : " << m_windowsSources << "=>" << m_windowsDestination << "progress=" << ((float)m_slidingProgress/1000.) );
		// draw Sources :
		int32_t iii = m_windowsSources;
		if (iii>=0 || iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SystemDraw(prop);
			}
		}
		// Draw Destination : 
		iii = m_windowsDestination;
		if (iii>=0 || iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SystemDraw(prop);
			}
		}
	}
}

void widget::WSlider::OnRegenerateDisplay(void)
{
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii>=0 || iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->OnRegenerateDisplay();
			}
		}
	} else {
		int32_t iii = m_windowsSources;
		if (iii>=0 || iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->OnRegenerateDisplay();
			}
		}
		iii = m_windowsDestination;
		if (iii>=0 || iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->OnRegenerateDisplay();
			}
		}
	}
}


bool widget::WSlider::OnSetConfig(const ewol::EConfig& _conf)
{
	if (true == widget::ContainerN::OnSetConfig(_conf)) {
		return true;
	}
	if (_conf.GetConfig() == configMode) {
		sladingMode_te tmpTransition = sladingTransitionHori;
		if(true == _conf.GetData().CompareNoCase("vert")) {
			tmpTransition = sladingTransitionVert;
		} else if(true == _conf.GetData().CompareNoCase("hori")) {
			tmpTransition = sladingTransitionHori;
		}
		SetTransitionMode(tmpTransition);
		return true;
	}
	return false;
}

bool widget::WSlider::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == widget::ContainerN::OnGetConfig(_config, _result)) {
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
	return false;
}



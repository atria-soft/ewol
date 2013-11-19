/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/meta/ColorChooser.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/WidgetManager.h>
//#include <vector>
#include <vector>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ColorChooser"




extern const char * const ewolEventColorChooserChange          = "ewol-event-color-chooser-change";


const char * const eventColorBarHasChange          = "event-color-bar-has-change";
const char * const eventColorSpecificHasChange     = "event-color-specific-has-change";


widget::ColorChooser::ColorChooser(void) :
  widget::Sizer(widget::Sizer::modeVert) {
	addObjectType("widget::ColorChooser");
	addEventId(ewolEventColorChooserChange);
	m_widgetColorBar = NULL;
	m_widgetRed = NULL;
	m_widgetGreen = NULL;
	m_widgetBlue = NULL;
	m_widgetAlpha = NULL;
	lockExpand(bvec2(true,true));
		m_widgetColorBar = new widget::ColorBar();
			m_widgetColorBar->registerOnEvent(this, ewolEventColorBarChange, eventColorBarHasChange);
			m_widgetColorBar->setFill(bvec2(true,true));
			/*
			m_widgetColorBar->setWidth(200);
			m_widgetColorBar->setHeigh(200);
			*/
			subWidgetAdd(m_widgetColorBar);
		
		etk::Color<> sliderColor;
		sliderColor = etk::color::black;
		
		m_widgetRed = new widget::Slider();
			m_widgetRed->registerOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetRed->setExpand(bvec2(true,false));
			m_widgetRed->setFill(bvec2(true,false));
			m_widgetRed->setMin(0);
			m_widgetRed->setMax(255);
			sliderColor = 0xFF0000FF;
			m_widgetRed->setColor(sliderColor);
			subWidgetAdd(m_widgetRed);
		m_widgetGreen = new widget::Slider();
			m_widgetGreen->registerOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetGreen->setExpand(bvec2(true,false));
			m_widgetGreen->setFill(bvec2(true,false));
			m_widgetGreen->setMin(0);
			sliderColor = 0x00FF00FF;
			m_widgetGreen->setColor(sliderColor);
			m_widgetGreen->setMax(255);
			subWidgetAdd(m_widgetGreen);
		m_widgetBlue = new widget::Slider();
			m_widgetBlue->registerOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetBlue->setExpand(bvec2(true,false));
			m_widgetBlue->setFill(bvec2(true,false));
			m_widgetBlue->setMin(0);
			sliderColor = 0x0000FFFF;
			m_widgetBlue->setColor(sliderColor);
			m_widgetBlue->setMax(255);
			subWidgetAdd(m_widgetBlue);
		m_widgetAlpha = new widget::Slider();
			m_widgetAlpha->registerOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetAlpha->setExpand(bvec2(true,false));
			m_widgetAlpha->setFill(bvec2(true,false));
			m_widgetAlpha->setMin(0);
			m_widgetAlpha->setMax(255);
			subWidgetAdd(m_widgetAlpha);
	
	m_currentColor = etk::color::white;
}


widget::ColorChooser::~ColorChooser(void) {
	
}


void widget::ColorChooser::setColor(etk::Color<> _newColor) {
	m_currentColor = _newColor;
	if (NULL != m_widgetRed) {
		m_widgetRed->setValue(m_currentColor.r());
	}
	if (NULL != m_widgetGreen) {
		m_widgetGreen->setValue(m_currentColor.g());
	}
	if (NULL != m_widgetBlue) {
		m_widgetBlue->setValue(m_currentColor.b());
	}
	if (NULL != m_widgetAlpha) {
		m_widgetAlpha->setValue(m_currentColor.a());
	}
	if (NULL != m_widgetColorBar) {
		m_widgetColorBar->setCurrentColor(m_currentColor);
	}
}


etk::Color<> widget::ColorChooser::getColor(void) {
	return m_currentColor;
}


void widget::ColorChooser::onReceiveMessage(const ewol::EMessage& _msg) {
	if (NULL == _msg.getCaller()) {
		return;
	}
	//EWOL_INFO("Receive Extern Event ... : widgetPointer=" << CallerObject << "\"" << eventId << "\"  == > data=\"" << data << "\"" );
	if (eventColorBarHasChange == _msg.getMessage()) {
		// == > colorBar has change ...
		uint8_t tmpAlpha = m_currentColor.a();
		// the colorbar has no notion of the alpha  == > keep it ...
		if (NULL != m_widgetColorBar) {
			m_currentColor = m_widgetColorBar->getCurrentColor();
		}
		m_currentColor.setA(tmpAlpha);
		if (NULL != m_widgetRed) {
			m_widgetRed->setValue(m_currentColor.r());
		}
		if (NULL != m_widgetGreen) {
			m_widgetGreen->setValue(m_currentColor.g());
		}
		if (NULL != m_widgetBlue) {
			m_widgetBlue->setValue(m_currentColor.b());
		}
		if (NULL != m_widgetAlpha) {
			m_widgetAlpha->setValue(m_currentColor.a());
		}
		generateEventId(ewolEventColorChooserChange, m_currentColor.getString());
	} else if (eventColorSpecificHasChange == _msg.getMessage()) {
		// Slider has changes his color  == > get the one change ...
		if (_msg.getCaller() == m_widgetRed) {
			m_currentColor.setR(m_widgetRed->getValue());
		}
		if (_msg.getCaller() == m_widgetGreen) {
			m_currentColor.setG(m_widgetGreen->getValue());
		}
		if (_msg.getCaller() == m_widgetBlue) {
			m_currentColor.setB(m_widgetBlue->getValue());
		}
		if (_msg.getCaller() == m_widgetAlpha) {
			m_currentColor.setA(m_widgetAlpha->getValue());
		}
		if (NULL != m_widgetColorBar) {
			m_widgetColorBar->setCurrentColor(m_currentColor);
		}
		generateEventId(ewolEventColorChooserChange, m_currentColor.getString());
	}
};


void widget::ColorChooser::onObjectRemove(ewol::EObject * _removeObject) {
	// First step call parrent : 
	widget::Sizer::onObjectRemove(_removeObject);
	// second step find if in all the elements ...
	if(_removeObject == m_widgetRed) {
		m_widgetRed = NULL;
	}
	if(_removeObject == m_widgetGreen) {
		m_widgetGreen = NULL;
	}
	if(_removeObject == m_widgetBlue) {
		m_widgetBlue = NULL;
	}
	if(_removeObject == m_widgetAlpha) {
		m_widgetAlpha = NULL;
	}
	if(_removeObject == m_widgetColorBar) {
		m_widgetColorBar = NULL;
	}
}


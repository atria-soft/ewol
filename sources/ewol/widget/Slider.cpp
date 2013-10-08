/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Slider.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventSliderChange    = "ewol-event-slider-change";



#undef __class__
#define __class__	"Slider"


static ewol::Widget* create(void)
{
	return new widget::Slider();
}

void widget::Slider::init(ewol::WidgetManager& _widgetManager)
{
	_widgetManager.addWidgetCreator(__class__,&create);
}


const int32_t dotRadius = 6;

widget::Slider::Slider(void)
{
	addEventId(ewolEventSliderChange);
	
	m_value = 0;
	m_min = 0;
	m_max = 10;
	
	m_textColorFg = etk::color::black;
	
	m_textColorBg = etk::color::black;
	m_textColorBg.setA(0x3F);
	setCanHaveFocus(true);
	// Limit event at 1:
	setMouseLimit(1);
}

widget::Slider::~Slider(void)
{
	
}


void widget::Slider::calculateMinMaxSize(void)
{
	vec2 minTmp = m_userMinSize.getPixel();
	m_minSize.setValue(etk_max(minTmp.x(), 40),
	                   etk_max(minTmp.y(), dotRadius*2) );
	markToRedraw();
}


void widget::Slider::setValue(int32_t val)
{
	m_value = etk_max(etk_min(val, m_max), m_min);
	markToRedraw();
}


int32_t widget::Slider::getValue(void)
{
	return m_value;
}


void widget::Slider::setMin(int32_t val)
{
	m_min = val;
	m_value = etk_max(etk_min(m_value, m_max), m_min);
	markToRedraw();
}


void widget::Slider::setMax(int32_t val)
{
	m_max = val;
	m_value = etk_max(etk_min(m_value, m_max), m_min);
	markToRedraw();
}


void widget::Slider::onRegenerateDisplay(void)
{
	if (true == needRedraw()) {
		// clean the object list ...
		clearOObjectList();
		
		ewol::Drawing * tmpDraw = new ewol::Drawing;
		
		tmpDraw->setColor(m_textColorFg);
		// draw a line :
		tmpDraw->setThickness(1);
		tmpDraw->setPos(vec3(dotRadius, m_size.y()/2, 0) );
		tmpDraw->lineTo(vec3(m_size.x()-dotRadius, m_size.y()/2, 0) );
		tmpDraw->setThickness(0);
		
		etk::Color<> borderDot = m_textColorFg;
		borderDot.setA(borderDot.a()/2);
		tmpDraw->setPos(vec3(4+((float)(m_value-m_min)/(float)(m_max-m_min))*(float)(m_size.x()-2*dotRadius), m_size.y()/2, 0) );
		tmpDraw->setColorBg(borderDot);
		tmpDraw->circle(dotRadius);
		tmpDraw->setColorBg(m_textColorFg);
		tmpDraw->circle(dotRadius/1.6);
		addOObject(tmpDraw);
	}
}


bool widget::Slider::onEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = relativePosition(_event.getPos());
	//EWOL_DEBUG("Event on Slider ...");
	if (1 == _event.getId()) {
		if(    ewol::keyEvent::statusSingle == _event.getStatus()
		    || ewol::keyEvent::statusMove   == _event.getStatus()) {
			// get the new position :
			EWOL_VERBOSE("Event on Slider (" << relativePos.x() << "," << relativePos.y() << ")");
			int32_t oldValue = m_value;
			m_value = m_min + (float)(relativePos.x() - dotRadius) / (float)(m_size.x()-2*dotRadius) * (float)(m_max-m_min);
			m_value = etk_max(etk_min(m_value, m_max), m_min);
			if (oldValue != m_value) {
				EWOL_DEBUG(" new value : " << m_value << " in [" << m_min << ".." << m_max << "]");
				generateEventId(ewolEventSliderChange, m_value);
				markToRedraw();
			}
			return true;
		}
	}
	return false;
}



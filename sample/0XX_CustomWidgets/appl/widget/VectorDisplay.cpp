/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD 3 clauses (see license file)
 */

#include <appl/debug.h>
#include <appl/widget/VectorDisplay.h>
#include <etk/tool.h>

#undef __class__
#define __class__ "VectorDisplay"


appl::widget::VectorDisplay::VectorDisplay() :
  m_autoDisplay(false),
  m_minVal(-1.0f),
  m_maxVal(1.0f) {
	addObjectType("appl::widget::VectorDisplay");
}

void appl::widget::VectorDisplay::init() {
	ewol::Widget::init();
	markToRedraw();
}


appl::widget::VectorDisplay::~VectorDisplay() {
	
}


void appl::widget::VectorDisplay::setValue(const std::vector<float>& _data) {
	m_data = _data;
	markToRedraw();
}

void appl::widget::VectorDisplay::ToggleAuto() {
	if (m_autoDisplay == false) {
		periodicCallEnable();
		m_autoDisplay = true;
	} else {
		periodicCallDisable();
		m_autoDisplay = false;
	}
}

void appl::widget::VectorDisplay::onDraw() {
	m_draw.draw();
}


void appl::widget::VectorDisplay::onRegenerateDisplay() {
	//!< Check if we really need to redraw the display, if not needed, we redraw the previous data ...
	if (needRedraw() == false) {
		return;
	}
	// remove previous data
	m_draw.clear();
	// set background
	m_draw.setColor(etk::color::black);
	m_draw.setPos(vec2(0,0));
	m_draw.rectangleWidth(m_size);

	if (m_data.size() == 0) {
		return;
	}
	// set all the line:
	m_draw.setColor(etk::color::white);
	m_draw.setThickness(1);
	float origin = m_size.y()*0.5f;
	
	float ratioY = m_size.y() / (m_maxVal - m_minVal);
	float stepX = m_size.x() / (float)m_data.size();
	m_draw.setPos(vec2(0, origin + ratioY*m_data[0]));
	float baseX = 0;
	for (size_t iii=1; iii<m_data.size(); ++iii) {
		m_draw.lineTo(vec2((float)iii*stepX, origin + ratioY*m_data[iii]));
	}
}

void appl::widget::VectorDisplay::periodicCall(const ewol::event::Time& _event) {
	for (size_t iii=0; iii<std::max(m_data.size()/200.0f, 50.0f); ++iii) {
		if (m_data.size() > 50) {
			m_data.erase(m_data.begin());
		}
		m_data.push_back(etk::tool::frand(m_minVal, m_maxVal));
	}
	markToRedraw();
}

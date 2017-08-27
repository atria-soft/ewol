/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <appl/debug.hpp>
#include <appl/widget/VectorDisplay.hpp>
#include <ewol/object/Manager.hpp>
#include <etk/tool.hpp>

//! [EWOL_SAMPLE_CW_CONSTRUCTOR]
appl::widget::VectorDisplay::VectorDisplay() :
  m_autoDisplay(false),
  m_minVal(-1.0f),
  m_maxVal(1.0f) {
	addObjectType("appl::widget::VectorDisplay");
}
//! [EWOL_SAMPLE_CW_CONSTRUCTOR]

//! [EWOL_SAMPLE_CW_INIT]
void appl::widget::VectorDisplay::init() {
	ewol::Widget::init();
	markToRedraw();
	// set call all time (sample ...).
	getObjectManager().periodicCall.connect(sharedFromThis(), &appl::widget::VectorDisplay::periodicEvent);
}
//! [EWOL_SAMPLE_CW_INIT]


appl::widget::VectorDisplay::~VectorDisplay() {
	
}


void appl::widget::VectorDisplay::setValue(const etk::Vector<float>& _data) {
	m_data = _data;
	markToRedraw();
}

void appl::widget::VectorDisplay::ToggleAuto() {
	if (m_autoDisplay == false) {
		m_autoDisplay = true;
	} else {
		m_autoDisplay = false;
	}
}

//! [EWOL_SAMPLE_CW_DRAW]
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

void appl::widget::VectorDisplay::periodicEvent(const ewol::event::Time& _event) {
	if (m_autoDisplay == false) {
		return;
	}
	for (size_t iii=0; iii<etk::max(m_data.size()/200.0f, 50.0f); ++iii) {
		if (m_data.size() > 50) {
			m_data.erase(m_data.begin());
		}
		m_data.pushBack(etk::tool::frand(m_minVal, m_maxVal));
	}
	markToRedraw();
}

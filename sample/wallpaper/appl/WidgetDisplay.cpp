/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/WidgetDisplay.h>

#include <appl/debug.h>
#include <etk/tool.h>

appl::WidgetDisplay::WidgetDisplay() {
	addObjectType("appl::WidgetDisplay");
	propertyCanFocus.setDirectCheck(true);
}

void appl::WidgetDisplay::init() {
	ewol::Widget::init();
	m_compositing.setSource("DATA:SnowFlake.svg", ivec2(128,128));
	periodicCallEnable();
	for (int32_t iii=0; iii<250 ; ++iii) {
		m_elements.push_back(appl::WidgetDisplay::Element());
	}
}

appl::WidgetDisplay::~WidgetDisplay() {
	
}

void appl::WidgetDisplay::onDraw() {
	m_compositing.draw();
}

appl::WidgetDisplay::Element::Element() {
	regenerate(vec2(1024,2048));
	m_lifeTime = etk::tool::frand(-4,4);
	m_life = m_lifeTime;
}

void appl::WidgetDisplay::Element::regenerate(const vec2& _size) {
	float sizeDisplay=etk::tool::frand(10,64);
	m_position = vec2(etk::tool::frand(0,_size.x()),
	                  etk::tool::frand(_size.y(),_size.y()+64));
	m_size = vec2(sizeDisplay,sizeDisplay);
	m_angle = etk::tool::frand(-3.1415952f,3.1415952f);
	m_lifeTime = etk::tool::frand(1,10);
	m_life = m_lifeTime;
	m_lifeTime += 4;
	m_angleAdd = etk::tool::frand(-0.05f,0.05f);
	m_downSpeed = etk::tool::frand(0.5f,3.f);
}

void appl::WidgetDisplay::Element::move(float _deltaTime) {
	m_lifeTime -= _deltaTime;
	m_position -= vec2(0, m_downSpeed);
	m_angle += m_angleAdd;
}

void appl::WidgetDisplay::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	// remove data of the previous composition :
	m_compositing.clear();
	for (int32_t iii=0; iii<m_elements.size() ; ++iii) {
		if (m_elements[iii].m_lifeTime < 0.0f) {
			int32_t color = 0xFF*(1.0f+0.25*m_elements[iii].m_lifeTime);
			m_compositing.setColor(etk::Color<>(0xFF, 0xFF, 0xFF, color));
		} else if (m_elements[iii].m_lifeTime > m_elements[iii].m_life) {
			int32_t color = 0xFF*(1.0f-0.25*(m_elements[iii].m_lifeTime - m_elements[iii].m_life));
			m_compositing.setColor(etk::Color<>(0xFF, 0xFF, 0xFF, color));
		} else {
			m_compositing.setColor(etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF));
		}
		//3d7dab
		m_compositing.setPos(m_elements[iii].m_position);
		m_compositing.setAngle(m_elements[iii].m_angle);
		m_compositing.print(m_elements[iii].m_size);
	}
}

void appl::WidgetDisplay::periodicCall(const ewol::event::Time& _event) {
	float curentDelta=_event.getDeltaCall();
	// set the somposition properties :
	for (int32_t iii=0; iii<m_elements.size() ; ++iii) {
		m_elements[iii].move(curentDelta);
		if(    m_elements[iii].m_lifeTime < -4.0f
		    || m_elements[iii].m_position.y() < -m_elements[iii].m_size.y()*1.4f) {
			// change properties
			m_elements[iii].regenerate(m_size);
		}
	}
	markToRedraw();
}

bool appl::WidgetDisplay::onEventInput(const ewol::event::Input& _event) {
	//EWOL_DEBUG("Input event : " << _event);
	vec2 relPos = relativePosition(_event.getPos());
	if (_event.getType() == gale::key::type_finger) {
		keepFocus();
		if (1 == _event.getId()) {
			APPL_DEBUG("Touch event ... ");
		}
	}
	
	return false;
}


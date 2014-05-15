/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Manager.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>
#include <ewol/Padding.h>

#undef __class__
#define __class__	"ContextMenu"


const char* const ewol::Widget::ContextMenu::configArrowPosition = "arrow-position";
const char* const ewol::Widget::ContextMenu::configArrowMode     = "arrow-mode";
const char* const ewol::Widget::ContextMenu::configShaper        = "shaper";

static ewol::object::Shared<ewol::Widget> Create() {
	return new ewol::Widget::ContextMenu();
}

void ewol::Widget::ContextMenu::init(ewol::object::Shared<ewol::Widget::Manager> _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&Create);
}



ewol::Widget::ContextMenu::ContextMenu(const std::string& _shaperName) :
  m_shaper(_shaperName) {
	addObjectType("ewol::Widget::ContextMenu");
	// add basic configurations :
	registerConfig(configArrowPosition, "vec2", NULL, "position of the arrow");
	registerConfig(configArrowMode, "list", "none;left;buttom;right;top", "Position of the arrow in the pop-up");
	registerConfig(configShaper, "string", NULL, "the display name for config file");
	
	m_userExpand.setValue(false,false);
	
	m_offset = 20;
	
	m_colorBackGroung = etk::color::white;
	
	m_colorBorder = etk::color::black;
	m_colorBorder.setA(0x7F);
	
	m_arrowPos.setValue(0,0);
	m_arrawBorder = markTop;
	setMouseLimit(1);
}

ewol::Widget::ContextMenu::~ContextMenu() {
	
}

void ewol::Widget::ContextMenu::setShaperName(const std::string& _shaperName) {
	m_shaper.setSource(_shaperName);
	markToRedraw();
}


void ewol::Widget::ContextMenu::calculateSize(const vec2& _availlable) {
	//EWOL_DEBUG("CalculateSize=" << availlable);
	// pop-up fill all the display :
	m_size = _availlable;
	ewol::Padding padding = m_shaper.getPadding();
	EWOL_VERBOSE("our origin=" << m_origin << " size=" << m_size);
	if (NULL != m_subWidget) {
		vec2 subWidgetSize;
		vec2 subWidgetOrigin;
		subWidgetSize = m_subWidget->getCalculateMinSize();
		if (true == m_subWidget->canExpand().x()) {
			subWidgetSize.setX(m_size.x());
		}
		if (true == m_subWidget->canExpand().y()) {
			subWidgetSize.setY(m_size.y());
		}
		int32_t minWidth = 100;
		int32_t maxWidth = 300;
		subWidgetSize.setX((int32_t)etk_max(minWidth, subWidgetSize.x()));
		subWidgetSize.setX((int32_t)etk_min(maxWidth, subWidgetSize.x()));
		subWidgetSize.setY((int32_t)subWidgetSize.y());
		
		// set config to the Sub-widget
		switch (m_arrawBorder) {
			case markTop:
				subWidgetOrigin.setX((int32_t)(m_arrowPos.x() - subWidgetSize.x()/2));
				subWidgetOrigin.setY((int32_t)(m_arrowPos.y() - m_offset - subWidgetSize.y()));
				break;
			case markButtom:
				subWidgetOrigin.setX((int32_t)(m_arrowPos.x() - subWidgetSize.x()/2));
				subWidgetOrigin.setY((int32_t)(m_arrowPos.y() + m_offset));
				break;
			case markRight:
			case markLeft:
			default:
				subWidgetOrigin.setX((int32_t)(m_size.x() - m_origin.x() - subWidgetSize.x())/2 + m_origin.x());
				subWidgetOrigin.setY((int32_t)(m_size.y() - m_origin.y() - subWidgetSize.y())/2 + m_origin.y());
				break;
		}
		// set the widget position at the border of the screen
		subWidgetOrigin.setX( (int32_t)(   etk_max(0, (subWidgetOrigin.x()-padding.x()))
		                                 + padding.x()) );
		subWidgetOrigin.setY( (int32_t)(   etk_max(0, (subWidgetOrigin.y()-padding.y()))
		                                 + padding.y()) );
		switch (m_arrawBorder)
		{
			default:
			case markTop:
			case markButtom:
				if (m_arrowPos.x() <= m_offset ) {
					subWidgetOrigin.setX(m_arrowPos.x()+padding.xLeft());
				}
				break;
			case markRight:
			case markLeft:
				if (m_arrowPos.y() <= m_offset ) {
					subWidgetOrigin.setY(m_arrowPos.y()+padding.yButtom());
				}
				break;
		}
		EWOL_VERBOSE("       == > sub origin=" << subWidgetOrigin << " size=" << subWidgetSize);
		m_subWidget->setOrigin(subWidgetOrigin);
		m_subWidget->calculateSize(subWidgetSize);
	}
	markToRedraw();
}


void ewol::Widget::ContextMenu::calculateMinMaxSize() {
	// call main class to calculate the min size...
	ewol::Widget::Container::calculateMinMaxSize();
	// add padding of the display
	ewol::Padding padding = m_shaper.getPadding();
	m_minSize += vec2(padding.x(), padding.y());
	//EWOL_DEBUG("CalculateMinSize=>>" << m_minSize);
	markToRedraw();
}


void ewol::Widget::ContextMenu::onDraw() {
	m_compositing.draw();
	m_shaper.draw();
}


void ewol::Widget::ContextMenu::onRegenerateDisplay() {
	// call upper class :
	ewol::Widget::Container::onRegenerateDisplay();
	if (needRedraw() == false) {
		return;
	}
	m_compositing.clear();
	m_shaper.clear();
	ewol::Padding padding = m_shaper.getPadding();
	
	if (m_subWidget == NULL) {
		return;
	}
	vec2 tmpSize = m_subWidget->getSize();
	vec2 tmpOrigin = m_subWidget->getOrigin();
	
	// display border ...
	m_compositing.setColor(m_colorBorder);
	switch (m_arrawBorder) {
		case markTop:
			m_compositing.setPos(vec3(m_arrowPos.x(), m_arrowPos.y(), 0.0f) );
			m_compositing.addVertex();
			if (m_arrowPos.x() <= tmpOrigin.x() ) {
				float laking = m_offset - padding.yTop();
				m_compositing.setPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()-laking, 0.0f) );
				m_compositing.addVertex();
				m_compositing.setPos(vec3(m_arrowPos.x(),        m_arrowPos.y()-laking, 0.0f) );
				m_compositing.addVertex();
			} else {
				float laking = m_offset - padding.yTop();
				m_compositing.setPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()-laking, 0.0f) );
				m_compositing.addVertex();
				m_compositing.setPos(vec3(m_arrowPos.x()-laking, m_arrowPos.y()-laking, 0.0f) );
				m_compositing.addVertex();
			}
			break;
		case markButtom:
			m_compositing.setPos(vec3(m_arrowPos.x(), m_arrowPos.y(), 0) );
			m_compositing.addVertex();
			if (m_arrowPos.x() <= tmpOrigin.x() ) {
				int32_t laking = m_offset - padding.yTop();
				m_compositing.setPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()+laking, 0.0f) );
				m_compositing.addVertex();
				m_compositing.setPos(vec3(m_arrowPos.x(),        m_arrowPos.y()+laking, 0.0f) );
				m_compositing.addVertex();
			} else {
				int32_t laking = m_offset - padding.yTop();
				m_compositing.setPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()+laking, 0.0f) );
				m_compositing.addVertex();
				m_compositing.setPos(vec3(m_arrowPos.x()-laking, m_arrowPos.y()+laking, 0.0f) );
				m_compositing.addVertex();
			}
			break;
		default:
		case markRight:
		case markLeft:
			EWOL_TODO("later");
			break;
	}
	
	vec2 shaperOrigin = tmpOrigin-vec2(padding.xLeft(), padding.yButtom());
	vec2 shaperSize = tmpSize+vec2(padding.x(), padding.y());
	m_shaper.setShape(vec2ClipInt32(shaperOrigin),
	                  vec2ClipInt32(shaperSize));
}

bool ewol::Widget::ContextMenu::onEventInput(const ewol::event::Input& _event) {
	if (_event.getId() > 0) {
		if (NULL != ewol::Widget::Container::getWidgetAtPos(_event.getPos())) {
			return false;
		}
		if(    _event.getStatus() == ewol::key::statusDown
		    || _event.getStatus() == ewol::key::statusMove
		    || _event.getStatus() == ewol::key::statusSingle
		    || _event.getStatus() == ewol::key::statusUp
		    || _event.getStatus() == ewol::key::statusEnter
		    || _event.getStatus() == ewol::key::statusLeave ) {
			// Auto-remove ...
			autoDestroy();
			return true;
		}
	}
	return false;
}


void ewol::Widget::ContextMenu::setPositionMark(enum markPosition _position, vec2 _arrowPos) {
	EWOL_DEBUG("set context menu at the position : " << _arrowPos);
	m_arrawBorder = _position;
	m_arrowPos = _arrowPos;
	markToRedraw();
}

ewol::object::Shared<ewol::Widget> ewol::Widget::ContextMenu::getWidgetAtPos(const vec2& _pos) {
	ewol::object::Shared<ewol::Widget> val = ewol::Widget::Container::getWidgetAtPos(_pos);
	if (NULL != val) {
		return val;
	}
	return this;
}


bool ewol::Widget::ContextMenu::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::Widget::Container::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configArrowPosition) {
		m_arrowPos = _conf.getData();
		return true;
	}
	if (_conf.getConfig() == configArrowMode) {
		if(compare_no_case(_conf.getData(), "top") == true) {
			m_arrawBorder = markTop;
		} else if(compare_no_case(_conf.getData(), "right") == true) {
			m_arrawBorder = markRight;
		} else if(compare_no_case(_conf.getData(), "buttom") == true) {
			m_arrawBorder = markButtom;
		} else if(compare_no_case(_conf.getData(), "left") == true) {
			m_arrawBorder = markLeft;
		} else {
			m_arrawBorder = markNone;
		}
		return true;
	}
	if (_conf.getConfig() == configShaper) {
		setShaperName(_conf.getData());
		return true;
	}
	return false;
}

bool ewol::Widget::ContextMenu::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::Widget::Container::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configArrowPosition) {
		_result = m_arrowPos;
		return true;
	}
	if (_config == configArrowMode) {
		switch(m_arrawBorder) {
			case markTop:
				_result = "top";
				break;
			case markRight:
				_result = "right";
				break;
			case markButtom:
				_result = "buttom";
				break;
			case markLeft:
				_result = "left";
				break;
			default:
			case markNone:
				_result = "none";
				break;
		}
		return true;
	}
	if (_config == configShaper) {
		_result = m_shaper.getSource();;
		return true;
	}
	return false;
}






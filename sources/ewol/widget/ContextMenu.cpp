/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"ContextMenu"


const char* const widget::ContextMenu::configArrowPosition = "arrow-position";
const char* const widget::ContextMenu::configArrowMode     = "arrow-mode";
const char* const widget::ContextMenu::configShaper        = "shaper";

static ewol::Widget* Create(void)
{
	return new widget::ContextMenu();
}

void widget::ContextMenu::init(ewol::WidgetManager& _widgetManager)
{
	_widgetManager.addWidgetCreator(__class__,&Create);
}



widget::ContextMenu::ContextMenu(const etk::UString& _shaperName) :
	m_shaper(_shaperName)
{
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
	m_arrawBorder = widget::CONTEXT_MENU_MARK_TOP;
	setMouseLimit(1);
}

widget::ContextMenu::~ContextMenu(void)
{
	
}

void widget::ContextMenu::setShaperName(const etk::UString& _shaperName)
{
	m_shaper.setSource(_shaperName);
	markToRedraw();
}


void widget::ContextMenu::calculateSize(const vec2& _availlable)
{
	//EWOL_DEBUG("CalculateSize=" << availlable);
	// pop-up fill all the display :
	m_size = _availlable;
	vec2 padding = m_shaper.getPadding();
	EWOL_DEBUG("our origin=" << m_origin << " size=" << m_size);
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
			case widget::CONTEXT_MENU_MARK_TOP:
				subWidgetOrigin.setX((int32_t)(m_arrowPos.x() - subWidgetSize.x()/2));
				subWidgetOrigin.setY((int32_t)(m_arrowPos.y() - m_offset - subWidgetSize.y()));
				break;
			case widget::CONTEXT_MENU_MARK_BOTTOM:
				subWidgetOrigin.setX((int32_t)(m_arrowPos.x() - subWidgetSize.x()/2));
				subWidgetOrigin.setY((int32_t)(m_arrowPos.y() + m_offset));
				break;
			case widget::CONTEXT_MENU_MARK_RIGHT:
			case widget::CONTEXT_MENU_MARK_LEFT:
			default:
				subWidgetOrigin.setX((int32_t)(m_size.x() - m_origin.x() - subWidgetSize.x())/2 + m_origin.x());
				subWidgetOrigin.setY((int32_t)(m_size.y() - m_origin.y() - subWidgetSize.y())/2 + m_origin.y());
				break;
		}
		// set the widget position at the border of the screen
		subWidgetOrigin.setX( (int32_t)(   etk_max(0, (subWidgetOrigin.x()-padding.x()*2))
		                                 + padding.x()*2) );
		subWidgetOrigin.setY( (int32_t)(   etk_max(0, (subWidgetOrigin.y()-padding.y()*2))
		                                 + padding.y()*2) );
		switch (m_arrawBorder)
		{
			default:
			case widget::CONTEXT_MENU_MARK_TOP:
			case widget::CONTEXT_MENU_MARK_BOTTOM:
				if (m_arrowPos.x() <= m_offset ) {
					subWidgetOrigin.setX(m_arrowPos.x()+padding.x());
				}
				break;
			case widget::CONTEXT_MENU_MARK_RIGHT:
			case widget::CONTEXT_MENU_MARK_LEFT:
				if (m_arrowPos.y() <= m_offset ) {
					subWidgetOrigin.setY(m_arrowPos.y()+padding.y());
				}
				break;
		}
		EWOL_DEBUG("       == > sub origin=" << subWidgetOrigin << " size=" << subWidgetSize);
		m_subWidget->setOrigin(subWidgetOrigin);
		m_subWidget->calculateSize(subWidgetSize);
	}
	markToRedraw();
}


void widget::ContextMenu::calculateMinMaxSize(void)
{
	// call main class to calculate the min size...
	widget::Container::calculateMinMaxSize();
	// add padding of the display
	m_minSize += m_shaper.getPadding();
	//EWOL_DEBUG("CalculateMinSize=>>" << m_minSize);
	markToRedraw();
}


void widget::ContextMenu::onDraw(void)
{
	m_compositing.draw();
	m_shaper.draw();
}


void widget::ContextMenu::onRegenerateDisplay(void)
{
	// call upper class :
	widget::Container::onRegenerateDisplay();
	if (true == needRedraw()) {
		m_compositing.clear();
		m_shaper.clear();
		vec2 padding = m_shaper.getPadding();
		
		if (NULL != m_subWidget) {
			vec2 tmpSize = m_subWidget->getSize();
			vec2 tmpOrigin = m_subWidget->getOrigin();
			
			// display border ...
			m_compositing.setColor(m_colorBorder);
			switch (m_arrawBorder)
			{
				case widget::CONTEXT_MENU_MARK_TOP:
					m_compositing.setPos(vec3(m_arrowPos.x(), m_arrowPos.y(), 0.0f) );
					m_compositing.addVertex();
					if (m_arrowPos.x() <= tmpOrigin.x() ) {
						float laking = m_offset - padding.y();
						m_compositing.setPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()-laking, 0.0f) );
						m_compositing.addVertex();
						m_compositing.setPos(vec3(m_arrowPos.x(),        m_arrowPos.y()-laking, 0.0f) );
						m_compositing.addVertex();
					} else {
						float laking = m_offset - padding.y();
						m_compositing.setPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()-laking, 0.0f) );
						m_compositing.addVertex();
						m_compositing.setPos(vec3(m_arrowPos.x()-laking, m_arrowPos.y()-laking, 0.0f) );
						m_compositing.addVertex();
					}
					break;
				case widget::CONTEXT_MENU_MARK_BOTTOM:
					m_compositing.setPos(vec3(m_arrowPos.x(), m_arrowPos.y(), 0) );
					m_compositing.addVertex();
					if (m_arrowPos.x() <= tmpOrigin.x() ) {
						int32_t laking = m_offset - padding.y();
						m_compositing.setPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()+laking, 0.0f) );
						m_compositing.addVertex();
						m_compositing.setPos(vec3(m_arrowPos.x(),        m_arrowPos.y()+laking, 0.0f) );
						m_compositing.addVertex();
					} else {
						int32_t laking = m_offset - padding.y();
						m_compositing.setPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()+laking, 0.0f) );
						m_compositing.addVertex();
						m_compositing.setPos(vec3(m_arrowPos.x()-laking, m_arrowPos.y()+laking, 0.0f) );
						m_compositing.addVertex();
					}
					break;
				default:
				case widget::CONTEXT_MENU_MARK_RIGHT:
				case widget::CONTEXT_MENU_MARK_LEFT:
					EWOL_TODO("later");
					break;
			}
			
			vec2 shaperOrigin = tmpOrigin-padding;
			vec2 shaperSize = tmpSize+padding*2.0f;
			m_shaper.setOrigin(vec2ClipInt32(shaperOrigin));
			m_shaper.setSize(vec2ClipInt32(shaperSize));
			m_shaper.setInsidePos(vec2ClipInt32(shaperOrigin+padding));
			m_shaper.setInsideSize(vec2ClipInt32(shaperSize-padding*2.0f));
		}
	}
}
bool widget::ContextMenu::onEventInput(const ewol::EventInput& _event)
{
	if (_event.getId() > 0) {
		if (NULL != widget::Container::getWidgetAtPos(_event.getPos())) {
			return false;
		}
		if(    _event.getStatus() == ewol::keyEvent::statusDown
		    || _event.getStatus() == ewol::keyEvent::statusMove
		    || _event.getStatus() == ewol::keyEvent::statusSingle
		    || _event.getStatus() == ewol::keyEvent::statusUp
		    || _event.getStatus() == ewol::keyEvent::statusEnter
		    || _event.getStatus() == ewol::keyEvent::statusLeave ) {
			// Auto-remove ...
			autoDestroy();
			return true;
		}
	}
	return false;
}


void widget::ContextMenu::setPositionMark(markPosition_te position, vec2 arrowPos)
{
	EWOL_DEBUG("set context menu at the position : " << arrowPos);
	m_arrawBorder = position;
	m_arrowPos = arrowPos;
	markToRedraw();
}

ewol::Widget* widget::ContextMenu::getWidgetAtPos(const vec2& pos)
{
	ewol::Widget* val = widget::Container::getWidgetAtPos(pos);
	if (NULL != val) {
		return val;
	}
	return this;
}


bool widget::ContextMenu::onSetConfig(const ewol::EConfig& _conf)
{
	if (true == widget::Container::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configArrowPosition) {
		m_arrowPos = _conf.getData();
		return true;
	}
	if (_conf.getConfig() == configArrowMode) {
		if(true == _conf.getData().compareNoCase("top")) {
			m_arrawBorder = CONTEXT_MENU_MARK_TOP;
		} else if(true == _conf.getData().compareNoCase("right")) {
			m_arrawBorder = CONTEXT_MENU_MARK_RIGHT;
		} else if(true == _conf.getData().compareNoCase("buttom")) {
			m_arrawBorder = CONTEXT_MENU_MARK_BOTTOM;
		} else if(true == _conf.getData().compareNoCase("left")) {
			m_arrawBorder = CONTEXT_MENU_MARK_LEFT;
		} else {
			m_arrawBorder = CONTEXT_MENU_MARK_NONE;
		}
		return true;
	}
	if (_conf.getConfig() == configShaper) {
		setShaperName(_conf.getData());
		return true;
	}
	return false;
}

bool widget::ContextMenu::onGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == widget::Container::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configArrowPosition) {
		_result = m_arrowPos;
		return true;
	}
	if (_config == configArrowMode) {
		switch(m_arrawBorder) {
			case CONTEXT_MENU_MARK_TOP:
				_result = "top";
				break;
			case CONTEXT_MENU_MARK_RIGHT:
				_result = "right";
				break;
			case CONTEXT_MENU_MARK_BOTTOM:
				_result = "buttom";
				break;
			case CONTEXT_MENU_MARK_LEFT:
				_result = "left";
				break;
			default:
			case CONTEXT_MENU_MARK_NONE:
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






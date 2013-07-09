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

void widget::ContextMenu::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::ContextMenu::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}



widget::ContextMenu::ContextMenu(const etk::UString& _shaperName) :
	m_shaper(_shaperName)
{
	// add basic configurations :
	RegisterConfig(configArrowPosition, "vec2", NULL, "position of the arrow");
	RegisterConfig(configArrowMode, "list", "none;left;buttom;right;top", "Position of the arrow in the pop-up");
	RegisterConfig(configShaper, "string", NULL, "the display name for config file");
	
	m_userExpand.setValue(false,false);
	
	m_offset = 20;
	
	m_colorBackGroung = etk::color::white;
	
	m_colorBorder = etk::color::black;
	m_colorBorder.SetA(0x7F);
	
	m_arrowPos.setValue(0,0);
	m_arrawBorder = widget::CONTEXT_MENU_MARK_TOP;
	SetMouseLimit(1);
}

widget::ContextMenu::~ContextMenu(void)
{
	
}

void widget::ContextMenu::SetShaperName(const etk::UString& _shaperName)
{
	m_shaper.SetSource(_shaperName);
	MarkToRedraw();
}


void widget::ContextMenu::CalculateSize(const vec2& _availlable)
{
	//EWOL_DEBUG("CalculateSize=" << availlable);
	// pop-up fill all the display :
	m_size = _availlable;
	vec2 padding = m_shaper.GetPadding();
	EWOL_DEBUG("our origin=" << m_origin << " size=" << m_size);
	if (NULL != m_subWidget) {
		vec2 subWidgetSize;
		vec2 subWidgetOrigin;
		subWidgetSize = m_subWidget->GetCalculateMinSize();
		if (true == m_subWidget->CanExpand().x()) {
			subWidgetSize.setX(m_size.x());
		}
		if (true == m_subWidget->CanExpand().y()) {
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
		EWOL_DEBUG("      ==> sub origin=" << subWidgetOrigin << " size=" << subWidgetSize);
		m_subWidget->SetOrigin(subWidgetOrigin);
		m_subWidget->CalculateSize(subWidgetSize);
	}
	MarkToRedraw();
}


void widget::ContextMenu::CalculateMinMaxSize(void)
{
	// call main class to calculate the min size...
	widget::Container::CalculateMinMaxSize();
	// add padding of the display
	m_minSize += m_shaper.GetPadding();
	//EWOL_DEBUG("CalculateMinSize=>>" << m_minSize);
	MarkToRedraw();
}


void widget::ContextMenu::OnDraw(void)
{
	m_compositing.Draw();
	m_shaper.Draw();
}


void widget::ContextMenu::OnRegenerateDisplay(void)
{
	// call upper class :
	widget::Container::OnRegenerateDisplay();
	if (true == NeedRedraw()) {
		m_compositing.Clear();
		m_shaper.Clear();
		vec2 padding = m_shaper.GetPadding();
		
		if (NULL != m_subWidget) {
			vec2 tmpSize = m_subWidget->GetSize();
			vec2 tmpOrigin = m_subWidget->GetOrigin();
			
			// display border ...
			m_compositing.SetColor(m_colorBorder);
			switch (m_arrawBorder)
			{
				case widget::CONTEXT_MENU_MARK_TOP:
					m_compositing.SetPos(vec3(m_arrowPos.x(), m_arrowPos.y(), 0.0f) );
					m_compositing.AddVertex();
					if (m_arrowPos.x() <= tmpOrigin.x() ) {
						float laking = m_offset - padding.y();
						m_compositing.SetPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()-laking, 0.0f) );
						m_compositing.AddVertex();
						m_compositing.SetPos(vec3(m_arrowPos.x(),        m_arrowPos.y()-laking, 0.0f) );
						m_compositing.AddVertex();
					} else {
						float laking = m_offset - padding.y();
						m_compositing.SetPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()-laking, 0.0f) );
						m_compositing.AddVertex();
						m_compositing.SetPos(vec3(m_arrowPos.x()-laking, m_arrowPos.y()-laking, 0.0f) );
						m_compositing.AddVertex();
					}
					break;
				case widget::CONTEXT_MENU_MARK_BOTTOM:
					m_compositing.SetPos(vec3(m_arrowPos.x(), m_arrowPos.y(), 0) );
					m_compositing.AddVertex();
					if (m_arrowPos.x() <= tmpOrigin.x() ) {
						int32_t laking = m_offset - padding.y();
						m_compositing.SetPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()+laking, 0.0f) );
						m_compositing.AddVertex();
						m_compositing.SetPos(vec3(m_arrowPos.x(),        m_arrowPos.y()+laking, 0.0f) );
						m_compositing.AddVertex();
					} else {
						int32_t laking = m_offset - padding.y();
						m_compositing.SetPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()+laking, 0.0f) );
						m_compositing.AddVertex();
						m_compositing.SetPos(vec3(m_arrowPos.x()-laking, m_arrowPos.y()+laking, 0.0f) );
						m_compositing.AddVertex();
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
			m_shaper.SetOrigin(vec2ClipInt32(shaperOrigin));
			m_shaper.SetSize(vec2ClipInt32(shaperSize));
			m_shaper.SetInsidePos(vec2ClipInt32(shaperOrigin+padding));
			m_shaper.SetInsideSize(vec2ClipInt32(shaperSize-padding*2.0f));
		}
	}
}
bool widget::ContextMenu::OnEventInput(const ewol::EventInput& _event)
{
	if (_event.GetId() > 0) {
		if (NULL != widget::Container::GetWidgetAtPos(_event.GetPos())) {
			return false;
		}
		if(    _event.GetStatus() == ewol::keyEvent::statusDown
		    || _event.GetStatus() == ewol::keyEvent::statusMove
		    || _event.GetStatus() == ewol::keyEvent::statusSingle
		    || _event.GetStatus() == ewol::keyEvent::statusUp
		    || _event.GetStatus() == ewol::keyEvent::statusEnter
		    || _event.GetStatus() == ewol::keyEvent::statusLeave ) {
			// Auto-remove ...
			AutoDestroy();
			return true;
		}
	}
	return false;
}


void widget::ContextMenu::SetPositionMark(markPosition_te position, vec2 arrowPos)
{
	EWOL_DEBUG("set context menu at the position : " << arrowPos);
	m_arrawBorder = position;
	m_arrowPos = arrowPos;
	MarkToRedraw();
}

ewol::Widget* widget::ContextMenu::GetWidgetAtPos(const vec2& pos)
{
	ewol::Widget* val = widget::Container::GetWidgetAtPos(pos);
	if (NULL != val) {
		return val;
	}
	return this;
}


bool widget::ContextMenu::OnSetConfig(const ewol::EConfig& _conf)
{
	if (true == widget::Container::OnSetConfig(_conf)) {
		return true;
	}
	if (_conf.GetConfig() == configArrowPosition) {
		m_arrowPos = _conf.GetData();
		return true;
	}
	if (_conf.GetConfig() == configArrowMode) {
		if(true == _conf.GetData().CompareNoCase("top")) {
			m_arrawBorder = CONTEXT_MENU_MARK_TOP;
		} else if(true == _conf.GetData().CompareNoCase("right")) {
			m_arrawBorder = CONTEXT_MENU_MARK_RIGHT;
		} else if(true == _conf.GetData().CompareNoCase("buttom")) {
			m_arrawBorder = CONTEXT_MENU_MARK_BOTTOM;
		} else if(true == _conf.GetData().CompareNoCase("left")) {
			m_arrawBorder = CONTEXT_MENU_MARK_LEFT;
		} else {
			m_arrawBorder = CONTEXT_MENU_MARK_NONE;
		}
		return true;
	}
	if (_conf.GetConfig() == configShaper) {
		SetShaperName(_conf.GetData());
		return true;
	}
	return false;
}

bool widget::ContextMenu::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == widget::Container::OnGetConfig(_config, _result)) {
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
		_result = m_shaper.GetSource();;
		return true;
	}
	return false;
}






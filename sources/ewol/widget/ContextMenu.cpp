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

#undef __class__
#define __class__	"ContextMenu"

widget::ContextMenu::ContextMenu(void)
{
	m_userExpand.setValue(false,false);
	
	m_padding.setValue(4,4);
	m_offset = 20;
	
	m_colorBackGroung = draw::color::white;
	
	m_colorBorder = draw::color::black;
	m_colorBorder.a = 0x7F;
	
	m_arrowPos.setValue(0,0);
	m_arrawBorder = widget::CONTEXT_MENU_MARK_TOP;
	SetMouseLimit(1);
}

widget::ContextMenu::~ContextMenu(void)
{
	
}


void widget::ContextMenu::CalculateSize(const vec2& availlable)
{
	//EWOL_DEBUG("CalculateSize=" << availlable);
	// pop-up fill all the display :
	m_size = availlable;
	
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
		switch (m_arrawBorder)
		{
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
		subWidgetOrigin.setX( (int32_t)(   etk_max(0, (subWidgetOrigin.x()-m_padding.x()*2))
		                                 + m_padding.x()*2) );
		subWidgetOrigin.setY( (int32_t)(   etk_max(0, (subWidgetOrigin.y()-m_padding.y()*2))
		                                 + m_padding.y()*2) );
		switch (m_arrawBorder)
		{
			default:
			case widget::CONTEXT_MENU_MARK_TOP:
			case widget::CONTEXT_MENU_MARK_BOTTOM:
				if (m_arrowPos.x() <= m_offset ) {
					subWidgetOrigin.setX(m_arrowPos.x()+m_padding.x());
				}
				break;
			case widget::CONTEXT_MENU_MARK_RIGHT:
			case widget::CONTEXT_MENU_MARK_LEFT:
				if (m_arrowPos.y() <= m_offset ) {
					subWidgetOrigin.setY(m_arrowPos.y()+m_padding.y());
				}
				break;
		}
		m_subWidget->SetOrigin(subWidgetOrigin);
		m_subWidget->CalculateSize(subWidgetSize);
	}
	MarkToRedraw();
}


void widget::ContextMenu::CalculateMinMaxSize(void)
{
	//EWOL_DEBUG("CalculateMinSize");
	m_userExpand.setValue(false,false);
	m_minSize.setValue(50,50);
	if (NULL != m_subWidget) {
		m_subWidget->CalculateMinMaxSize();
		m_minSize = m_subWidget->GetCalculateMinSize();
	}
	//EWOL_DEBUG("CalculateMinSize=>>" << m_minSize);
	MarkToRedraw();
}

void widget::ContextMenu::SystemDraw(const ewol::DrawProperty& _displayProp)
{
	ewol::Widget::SystemDraw(_displayProp);
	if (NULL != m_subWidget) {
		m_subWidget->SystemDraw(_displayProp);
	}
}


void widget::ContextMenu::OnDraw(void)
{
	m_compositing.Draw();
}


void widget::ContextMenu::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_compositing.Clear();
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
						float laking = m_offset - m_padding.y();
						m_compositing.SetPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()-laking, 0.0f) );
						m_compositing.AddVertex();
						m_compositing.SetPos(vec3(m_arrowPos.x(),        m_arrowPos.y()-laking, 0.0f) );
						m_compositing.AddVertex();
					} else {
						float laking = m_offset - m_padding.y();
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
						int32_t laking = m_offset - m_padding.y();
						m_compositing.SetPos(vec3(m_arrowPos.x()+laking, m_arrowPos.y()+laking, 0.0f) );
						m_compositing.AddVertex();
						m_compositing.SetPos(vec3(m_arrowPos.x(),        m_arrowPos.y()+laking, 0.0f) );
						m_compositing.AddVertex();
					} else {
						int32_t laking = m_offset - m_padding.y();
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
			m_compositing.SetPos(vec3(tmpOrigin.x()-m_padding.x(), tmpOrigin.y() - m_padding.y(), 0.0f) );
			m_compositing.RectangleWidth(vec3(tmpSize.x() + m_padding.x()*2, tmpSize.y() + m_padding.y()*2, 0.0f) );
			// set the area in white ...
			m_compositing.SetColor(m_colorBackGroung);
			m_compositing.SetPos(vec3(tmpOrigin.x(), tmpOrigin.y(), 0.0f) );
			m_compositing.RectangleWidth(vec3(tmpSize.x(), tmpSize.y(), 0.0f) );
		}
	}
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
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

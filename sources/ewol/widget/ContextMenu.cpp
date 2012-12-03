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
	m_userExpend.x = true;
	m_userExpend.y = true;
	
	m_padding.x = 4;
	m_padding.y = 4;
	m_offset = 20;

	m_colorBackGroung = draw::color::white;

	m_colorBorder = draw::color::black;
	m_colorBorder.a = 0x7F;
	
	m_arrowPos.x = 0;
	m_arrowPos.y = 0;
	m_arrawBorder = widget::CONTEXT_MENU_MARK_TOP;
	SetMouseLimit(1);
}

widget::ContextMenu::~ContextMenu(void)
{
	SubWidgetRemove();
}


bool widget::ContextMenu::CalculateSize(float availlableX, float availlableY)
{
	EWOL_DEBUG("CalculateSize(" << availlableX << "," << availlableY << ")");
	// pop-up fill all the display :
	m_size.x = availlableX;
	m_size.y = availlableY;
	
	if (NULL != m_subWidget) {
		vec2 subWidgetSize;
		vec2 subWidgetOrigin;
		subWidgetSize = m_subWidget->GetMinSize();
		if (true == m_subWidget->CanExpentX()) {
			subWidgetSize.x = m_size.x;
		}
		if (true == m_subWidget->CanExpentY()) {
			subWidgetSize.y = m_size.y;
		}
		int32_t minWidth = 100;
		int32_t maxWidth = 300;
		subWidgetSize.x = (int32_t)etk_max(minWidth, subWidgetSize.x);
		subWidgetSize.x = (int32_t)etk_min(maxWidth, subWidgetSize.x);
		subWidgetSize.y = (int32_t)subWidgetSize.y;
		
		// set config to the Sub-widget
		switch (m_arrawBorder)
		{
			case widget::CONTEXT_MENU_MARK_TOP:
				subWidgetOrigin.x = (int32_t)(m_arrowPos.x - subWidgetSize.x/2);
				subWidgetOrigin.y = (int32_t)(m_arrowPos.y - m_offset - subWidgetSize.y);
				break;
			case widget::CONTEXT_MENU_MARK_BOTTOM:
				subWidgetOrigin.x = (int32_t)(m_arrowPos.x - subWidgetSize.x/2);
				subWidgetOrigin.y = (int32_t)(m_arrowPos.y + m_offset);
				break;
			case widget::CONTEXT_MENU_MARK_RIGHT:
			case widget::CONTEXT_MENU_MARK_LEFT:
			default:
				subWidgetOrigin.x = (int32_t)(m_size.x - m_origin.x - subWidgetSize.x)/2 + m_origin.x;
				subWidgetOrigin.y = (int32_t)(m_size.y - m_origin.y - subWidgetSize.y)/2 + m_origin.y;
				break;
		}
		// set the widget position at the border of the screen
		subWidgetOrigin.x -= m_padding.x*2;
		subWidgetOrigin.x  = etk_max(0, subWidgetOrigin.x);
		subWidgetOrigin.x += m_padding.x*2;
		subWidgetOrigin.x  = (int32_t)subWidgetOrigin.x;
		
		subWidgetOrigin.y -= m_padding.y*2;
		subWidgetOrigin.y  = etk_max(0, subWidgetOrigin.y);
		subWidgetOrigin.y += m_padding.y*2;
		subWidgetOrigin.y  = (int32_t)subWidgetOrigin.y;
		switch (m_arrawBorder)
		{
			default:
			case widget::CONTEXT_MENU_MARK_TOP:
			case widget::CONTEXT_MENU_MARK_BOTTOM:
				if (m_arrowPos.x <= m_offset ) {
					subWidgetOrigin.x = m_arrowPos.x+m_padding.x;
				}
				break;
			case widget::CONTEXT_MENU_MARK_RIGHT:
			case widget::CONTEXT_MENU_MARK_LEFT:
				if (m_arrowPos.y <= m_offset ) {
					subWidgetOrigin.y = m_arrowPos.y+m_padding.y;
				}
				break;
		}
		m_subWidget->SetOrigin(subWidgetOrigin.x, subWidgetOrigin.y);
		m_subWidget->CalculateSize(subWidgetSize.x, subWidgetSize.y);
	}
	MarkToRedraw();
	return true;
}


bool widget::ContextMenu::CalculateMinSize(void)
{
	EWOL_DEBUG("CalculateMinSize");
	m_userExpend.x=false;
	m_userExpend.y=false;
	m_minSize.x = 50.0;
	m_minSize.y = 50.0;
	if (NULL != m_subWidget) {
		m_subWidget->CalculateMinSize();
		vec2 tmpSize = m_subWidget->GetMinSize();
		m_minSize.x = tmpSize.x;
		m_minSize.y = tmpSize.y;
	}
	EWOL_DEBUG("CalculateMinSize(" << m_minSize.x << "," << m_minSize.y << ")");
	MarkToRedraw();
	return true;
}

void widget::ContextMenu::SetMinSise(float x, float y)
{
	EWOL_ERROR("Pop-up can not have a user Minimum size (herited from under elements)");
}

void widget::ContextMenu::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings X (herited from under elements)");
}

void widget::ContextMenu::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings Y (herited from under elements)");
}


void widget::ContextMenu::SubWidgetSet(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget = newWidget;
}


void widget::ContextMenu::SubWidgetRemove(void)
{
	if (NULL != m_subWidget) {
		delete(m_subWidget);
		m_subWidget = NULL;
	}
}

void widget::ContextMenu::OnDraw(ewol::DrawProperty& displayProp)
{
	//EWOL_DEBUG("On Draw " << m_currentDrawId);
	widget::Drawable::OnDraw(displayProp);
	if (NULL != m_subWidget) {
		m_subWidget->GenDraw(displayProp);
	}
}


void widget::ContextMenu::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
	}
	// generate a white background and take gray on other surfaces
	ClearOObjectList();
	ewol::Drawing * BGOObjects = new ewol::Drawing();
	AddOObject(BGOObjects);
	
	if (NULL != m_subWidget) {
		vec2 tmpSize = m_subWidget->GetSize();
		vec2 tmpOrigin = m_subWidget->GetOrigin();
		
		// display border ...
		BGOObjects->SetColor(m_colorBorder);
		switch (m_arrawBorder)
		{
			case widget::CONTEXT_MENU_MARK_TOP:
				BGOObjects->SetPos(vec3(m_arrowPos.x, m_arrowPos.y, 0.0f) );
				BGOObjects->AddVertex();
				if (m_arrowPos.x <= tmpOrigin.x ) {
					float laking = m_offset - m_padding.y;
					BGOObjects->SetPos(vec3(m_arrowPos.x+laking, m_arrowPos.y-laking, 0.0f) );
					BGOObjects->AddVertex();
					BGOObjects->SetPos(vec3(m_arrowPos.x,        m_arrowPos.y-laking, 0.0f) );
					BGOObjects->AddVertex();
				} else {
					float laking = m_offset - m_padding.y;
					BGOObjects->SetPos(vec3(m_arrowPos.x+laking, m_arrowPos.y-laking, 0.0f) );
					BGOObjects->AddVertex();
					BGOObjects->SetPos(vec3(m_arrowPos.x-laking, m_arrowPos.y-laking, 0.0f) );
					BGOObjects->AddVertex();
				}
				break;
			case widget::CONTEXT_MENU_MARK_BOTTOM:
				BGOObjects->SetPos(vec3(m_arrowPos.x, m_arrowPos.y, 0.0f) );
				BGOObjects->AddVertex();
				if (m_arrowPos.x <= tmpOrigin.x ) {
					int32_t laking = m_offset - m_padding.y;
					BGOObjects->SetPos(vec3(m_arrowPos.x+laking, m_arrowPos.y+laking, 0.0f) );
					BGOObjects->AddVertex();
					BGOObjects->SetPos(vec3(m_arrowPos.x,        m_arrowPos.y+laking, 0.0f) );
					BGOObjects->AddVertex();
				} else {
					int32_t laking = m_offset - m_padding.y;
					BGOObjects->SetPos(vec3(m_arrowPos.x+laking, m_arrowPos.y+laking, 0.0f) );
					BGOObjects->AddVertex();
					BGOObjects->SetPos(vec3(m_arrowPos.x-laking, m_arrowPos.y+laking, 0.0f) );
					BGOObjects->AddVertex();
				}
				break;
			default:
			case widget::CONTEXT_MENU_MARK_RIGHT:
			case widget::CONTEXT_MENU_MARK_LEFT:
				EWOL_TODO("later");
				break;
		}
		BGOObjects->SetPos(vec3(tmpOrigin.x-m_padding.x, tmpOrigin.y - m_padding.y, 0.0f) );
		BGOObjects->RectangleWidth(vec3(tmpSize.x + m_padding.x*2, tmpSize.y + m_padding.y*2, 0.0f) );
		// set the area in white ...
		BGOObjects->SetColor(m_colorBackGroung);
		BGOObjects->SetPos(vec3(tmpOrigin.x, tmpOrigin.y, 0.0f) );
		BGOObjects->RectangleWidth(vec3(tmpSize.x, tmpSize.y, 0.0f) );
	}
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
}


ewol::Widget * widget::ContextMenu::GetWidgetAtPos(vec2 pos)
{
	// calculate relative position
	vec2 relativePos = RelativePosition(pos);
	// Check for sub Element
	if (NULL != m_subWidget) {
		vec2 tmpSize = m_subWidget->GetSize();
		vec2 tmpOrigin = m_subWidget->GetOrigin();
		if(    (tmpOrigin.x <= relativePos.x && tmpOrigin.x + tmpSize.x >= relativePos.x)
		    && (tmpOrigin.y <= relativePos.y && tmpOrigin.y + tmpSize.y >= relativePos.y) )
		{
			return m_subWidget->GetWidgetAtPos(pos);
		}
	}
	return this;
}


bool widget::ContextMenu::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos)
{
	//EWOL_INFO("Event ouside the context menu");
	if (IdInput > 0) {
		if(    typeEvent == ewol::keyEvent::statusDown
		    || typeEvent == ewol::keyEvent::statusMove
		    || typeEvent == ewol::keyEvent::statusSingle
		    || typeEvent == ewol::keyEvent::statusUp
		    || typeEvent == ewol::keyEvent::statusEnter
		    || typeEvent == ewol::keyEvent::statusLeave ) {
			// Auto-remove ...
			AutoDestroy();
			return true;
		}
	}
	return false;
}


void widget::ContextMenu::SetPositionMark(markPosition_te position, vec2 arrowPos)
{
	EWOL_DEBUG("set context menu at the position : (" << arrowPos.x << "," << arrowPos.y << ")");
	m_arrawBorder = position;
	m_arrowPos = arrowPos;
	MarkToRedraw();
}


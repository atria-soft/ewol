/**
 *******************************************************************************
 * @file ewol/widget/ContextMenu.cpp
 * @brief ewol Contextual Menu widget system (Sources)
 * @author Edouard DUPIN
 * @date 16/02/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */



#include <ewol/ewol.h>
#include <ewol/WidgetManager.h>
#include <ewol/widget/ContextMenu.h>

#undef __class__
#define __class__	"ContextMenu"

ewol::ContextMenu::ContextMenu(void)
{
	m_userExpendX = true;
	m_userExpendY = true;
	
	m_padding.x = 4;
	m_padding.y = 4;
	m_offset = 20;

	m_colorBackGroung.red   = 1.0;
	m_colorBackGroung.green = 1.0;
	m_colorBackGroung.blue  = 1.0;
	m_colorBackGroung.alpha = 1.0;

	m_colorBorder.red   = 0.0;
	m_colorBorder.green = 0.0;
	m_colorBorder.blue  = 0.0;
	m_colorBorder.alpha = 0.50;
	
	m_arrowPos.x = 0;
	m_arrowPos.y = 0;
	m_arrawBorder = ewol::CONTEXT_MENU_MARK_TOP;
}

ewol::ContextMenu::~ContextMenu(void)
{
	SubWidgetRemove();
}


bool ewol::ContextMenu::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	EWOL_DEBUG("CalculateSize(" << availlableX << "," << availlableY << ")");
	// pop-up fill all the display :
	m_size.x = availlableX;
	m_size.y = availlableY;
	
	if (NULL != m_subWidget[m_currentCreateId]) {
		coord2D_ts subWidgetSize;
		coord2D_ts subWidgetOrigin;
		subWidgetSize = m_subWidget[m_currentCreateId]->GetMinSize();
		if (true == m_subWidget[m_currentCreateId]->CanExpentX()) {
			subWidgetSize.x = m_size.x;
		}
		if (true == m_subWidget[m_currentCreateId]->CanExpentY()) {
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
			case ewol::CONTEXT_MENU_MARK_TOP:
				subWidgetOrigin.x = (int32_t)(m_arrowPos.x - subWidgetSize.x/2);
				subWidgetOrigin.y = (int32_t)(m_arrowPos.y + m_offset);
				break;
			case ewol::CONTEXT_MENU_MARK_BOTTOM:
				subWidgetOrigin.x = (int32_t)(m_arrowPos.x - subWidgetSize.x/2);
				subWidgetOrigin.y = (int32_t)(m_arrowPos.y - m_offset - subWidgetSize.y);
				break;
			case ewol::CONTEXT_MENU_MARK_RIGHT:
			case ewol::CONTEXT_MENU_MARK_LEFT:
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
			case ewol::CONTEXT_MENU_MARK_TOP:
			case ewol::CONTEXT_MENU_MARK_BOTTOM:
				if (m_arrowPos.x <= m_offset ) {
					subWidgetOrigin.x = m_arrowPos.x+m_padding.x;
				}
				break;
			case ewol::CONTEXT_MENU_MARK_RIGHT:
			case ewol::CONTEXT_MENU_MARK_LEFT:
				if (m_arrowPos.y <= m_offset ) {
					subWidgetOrigin.y = m_arrowPos.y+m_padding.y;
				}
				break;
		}
		m_subWidget[m_currentCreateId]->SetOrigin(subWidgetOrigin.x, subWidgetOrigin.y);
		m_subWidget[m_currentCreateId]->CalculateSize(subWidgetSize.x, subWidgetSize.y);
	}
	MarkToReedraw();
	return true;
}


bool ewol::ContextMenu::CalculateMinSize(void)
{
	EWOL_DEBUG("CalculateMinSize");
	m_userExpendX=false;
	m_userExpendY=false;
	m_minSize.x = 50.0;
	m_minSize.y = 50.0;
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->CalculateMinSize();
		coord2D_ts tmpSize = m_subWidget[m_currentCreateId]->GetMinSize();
		m_minSize.x = tmpSize.x;
		m_minSize.y = tmpSize.y;
	}
	EWOL_DEBUG("CalculateMinSize(" << m_minSize.x << "," << m_minSize.y << ")");
	MarkToReedraw();
	return true;
}

void ewol::ContextMenu::SetMinSise(etkFloat_t x, etkFloat_t y)
{
	EWOL_ERROR("Pop-up can not have a user Minimum size (herited from under elements)");
}

void ewol::ContextMenu::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings X (herited from under elements)");
}

void ewol::ContextMenu::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings Y (herited from under elements)");
}


void ewol::ContextMenu::SubWidgetSet(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget[m_currentCreateId] = newWidget;
}


void ewol::ContextMenu::SubWidgetRemove(void)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->MarkToRemove();
		m_subWidget[m_currentCreateId] = NULL;
	}
}

bool ewol::ContextMenu::OnDraw(void)
{
	//EWOL_DEBUG("On Draw " << m_currentDrawId);
	ewol::Drawable::OnDraw();
	if (NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->GenDraw();
	}
	return true;
}


void ewol::ContextMenu::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
	}
	// generate a white background and take gray on other surfaces
	ClearOObjectList();
	ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
	AddOObject(BGOObjects);
	
	if (NULL != m_subWidget[m_currentCreateId]) {
		coord2D_ts tmpSize = m_subWidget[m_currentCreateId]->GetSize();
		coord2D_ts tmpOrigin = m_subWidget[m_currentCreateId]->GetOrigin();
		
		// display border ...
		BGOObjects->SetColor(m_colorBorder);
		switch (m_arrawBorder)
		{
			case ewol::CONTEXT_MENU_MARK_TOP:
				BGOObjects->SetPoint(m_arrowPos.x, m_arrowPos.y);
				if (m_arrowPos.x <= tmpOrigin.x ) {
					int32_t laking = m_offset - m_padding.y;
					BGOObjects->SetPoint(m_arrowPos.x+laking, m_arrowPos.y+laking);
					BGOObjects->SetPoint(m_arrowPos.x,        m_arrowPos.y+laking);
				} else {
					int32_t laking = m_offset - m_padding.y;
					BGOObjects->SetPoint(m_arrowPos.x+laking, m_arrowPos.y+laking);
					BGOObjects->SetPoint(m_arrowPos.x-laking, m_arrowPos.y+laking);
				}
				break;
			case ewol::CONTEXT_MENU_MARK_BOTTOM:
				BGOObjects->SetPoint(m_arrowPos.x, m_arrowPos.y);
				if (m_arrowPos.x <= tmpOrigin.x ) {
					int32_t laking = m_offset - m_padding.y;
					BGOObjects->SetPoint(m_arrowPos.x+laking, m_arrowPos.y-laking);
					BGOObjects->SetPoint(m_arrowPos.x,        m_arrowPos.y-laking);
				} else {
					int32_t laking = m_offset - m_padding.y;
					BGOObjects->SetPoint(m_arrowPos.x+laking, m_arrowPos.y-laking);
					BGOObjects->SetPoint(m_arrowPos.x-laking, m_arrowPos.y-laking);
				}
				break;
				break;
			default:
			case ewol::CONTEXT_MENU_MARK_RIGHT:
			case ewol::CONTEXT_MENU_MARK_LEFT:
				EWOL_TODO("later");
				break;
		}
		
		BGOObjects->Rectangle(tmpOrigin.x-m_padding.x, tmpOrigin.y - m_padding.y, tmpSize.x + m_padding.x*2, tmpSize.y + m_padding.y*2);
		// set the area in white ...
		BGOObjects->SetColor(m_colorBackGroung);
		BGOObjects->Rectangle(tmpOrigin.x, tmpOrigin.y, tmpSize.x, tmpSize.y);
	}
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->OnRegenerateDisplay();
	}
}


bool ewol::ContextMenu::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		coord2D_ts tmpSize = m_subWidget[m_currentCreateId]->GetSize();
		coord2D_ts tmpOrigin = m_subWidget[m_currentCreateId]->GetOrigin();
		if(    (tmpOrigin.x <= pos.local.x && tmpOrigin.x + tmpSize.x >= pos.local.x)
		    && (tmpOrigin.y <= pos.local.y && tmpOrigin.y + tmpSize.y >= pos.local.y) )
		{
			return m_subWidget[m_currentCreateId]->GenEventInput(IdInput, typeEvent, pos.abs);
		} else {
			//EWOL_INFO("Event ouside the context menu");
			if (IdInput > 0) {
				if(    typeEvent == ewol::EVENT_INPUT_TYPE_DOWN
				    || typeEvent == ewol::EVENT_INPUT_TYPE_MOVE
				    || typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE
				    || typeEvent == ewol::EVENT_INPUT_TYPE_DOUBLE
				    || typeEvent == ewol::EVENT_INPUT_TYPE_TRIPLE
				    || typeEvent == ewol::EVENT_INPUT_TYPE_UP
				    || typeEvent == ewol::EVENT_INPUT_TYPE_ENTER
				    || typeEvent == ewol::EVENT_INPUT_TYPE_LEAVE ) {
					// Auto-remove ...
					MarkToRemove();
				}
			}
		}
	}
	return true;
}


void ewol::ContextMenu::SetPositionMark(markPosition_te position, coord2D_ts arrowPos)
{
	EWOL_DEBUG("set context menu at the position : (" << arrowPos.x << "," << arrowPos.y << ")");
	m_arrawBorder = position;
	m_arrowPos = arrowPos;
	MarkToReedraw();
}

void ewol::ContextMenu::OnFlipFlopEvent(void)
{
	//EWOL_DEBUG("Flip-Flop");
	bool needFlipFlop = m_needFlipFlop;
	// call herited classes
	ewol::Drawable::OnFlipFlopEvent();
	// internal saving
	if (true == needFlipFlop) {
		m_subWidget[m_currentCreateId] = m_subWidget[m_currentDrawId];
	}
	// in every case, we propagate the flip-flop EVENT
	if (NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->OnFlipFlopEvent();
	}
}
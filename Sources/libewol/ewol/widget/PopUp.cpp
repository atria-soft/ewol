/**
 *******************************************************************************
 * @file ewol/widget/PopUp.cpp
 * @brief ewol pop-up widget system (Sources)
 * @author Edouard DUPIN
 * @date 29/12/2011
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



#include <ewol/widget/PopUp.h>

#undef __class__
#define __class__	"ewol::PopUp"

ewol::PopUp::PopUp(void)
{
	//GenericDrawDisable();
	SpecificDrawEnable();
	m_userExpendX = true;
	m_userExpendY = true;

	m_colorBackGroung.red   = 1.0;
	m_colorBackGroung.green = 1.0;
	m_colorBackGroung.blue  = 1.0;
	m_colorBackGroung.alpha = 1.0;

	m_colorEmptyArea.red   = 0.0;
	m_colorEmptyArea.green = 0.0;
	m_colorEmptyArea.blue  = 0.0;
	m_colorEmptyArea.alpha = 0.50;
}

ewol::PopUp::~PopUp(void)
{
	SubWidgetRemove();
}


bool ewol::PopUp::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	//EWOL_DEBUG("CalculateSize(" << availlableX << "," << availlableY << ")");
	// pop-up fill all the display :
	m_size.x = availlableX;
	m_size.y = availlableY;
	
	if (NULL != m_subWidget) {
		coord         subWidgetSize;
		coord         subWidgetOrigin;
		subWidgetSize = m_subWidget->GetMinSize();
		if (true == m_subWidget->CanExpentX()) {
			subWidgetSize.x = m_size.x;
		}
		if (true == m_subWidget->CanExpentY()) {
			subWidgetSize.y = m_size.y;
		}
		subWidgetOrigin.x = (int32_t)(m_size.x - m_origin.x - subWidgetSize.x)/2 + m_origin.x;
		subWidgetOrigin.y = (int32_t)(m_size.y - m_origin.y - subWidgetSize.y)/2 + m_origin.y;
		
		m_subWidget->SetOrigin(subWidgetOrigin.x, subWidgetOrigin.y);
		m_subWidget->CalculateSize(subWidgetSize.x, subWidgetSize.y);
	}
	return true;
}


bool ewol::PopUp::CalculateMinSize(void)
{
	m_userExpendX=false;
	m_userExpendY=false;
	m_minSize.x = 50.0;
	m_minSize.y = 50.0;
	if (NULL != m_subWidget) {
		m_subWidget->CalculateMinSize();
		coord tmpSize = m_subWidget->GetMinSize();
		m_minSize.x = tmpSize.x;
		m_minSize.y = tmpSize.y;
	}
	//EWOL_DEBUG("CalculateMinSize(" << m_minSize.x << "," << m_minSize.y << ")");
	return true;
}

void ewol::PopUp::SetMinSise(etkFloat_t x, etkFloat_t y)
{
	EWOL_ERROR("Pop-up can not have a user Minimum size (herited from under elements)");
}

void ewol::PopUp::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings X (herited from under elements)");
}

void ewol::PopUp::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings Y (herited from under elements)");
}


void ewol::PopUp::SubWidgetSet(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget = newWidget;
	newWidget->SetParrent(this);
}


void ewol::PopUp::SubWidgetRemove(void)
{
	if (NULL != m_subWidget) {
		delete(m_subWidget);
		m_subWidget = NULL;
	}
}

bool ewol::PopUp::OnDraw(void)
{
	if (NULL != m_subWidget) {
		m_subWidget->GenDraw();
	}
	return true;
}


void ewol::PopUp::OnRegenerateDisplay(void)
{
	// generate a white background and take gray on other surfaces
	ClearOObjectList();
	ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
	AddOObject(BGOObjects, "ListDeco");

	BGOObjects->SetColor(m_colorEmptyArea);
	BGOObjects->Rectangle(0, 0, m_size.x, m_size.y);
	// set the area in white ...
	if (NULL != m_subWidget) {
		coord tmpSize = m_subWidget->GetSize();
		coord tmpOrigin = m_subWidget->GetOrigin();
		BGOObjects->SetColor(m_colorBackGroung);
		BGOObjects->Rectangle(tmpOrigin.x, tmpOrigin.y, tmpSize.x, tmpSize.y);
	}
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
}


bool ewol::PopUp::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	if (NULL != m_subWidget) {
		coord tmpSize = m_subWidget->GetSize();
		coord tmpOrigin = m_subWidget->GetOrigin();
		if(    (tmpOrigin.x <= x && tmpOrigin.x + tmpSize.x >= x)
		    && (tmpOrigin.y <= y && tmpOrigin.y + tmpSize.y >= y) )
		{
			return m_subWidget->GenEventInput(IdInput, typeEvent, x, y);
		} else {
			//EWOL_INFO("Event ouside the Pop-up");
		}
		
	}
	return true;
}



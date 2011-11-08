/**
 *******************************************************************************
 * @file ewolSizerHori.cpp
 * @brief ewol hirisantal sizer widget system (Sources)
 * @author Edouard DUPIN
 * @date 07/11/2011
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



#include <widget/ewolSizerHori.h>

#undef __class__
#define __class__	"ewol::SizerHori"

ewol::SizerHori::SizerHori(void)
{
	GenericDrawDisable();
	SpecificDrawEnable();
}

ewol::SizerHori::~SizerHori(void)
{
	SubWidgetRemoveAll();
}


bool ewol::SizerHori::CalculateSize(double availlableX, double availlableY)
{
	EWOL_DEBUG("Update Size");
	m_size.x = availlableX;
	m_size.y = availlableY;
	// calculate unExpendable Size :
	double unexpendableSize=0.0;
	int32_t nbWidgetFixedSize=0;
	int32_t nbWidgetNotFixedSize=0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			coord tmpSize = m_subWidget[iii]->GetMinSize();
			unexpendableSize += tmpSize.x;
			if (false == m_subWidget[iii]->CanExpentX()) {
				nbWidgetFixedSize++;
			} else {
				nbWidgetNotFixedSize++;
			}
		}
	}
	double sizeToAddAtEveryOne = 0;
	// 2 cases : 1 or more can Expend, or all is done ...
	if (0 != nbWidgetNotFixedSize) {
		sizeToAddAtEveryOne = (m_size.x - unexpendableSize) / nbWidgetNotFixedSize;
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	coord tmpOrigin;
	tmpOrigin.x = m_origin.x;
	tmpOrigin.y = m_origin.y;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			coord tmpSize = m_subWidget[iii]->GetMinSize();
			// Set the origin :
			EWOL_DEBUG("Set ORIGIN : " << tmpOrigin.x << "," << tmpOrigin.y << ")");
			m_subWidget[iii]->SetOrigin(tmpOrigin.x, tmpOrigin.y);
			// Now Update his Size  his size in X and the curent sizer size in Y:
			if (true == m_subWidget[iii]->CanExpentX()) {
				m_subWidget[iii]->CalculateSize(tmpSize.x+sizeToAddAtEveryOne, m_size.y);
				tmpOrigin.x += tmpSize.x+sizeToAddAtEveryOne;
			} else {
				m_subWidget[iii]->CalculateSize(tmpSize.x, m_size.y);
				tmpOrigin.x += tmpSize.x;
			}
		}
	}
	return true;
}


bool ewol::SizerHori::CalculateMinSize(void)
{
	EWOL_DEBUG("Update minimum Size");
	m_userExpendX=false;
	m_userExpendY=false;
	m_minSize.x = 0.0;
	m_minSize.y = 0.0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinSize();
			if (true == m_subWidget[iii]->CanExpentX()) {
				m_userExpendX = true;
			}
			if (true == m_subWidget[iii]->CanExpentY()) {
				m_userExpendY = true;
			}
			coord tmpSize = m_subWidget[iii]->GetMinSize();
			m_minSize.x += tmpSize.x;
			if (tmpSize.y>m_minSize.y) {
				m_minSize.y = tmpSize.y;
			}
		}
	}
	EWOL_DEBUG("Result : expend  X="<< m_userExpendX << " Y=" << m_userExpendY);
	EWOL_DEBUG("         minSize ("<< m_minSize.x << "," << m_minSize.y << ")");
	return true;
}

void ewol::SizerHori::SetMinSise(double x, double y)
{
	EWOL_ERROR("Sizer can not have a user Minimum size (herited from under elements)");
}

void ewol::SizerHori::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings X (herited from under elements)");
}

void ewol::SizerHori::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings Y (herited from under elements)");
}


//etk::VectorType<ewol::Widget*> m_SubWidget;

void ewol::SizerHori::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			delete(m_subWidget[iii]);
			m_subWidget[iii] = NULL;
		}
	}
	m_subWidget.Clear();
}


void ewol::SizerHori::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
	newWidget->SetParrent(this);
}


void ewol::SizerHori::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			if (NULL != m_subWidget[iii]) {
				delete(m_subWidget[iii]);
				m_subWidget[iii] = NULL;
			}
			m_subWidget.Erase(iii);
			return;
		}
	}
}

void ewol::SizerHori::SubWidgetUnLink(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			return;
		}
	}
}


bool ewol::SizerHori::OnDraw(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw();
		}
	}
	return true;
}


void ewol::SizerHori::OnRegenerateDisplay(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
	}
}



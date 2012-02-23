/**
 *******************************************************************************
 * @file ewol/widget/SizerHori.cpp
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



#include <ewol/widget/SizerHori.h>
#include <ewol/WidgetManager.h>

#undef __class__
#define __class__	"ewol::SizerHori"

ewol::SizerHori::SizerHori(void)
{
	// set contamination enable
	LockExpendContamination();
}

ewol::SizerHori::~SizerHori(void)
{
	SubWidgetRemoveAll();
}


bool ewol::SizerHori::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	//EWOL_DEBUG("Update Size");
	m_size.x = availlableX;
	m_size.y = availlableY;
	// calculate unExpendable Size :
	etkFloat_t unexpendableSize=0.0;
	int32_t nbWidgetFixedSize=0;
	int32_t nbWidgetNotFixedSize=0;
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			coord2D_ts tmpSize = m_subWidget[m_currentCreateId][iii]->GetMinSize();
			unexpendableSize += tmpSize.x;
			if (false == m_subWidget[m_currentCreateId][iii]->CanExpentX()) {
				nbWidgetFixedSize++;
			} else {
				nbWidgetNotFixedSize++;
			}
		}
	}
	etkFloat_t sizeToAddAtEveryOne = 0;
	// 2 cases : 1 or more can Expend, or all is done ...
	if (0 != nbWidgetNotFixedSize) {
		sizeToAddAtEveryOne = (m_size.x - unexpendableSize) / nbWidgetNotFixedSize;
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	coord2D_ts tmpOrigin;
	tmpOrigin.x = m_origin.x;
	tmpOrigin.y = m_origin.y;
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			coord2D_ts tmpSize = m_subWidget[m_currentCreateId][iii]->GetMinSize();
			// Set the origin :
			//EWOL_DEBUG("Set ORIGIN : " << tmpOrigin.x << "," << tmpOrigin.y << ")");
			m_subWidget[m_currentCreateId][iii]->SetOrigin(tmpOrigin.x, tmpOrigin.y);
			// Now Update his Size  his size in X and the curent sizer size in Y:
			if (true == m_subWidget[m_currentCreateId][iii]->CanExpentX()) {
				m_subWidget[m_currentCreateId][iii]->CalculateSize(tmpSize.x+sizeToAddAtEveryOne, m_size.y);
				tmpOrigin.x += tmpSize.x+sizeToAddAtEveryOne;
			} else {
				m_subWidget[m_currentCreateId][iii]->CalculateSize(tmpSize.x, m_size.y);
				tmpOrigin.x += tmpSize.x;
			}
		}
	}
	MarkToReedraw();
	return true;
}


bool ewol::SizerHori::CalculateMinSize(void)
{
	//EWOL_DEBUG("Update minimum Size");
	m_userExpendX=false;
	m_userExpendY=false;
	m_minSize.x = 0.0;
	m_minSize.y = 0.0;
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii]->CalculateMinSize();
			if (true == m_subWidget[m_currentCreateId][iii]->CanExpentX()) {
				m_userExpendX = true;
			}
			if (true == m_subWidget[m_currentCreateId][iii]->CanExpentY()) {
				m_userExpendY = true;
			}
			coord2D_ts tmpSize = m_subWidget[m_currentCreateId][iii]->GetMinSize();
			m_minSize.x += tmpSize.x;
			if (tmpSize.y>m_minSize.y) {
				m_minSize.y = tmpSize.y;
			}
		}
	}
	//EWOL_DEBUG("Result : expend  X="<< m_userExpendX << " Y=" << m_userExpendY);
	//EWOL_DEBUG("         minSize ("<< m_minSize.x << "," << m_minSize.y << ")");
	return true;
}

void ewol::SizerHori::SetMinSise(etkFloat_t x, etkFloat_t y)
{
	EWOL_ERROR("Sizer can not have a user Minimum size (herited from under elements)");
}

void ewol::SizerHori::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings X (herited from under elements)");
}

bool ewol::SizerHori::CanExpentX(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpendX;
}

void ewol::SizerHori::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings Y (herited from under elements)");
}

bool ewol::SizerHori::CanExpentY(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpendY;
}

void ewol::SizerHori::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination = lockExpend;
}

//etk::VectorType<ewol::Widget*> m_subWidget[m_currentCreateId];

void ewol::SizerHori::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			ewol::widgetManager::MarkWidgetToBeRemoved(m_subWidget[m_currentCreateId][iii]);
			m_subWidget[m_currentCreateId][iii] = NULL;
		}
	}
	m_subWidget[m_currentCreateId].Clear();
}


void ewol::SizerHori::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget[m_currentCreateId].PushBack(newWidget);
	newWidget->SetParrent(this);
}


void ewol::SizerHori::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (newWidget == m_subWidget[m_currentCreateId][iii]) {
			if (NULL != m_subWidget[m_currentCreateId][iii]) {
				ewol::widgetManager::MarkWidgetToBeRemoved(m_subWidget[m_currentCreateId][iii]);
				m_subWidget[m_currentCreateId][iii] = NULL;
			}
			m_subWidget[m_currentCreateId].Erase(iii);
			return;
		}
	}
}

void ewol::SizerHori::SubWidgetUnLink(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (newWidget == m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii] = NULL;
			m_subWidget[m_currentCreateId].Erase(iii);
			return;
		}
	}
}


bool ewol::SizerHori::OnDraw(void)
{
	for (int32_t iii=0; iii<m_subWidget[m_currentDrawId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentDrawId][iii]) {
			m_subWidget[m_currentDrawId][iii]->GenDraw();
		}
	}
	return true;
}


void ewol::SizerHori::OnRegenerateDisplay(void)
{
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii]->OnRegenerateDisplay();
		}
	}
}


bool ewol::SizerHori::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos)
{
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			coord2D_ts tmpSize = m_subWidget[m_currentCreateId][iii]->GetSize();
			coord2D_ts tmpOrigin = m_subWidget[m_currentCreateId][iii]->GetOrigin();
			if(    (tmpOrigin.x <= pos.abs.x && tmpOrigin.x + tmpSize.x >= pos.abs.x)
			    && (tmpOrigin.y <= pos.abs.y && tmpOrigin.y + tmpSize.y >= pos.abs.y) )
			{
				return m_subWidget[m_currentCreateId][iii]->GenEventInput(IdInput, typeEvent, pos.abs);
			}
		}
	}
	return true;
}


void ewol::SizerHori::OnFlipFlopEvent(void)
{
	// keep in the current element all the modification done ...
	m_subWidget[m_currentCreateId] = m_subWidget[m_currentDrawId];
}



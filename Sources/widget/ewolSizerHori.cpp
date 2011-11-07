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


ewol::SizerHori::SizerHori(void)
{
	
}

ewol::SizerHori::~SizerHori(void)
{
	SubWidgetRemoveAll();
}


bool ewol::SizerHori::CalculateSize(double availlableX, double availlableY)
{
	EWOL_DEBUG("Update Size");
}




//etk::VectorType<ewol::Widget*> m_SubWidget;

void ewol::SizerHori::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_SubWidget.Size(); iii++) {
		delete(m_SubWidget[iii]);
		m_SubWidget[iii] = NULL;
	}
	m_SubWidget.Clear();
}


void ewol::SizerHori::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_SubWidget.PushBack(newWidget);
	newWidget->SetParrent(this);
}


void ewol::SizerHori::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_SubWidget.Size(); iii++) {
		if (newWidget == m_SubWidget[iii]) {
			delete(m_SubWidget[iii]);
			m_SubWidget[iii] = NULL;
			m_SubWidget.Erase(iii);
			return;
		}
	}
}

void ewol::SizerHori::SubWidgetUnLink(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_SubWidget.Size(); iii++) {
		if (newWidget == m_SubWidget[iii]) {
			m_SubWidget[iii] = NULL;
			m_SubWidget.Erase(iii);
			return;
		}
	}
}
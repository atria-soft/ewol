/**
 *******************************************************************************
 * @file ewol/widget/List.cpp
 * @brief ewol list widget system (Sources)
 * @author Edouard DUPIN
 * @date 27/12/2011
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


#include <ewol/widget/List.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>

#undef __class__
#define __class__	"ewol::List"


void ewol::List::Init(void)
{
	m_paddingSize = 2;
	SetCanHaveFocus(true);
}

ewol::List::List(void)
{
	Init();
}

ewol::List::~List(void)
{
	
}

bool ewol::List::CalculateMinSize(void)
{
	/*int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = 3+minWidth;
	m_minSize.y = 3+minHeight;
	*/
	m_minSize.x = 150;
	m_minSize.y = 150;
	return true;
}


void ewol::List::OnRegenerateDisplay(void)
{
	// clean the object list ...
	ClearOObjectList();
	
	
	int32_t tmpOriginX = 0;
	int32_t tmpOriginY = 0;
	
	if (true==m_userFillX) {
		tmpOriginX = (m_size.x - m_minSize.x) / 2;
	}
	if (true==m_userFillY) {
		tmpOriginY = (m_size.y - m_minSize.y) / 2;
	}
	tmpOriginX += m_paddingSize;
	tmpOriginY += m_paddingSize;

	//ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
	//tmpText->Text(tmpOriginX, tmpOriginY, "jhgjhg");

	//AddOObject(tmpText, "ListText");

}



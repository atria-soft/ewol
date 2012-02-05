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
	m_paddingSizeX = 2;
	#ifdef __PLATFORM__Android
		m_paddingSizeY = 10;
	#else
		m_paddingSizeY = 2;
	#endif
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
	//EWOL_DEBUG("OnRegenerateDisplay(" << m_size.x << "," << m_size.y << ")");
	
	int32_t tmpOriginX = 0;
	int32_t tmpOriginY = 0;
	int32_t tmpOriginYBG = 0;
	/*
	if (true==m_userFillX) {
		tmpOriginX = 0;
	}
	if (true==m_userFillY) {
		tmpOriginY = 0;
	}*/
	tmpOriginX += m_paddingSizeX;
	tmpOriginY += m_paddingSizeY;

	int32_t fontId = GetDefaultFontId();
	//int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);


	//uint32_t nbColomn = GetNuberOfColomn();
	uint32_t nbRaw    = GetNuberOfRaw();
	etk::VectorType<int32_t> listSizeColomn;
	
	ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
	color_ts basicBG = GetBasicBG();
	BGOObjects->SetColor(basicBG);
	BGOObjects->Rectangle(0, 0, m_size.x, m_size.y);
	
	uint32_t displayableRaw = m_size.y / (minHeight + 2*m_paddingSizeY);
	
	// We display only compleate lines ...
	for(uint32_t iii=0; iii<nbRaw && iii<displayableRaw; iii++) {
		etk::String myTextToWrite;
		color_ts fg;
		color_ts bg;
		GetElement(0, iii, myTextToWrite, fg, bg);
		BGOObjects->SetColor(bg);
		BGOObjects->Rectangle(0, tmpOriginYBG, m_size.x, minHeight+2*m_paddingSizeY);
		tmpOriginYBG += minHeight+2*m_paddingSizeY;
		
		ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, fg);
		tmpText->Text(tmpOriginX, tmpOriginY, myTextToWrite.c_str(), m_size.x - (2*m_paddingSizeX));
		AddOObject(tmpText);
		tmpOriginY += minHeight + 2* m_paddingSizeY;
	}
	AddOObject(BGOObjects, "ListDeco", 0);
	//ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
	//tmpText->Text(tmpOriginX, tmpOriginY, "jhgjhg");

	//AddOObject(tmpText, "ListText");

}


bool ewol::List::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	int32_t fontId = GetDefaultFontId();
	//int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);

	int32_t rawID = (y - m_origin.y) / (minHeight + 2*m_paddingSizeY);
	//EWOL_DEBUG("OnEventInput(" << IdInput << "," << typeEvent << ","  << 0 << "," << rawID << "," << x <<"," << y << ");");
	return OnItemEvent(IdInput, typeEvent, 0, rawID, x, y);
}
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
	int32_t minWidth = ewol::GetWidth(fontId, m_label);
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
	if (true == NeedRedraw()) {
		
		// clean the object list ...
		ClearOObjectList();
		//EWOL_DEBUG("OnRegenerateDisplay(" << m_size.x << "," << m_size.y << ")");
		
		int32_t tmpOriginX = 0;
		int32_t tmpOriginY = 0;
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
		//int32_t minWidth = ewol::GetWidth(fontId, m_label);
		int32_t minHeight = ewol::GetHeight(fontId);
	
	
		//uint32_t nbColomn = GetNuberOfColomn();
		uint32_t nbRaw    = GetNuberOfRaw();
		// For the scrooling windows
		m_maxSize.x = m_size.x;
		m_maxSize.y = (minHeight + 2*m_paddingSizeY) * nbRaw;
		
		
		etk::VectorType<int32_t> listSizeColomn;
		
		ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
		color_ts basicBG = GetBasicBG();
		BGOObjects->SetColor(basicBG);
		BGOObjects->Rectangle(0, 0, m_size.x, m_size.y);
		
		uint32_t displayableRaw = m_size.y / (minHeight + 2*m_paddingSizeY) +2;
		
		int32_t startRaw = m_originScrooled.y / (minHeight + 2*m_paddingSizeY);
		
		if (startRaw >= nbRaw-1 ) {
			startRaw = nbRaw - 1;
		}
		if (startRaw<0) {
			startRaw = 0;
		}
		// Calculate the real position ...
		tmpOriginY = -m_originScrooled.y + startRaw*(minHeight + 2*m_paddingSizeY);
		
		// We display only compleate lines ...
		//EWOL_DEBUG("Request drawing list : " << startRaw << "-->" << (startRaw+displayableRaw) << " in " << nbRaw << "raws ; start display : " << m_originScrooled.y << " ==> " << tmpOriginY << " line size=" << minHeight + 2*m_paddingSizeY );
		
		clipping_ts drawClipping;
		drawClipping.x = 0;
		drawClipping.y = 0;
		drawClipping.w = m_size.x - (2*m_paddingSizeX);
		drawClipping.h = m_size.y;
		
		for(uint32_t iii=startRaw; iii<nbRaw && iii<(startRaw+displayableRaw); iii++) {
			etk::UString myTextToWrite;
			color_ts fg;
			color_ts bg;
			GetElement(0, iii, myTextToWrite, fg, bg);
			BGOObjects->SetColor(bg);
			BGOObjects->Rectangle(0, tmpOriginY, m_size.x, minHeight+2*m_paddingSizeY);
			
			ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, fg);
			
			coord2D_ts textPos;
			textPos.x = tmpOriginX;
			textPos.y = tmpOriginY + m_paddingSizeY;
			tmpText->Text(textPos, drawClipping, myTextToWrite);
			
			AddOObject(tmpText);
			tmpOriginY += minHeight + 2* m_paddingSizeY;
		}
		AddOObject(BGOObjects, 0);
		
		// call the herited class...
		WidgetScrooled::OnRegenerateDisplay();
	}
}


bool ewol::List::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	if (true == WidgetScrooled::OnEventInput(IdInput, typeEvent, x, y)) {
		ewol::widgetManager::FocusKeep(this);
		// nothing to do ... done on upper widet ...
		return true;
	}
	int32_t fontId = GetDefaultFontId();
	//int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);
	
	int32_t startRaw = m_originScrooled.y / (minHeight + 2*m_paddingSizeY) - 1;
	if (startRaw<0) {
		startRaw = 0;
	}
	int32_t rawID = (y - m_origin.y) / (minHeight + 2*m_paddingSizeY) + startRaw;
	//EWOL_DEBUG("OnEventInput(" << IdInput << "," << typeEvent << ","  << 0 << "," << rawID << "," << x <<"," << y << ");");
	bool isUsed = OnItemEvent(IdInput, typeEvent, 0, rawID, x, y);
	if (true == isUsed) {
		// TODO : this generate bugs ... I did not understand why ..
		//ewol::widgetManager::FocusKeep(this);
	}
	return isUsed;
}



void ewol::List::OnGetFocus(void)
{
	EWOL_DEBUG("Ewol::List Get Focus");
}

void ewol::List::OnLostFocus(void)
{
	EWOL_DEBUG("Ewol::List Lost Focus");
}

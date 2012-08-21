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

#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"List"


void ewol::List::Init(void)
{
	m_paddingSizeX = 2;
	#ifdef __TARGET_OS__Android
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
	//clean all the object
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}


bool ewol::List::CalculateMinSize(void)
{
	/*int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_label);
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = 3+minWidth;
	m_minSize.y = 3+minHeight;
	*/
	m_minSize.x = 200;
	m_minSize.y = 150;
	return true;
}


void ewol::List::AddOObject(ewol::OObject* newObject, int32_t pos)
{
	if (NULL == newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (pos < 0 || pos >= m_listOObject.Size() ) {
		m_listOObject.PushBack(newObject);
	} else {
		m_listOObject.Insert(pos, newObject);
	}
}


void ewol::List::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}

void ewol::List::OnDraw(DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (NULL != m_listOObject[iii]) {
			m_listOObject[iii]->Draw();
		}
	}
	WidgetScrooled::OnDraw(displayProp);
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
		int32_t nbRaw    = GetNuberOfRaw();
		// For the scrooling windows
		m_maxSize.x = m_size.x;
		m_maxSize.y = (minHeight + 2*m_paddingSizeY) * nbRaw;
		
		
		etk::Vector<int32_t> listSizeColomn;
		
		ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
		draw::Color basicBG = GetBasicBG();
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
		tmpOriginY = -m_originScrooled.y + (startRaw+1)*(minHeight + 2*m_paddingSizeY);
		
		// We display only compleate lines ...
		//EWOL_DEBUG("Request drawing list : " << startRaw << "-->" << (startRaw+displayableRaw) << " in " << nbRaw << "raws ; start display : " << m_originScrooled.y << " ==> " << tmpOriginY << " line size=" << minHeight + 2*m_paddingSizeY );
		
		clipping_ts drawClipping;
		drawClipping.x = 0;
		drawClipping.y = 0;
		drawClipping.w = m_size.x - (2*m_paddingSizeX);
		drawClipping.h = m_size.y;
		
		for(int32_t iii=startRaw; iii<nbRaw && iii<(startRaw+displayableRaw); iii++) {
			etk::UString myTextToWrite;
			draw::Color fg;
			draw::Color bg;
			GetElement(0, iii, myTextToWrite, fg, bg);
			BGOObjects->SetColor(bg);
			BGOObjects->Rectangle(0, m_size.y - tmpOriginY, m_size.x, minHeight+2*m_paddingSizeY);
			
			ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, fg);
			
			Vector2D<float> textPos;
			textPos.x = tmpOriginX;
			textPos.y = m_size.y - tmpOriginY + m_paddingSizeY;
			tmpText->Text(textPos, drawClipping, myTextToWrite);
			
			AddOObject(tmpText);
			tmpOriginY += minHeight + 2* m_paddingSizeY;
		}
		AddOObject(BGOObjects, 0);
		
		// call the herited class...
		WidgetScrooled::OnRegenerateDisplay();
	}
}

/**
 * @brief Event on an input of this Widget
 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::List::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos)
{
	Vector2D<float> relativePos = RelativePosition(pos);
	// corection for the openGl abstraction
	relativePos.y = m_size.y - relativePos.y;
	
	if (true == WidgetScrooled::OnEventInput(type, IdInput, typeEvent, pos)) {
		ewol::widgetManager::FocusKeep(this);
		// nothing to do ... done on upper widet ...
		return true;
	}
	int32_t fontId = GetDefaultFontId();
	//int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);
	
	int32_t rawID = (relativePos.y+m_originScrooled.y) / (minHeight + 2*m_paddingSizeY);
	//EWOL_DEBUG("OnEventInput(" << IdInput << "," << typeEvent << ","  << 0 << "," << rawID << "," << x <<"," << y << ");");
	bool isUsed = OnItemEvent(IdInput, typeEvent, 0, rawID, pos.x, pos.y);
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

/**
 *******************************************************************************
 * @file ewol/widget/ListFile.cpp
 * @brief ewol File lister widget system (Sources)
 * @author Edouard DUPIN
 * @date 12/07/2012
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

#include <ewol/widgetMeta/ParameterList.h>
#include <etk/tool.h>


#undef __class__
#define __class__	"ParameterList"

extern const char * const ewolEventParameterListSelect     = "ewol-event-parameter-list-select";

#include <ewol/widget/List.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>

#undef __class__
#define __class__	"List"


ewol::ParameterList::ParameterList(void)
{
	AddEventId(ewolEventParameterListSelect);
	
	m_idSelected = -1;
	m_paddingSizeX = 2;
	#ifdef __TARGET_OS__Android
		m_paddingSizeY = 10;
	#else
		m_paddingSizeY = 2;
	#endif
	SetCanHaveFocus(true);
}

ewol::ParameterList::~ParameterList(void)
{
	//clean all the object
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
	MenuClear();
}


bool ewol::ParameterList::CalculateMinSize(void)
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


void ewol::ParameterList::AddOObject(ewol::OObject* newObject, int32_t pos)
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


void ewol::ParameterList::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}

void ewol::ParameterList::OnDraw(DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (NULL != m_listOObject[iii]) {
			m_listOObject[iii]->Draw();
		}
	}
	WidgetScrooled::OnDraw(displayProp);
}




void ewol::ParameterList::OnRegenerateDisplay(void)
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
		int32_t nbRaw    = m_list.Size();
		// For the scrooling windows
		m_maxSize.x = m_size.x;
		m_maxSize.y = (minHeight + 2*m_paddingSizeY) * nbRaw;
		
		
		etk::VectorType<int32_t> listSizeColomn;
		
		// set background color :
		ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
		BGOObjects->SetColor(0xFFFFFFFF);
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
		tmpOriginY = m_size.y - (-m_originScrooled.y + (startRaw+1)*(minHeight + 2*m_paddingSizeY));
		
		clipping_ts drawClipping;
		drawClipping.x = 0;
		drawClipping.y = 0;
		drawClipping.w = m_size.x - (2*m_paddingSizeX);
		drawClipping.h = m_size.y;
		
		for(int32_t iii=startRaw; iii<nbRaw && iii<(startRaw+displayableRaw); iii++) {
			etk::UString myTextToWrite = "???";
			color_ts fg(0x000000FF);
			if (m_list[iii] != NULL) {
				myTextToWrite = m_list[iii]->m_label;
			}
			
			ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, fg);
			
			Vector2D<float> textPos;
			textPos.x = (int32_t)tmpOriginX;
			if (m_list[iii]->m_group == false) {
				textPos.x += minHeight;
			}
			textPos.y = (int32_t)(tmpOriginY + m_paddingSizeY);
			tmpText->Text(textPos, drawClipping, myTextToWrite);
			
			AddOObject(tmpText);
			tmpOriginY -= minHeight + 2* m_paddingSizeY;
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
bool ewol::ParameterList::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos)
{
	if (true == WidgetScrooled::OnEventInput(type, IdInput, typeEvent, pos)) {
		ewol::widgetManager::FocusKeep(this);
		// nothing to do ... done on upper widet ...
		return true;
	}
	if (IdInput == 1 && typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE) {
		Vector2D<float> relativePos = RelativePosition(pos);
		// corection for the openGl abstraction
		relativePos.y = m_size.y - relativePos.y;
		int32_t fontId = GetDefaultFontId();
		//int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
		int32_t minHeight = ewol::GetHeight(fontId);
		
		int32_t rawID = (relativePos.y+m_originScrooled.y) / (minHeight + 2*m_paddingSizeY);
		// generate an event on a rawId if the element request change and Select it ...
		if (rawID >=0 && rawID<m_list.Size()) {
			if (m_list[rawID]!=NULL) {
				if (m_list[rawID]->m_refId>=0) {
					GenerateEventId(ewolEventParameterListSelect, m_list[rawID]->m_refId);
					m_idSelected = rawID;
					MarkToRedraw();
					return true;
				}
			}
		}
	}
	return false;
}



void ewol::ParameterList::OnGetFocus(void)
{
	EWOL_DEBUG("Ewol::List Get Focus");
}

void ewol::ParameterList::OnLostFocus(void)
{
	EWOL_DEBUG("Ewol::List Lost Focus");
}

void ewol::ParameterList::MenuAdd(etk::UString& label, int32_t refId, etk::UString& image)
{
	ewol::elementPL* tmpEmement = new ewol::elementPL(label, refId, image, false);
	if (NULL != tmpEmement) {
		m_list.PushBack(tmpEmement);
		if (m_idSelected == -1 && label != "---" && refId>0) {
			m_idSelected = m_list.Size()-1;
		}
		MarkToRedraw();
	}
}
void ewol::ParameterList::MenuAddGroup(etk::UString& label)
{
	etk::UString image = "";
	ewol::elementPL* tmpEmement = new ewol::elementPL(label, -1, image, true);
	if (NULL != tmpEmement) {
		m_list.PushBack(tmpEmement);
		MarkToRedraw();
	}
}


void ewol::ParameterList::MenuClear(void)
{
	m_idSelected = -1;
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	m_list.Clear();
}

void ewol::ParameterList::MenuSeparator(void)
{
	if (m_list.Size()>0) {
		etk::UString label = "";
		etk::UString image = "";
		MenuAdd(label, -1, image);
	}
}


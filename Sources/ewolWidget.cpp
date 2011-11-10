/**
 *******************************************************************************
 * @file ewolWidget.cpp
 * @brief basic ewol Widget (Sources)
 * @author Edouard DUPIN
 * @date 18/10/2011
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

#include <ewolWidget.h>

#undef __class__
#define __class__	"ewol::Widget"

ewol::Widget::Widget(void)
{
	m_origin.x = 0.0;
	m_origin.y = 0.0;
	m_size.x = 10.0;
	m_size.y = 10.0;
	m_minSize.x = -1.0;
	m_minSize.y = -1.0;
	// user settings :
	m_userMinSize.x = -1.0;
	m_userMinSize.y = -1.0;
	m_userExpendX = false;
	m_userExpendY = false;
	m_genericDraw = true;
	m_specificDraw = false;
}

ewol::Widget::~Widget(void)
{
	
}



bool ewol::Widget::CalculateSize(double availlableX, double availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	return true;
}


bool ewol::Widget::GenEventInput(int32_t IdInput, eventInputType_te typeEvent, double x, double y)
{
	bool ended = false;
	for(int32_t iii=m_inputEvent.Size()-1; iii>=0; iii--) {
		if (EWOL_EVENT_AREA == m_inputEvent[iii].mode) {
			if(    m_inputEvent[iii].area.origin.x <= x
			    && m_inputEvent[iii].area.origin.y <= y
			    && m_inputEvent[iii].area.origin.x + m_inputEvent[iii].area.size.x > x
			    && m_inputEvent[iii].area.origin.y + m_inputEvent[iii].area.size.y > y )
			{
				if(    (m_inputEvent[iii].area.flags & (1<<(IdInput-1)) )
				    && (    (    (FLAG_EVENT_INPUT_MOTION          & m_inputEvent[iii].area.flags) && EVENT_INPUT_TYPE_MOVE   == typeEvent)
				         || (    (FLAG_EVENT_INPUT_ENTER           & m_inputEvent[iii].area.flags) && EVENT_INPUT_TYPE_ENTER  == typeEvent)
				         || (    (FLAG_EVENT_INPUT_LEAVE           & m_inputEvent[iii].area.flags) && EVENT_INPUT_TYPE_LEAVE  == typeEvent)
				         || (    (FLAG_EVENT_INPUT_DOWN            & m_inputEvent[iii].area.flags) && EVENT_INPUT_TYPE_DOWN   == typeEvent)
				         || (    (FLAG_EVENT_INPUT_UP              & m_inputEvent[iii].area.flags) && EVENT_INPUT_TYPE_UP     == typeEvent)
				         || (    (FLAG_EVENT_INPUT_CLICKED         & m_inputEvent[iii].area.flags) && EVENT_INPUT_TYPE_SINGLE == typeEvent)
				         || (    (FLAG_EVENT_INPUT_CLICKED_DOUBLE  & m_inputEvent[iii].area.flags) && EVENT_INPUT_TYPE_DOUBLE == typeEvent)
				         || (    (FLAG_EVENT_INPUT_CLICKED_TRIPLE  & m_inputEvent[iii].area.flags) && EVENT_INPUT_TYPE_TRIPLE == typeEvent)
				       )
				  )
				{
					ended = OnEventArea(m_inputEvent[iii].generateEventId, x, y);
					if (true == ended) {
						break;
					}
					// todo : call other link widget :
					if (-1 != m_inputEvent[iii].widgetCall) {
						ewol::Widget * tmpWidget = NULL;
						//tmpWidget = ewol::GetWidgetWithID(newEvent.widgetCall);
						ended = tmpWidget->OnEventAreaExternal(m_uniqueId, m_inputEvent[iii].generateEventId, x, y);
						if (true == ended) {
							break;
						}
					}
				}
			}
		}
	}
	if (false == ended) {
		return OnEventInput(IdInput, typeEvent, x, y);
	}
	return ended;
}


bool ewol::Widget::GenEventShortCut(bool shift, bool control, bool alt, bool pomme, char UTF8_data[UTF8_MAX_SIZE])
{
	return true;
}


bool ewol::Widget::AddEventArea(coord origin, coord size, uint64_t flags, const char * generateEventId)
{
	/*
	if(    origin.x < 0.0
	    || origin.y < 0.0)
	{
		EWOL_WARNING("origin under 0.0 ... out of range");
		return false;
	}
	if(    size.x < 0.0
	    || size.y < 0.0)
	{
		EWOL_WARNING("size under 0.0 ... out of range");
		return false;
	}
	if(    origin.x > m_size.x
	    || origin.y > m_size.y)
	{
		EWOL_WARNING("origin out of range");
		return false;
	}
	if(    origin.x + size.x > m_size.x
	    || origin.y + size.y > m_size.y)
	{
		EWOL_WARNING("end area out of size");
		return false;
	}
	*/
	event_ts newEvent;
	newEvent.generateEventId = generateEventId;
	newEvent.widgetCall = -1; // by default no widget is called
	newEvent.mode = EWOL_EVENT_AREA;
	newEvent.area.origin.x = origin.x + m_origin.x;
	newEvent.area.origin.y = origin.y + m_origin.y;
	newEvent.area.size = size;
	newEvent.area.flags = flags;
	m_inputEvent.PushBack(newEvent);
	EWOL_DEBUG("Add an area event...");
	return true;
}


bool ewol::Widget::AddEventShortCut(bool shift, bool control, bool alt, bool pomme, char UTF8_data[UTF8_MAX_SIZE], const char * generateEventId)
{
	return true;
}


bool ewol::Widget::AddEventShortCut(char * descriptiveString, const char * generateEventId)
{
	return true;
}


bool ewol::Widget::ExternLinkOnEvent(const char * eventName, int32_t widgetId)
{
	return true;
}




void ewol::Widget::AddOObject(ewol::OObject* newObject, etk::String name)
{
	if (NULL == newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system : name=\"" << name << "\"");
		return;
	}
	newObject->SetName(name);
	//EWOL_INFO("UPDATE AT size : (" << m_size.x << "," << m_size.y << ")");
	newObject->UpdateSize(m_size.x, m_size.y);
	//EWOL_INFO("UPDATE AT origin : (" << m_origin.x << "," << m_origin.y << ")");
	newObject->UpdateOrigin(m_origin.x, m_origin.y);
	m_listOObject.PushBack(newObject);
}


ewol::OObject* ewol::Widget::GetOObject(etk::String name)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (m_listOObject[iii]->GetName() == name) {
			return m_listOObject[iii];
		}
	}
	return NULL;
}

void ewol::Widget::RmOObjectElem(etk::String name)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (m_listOObject[iii]->GetName() == name) {
			delete(m_listOObject[iii]);
			m_listOObject[iii] = NULL;
			m_listOObject.Erase(iii);
			return;
		}
	}
}

void ewol::Widget::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}

bool ewol::Widget::GenericDraw(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (NULL != m_listOObject[iii]) {
			m_listOObject[iii]->Draw();
		}
	}
	return true;
}


//} // ???



/**
 *******************************************************************************
 * @file ewol/Widget.cpp
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

#include <ewol/Widget.h>
#include <ewol/WidgetManager.h>
#include <ewol/WidgetMessageMultiCast.h>


char* ewol::GetCharTypeMoveEvent(eventKbMoveType_te type)
{
	char * returnValue = "?";
	switch(type) {
		case ewol::EVENT_KB_MOVE_TYPE_LEFT:				returnValue = "LEFT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_RIGHT:			returnValue = "RIGHT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_UP:				returnValue = "UP";				break;
		case ewol::EVENT_KB_MOVE_TYPE_DOWN:				returnValue = "DOWN";			break;
		case ewol::EVENT_KB_MOVE_TYPE_PAGE_UP:			returnValue = "PAGE_UP";		break;
		case ewol::EVENT_KB_MOVE_TYPE_PAGE_DOWN:		returnValue = "PAGE_DOWN";		break;
		case ewol::EVENT_KB_MOVE_TYPE_START:			returnValue = "START";			break;
		case ewol::EVENT_KB_MOVE_TYPE_END:				returnValue = "END";			break;
		case ewol::EVENT_KB_MOVE_TYPE_CENTER:			returnValue = "CENTER";			break;
		case ewol::EVENT_KB_MOVE_TYPE_ARRET_DEFIL:		returnValue = "ARRET_DEFIL";	break;
		case ewol::EVENT_KB_MOVE_TYPE_WAIT:				returnValue = "WAIT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_INSERT:			returnValue = "INSERT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_F1:				returnValue = "F1";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F2:				returnValue = "F2";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F3:				returnValue = "F3";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F4:				returnValue = "F4";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F5:				returnValue = "F5";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F6:				returnValue = "F6";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F7:				returnValue = "F7";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F8:				returnValue = "F8";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F9:				returnValue = "F9";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F10:				returnValue = "F10";			break;
		case ewol::EVENT_KB_MOVE_TYPE_F11:				returnValue = "F11";			break;
		case ewol::EVENT_KB_MOVE_TYPE_F12:				returnValue = "F12";			break;
		case ewol::EVENT_KB_MOVE_TYPE_CAPLOCK:			returnValue = "CAPLOCK";		break;
		case ewol::EVENT_KB_MOVE_TYPE_SHIFT_LEFT:		returnValue = "SHIFT_LEFT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_SHIFT_RIGHT:		returnValue = "SHIFT_RIGHT";	break;
		case ewol::EVENT_KB_MOVE_TYPE_CTRL_LEFT:		returnValue = "CTRL_LEFT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_CTRL_RIGHT:		returnValue = "CTRL_RIGHT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_META_LEFT:		returnValue = "META_LEFT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_META_RIGHT:		returnValue = "META_RIGHT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_ALT:				returnValue = "ALT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_ALT_GR:			returnValue = "ALT_GR";			break;
		case ewol::EVENT_KB_MOVE_TYPE_CONTEXT_MENU:		returnValue = "CONTEXT_MENU";	break;
		case ewol::EVENT_KB_MOVE_TYPE_VER_NUM:			returnValue = "VER_NUM";		break;
	}
	return returnValue;
}


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
	SetExpendX();
	SetExpendY();
	SetFillX();
	SetFillY();
	m_genericDraw = true;
	m_specificDraw = false;
	ewol::widgetManager::Add(this);
	m_canFocus = false;
	m_hasFocus = false;
}

ewol::Widget::~Widget(void)
{
	ewol::widgetMessageMultiCast::Rm(GetWidgetId());
	ewol::widgetManager::Rm(this);
}

int32_t ewol::Widget::GetWidgetId(void)
{
	return ewol::widgetManager::Get(this);
}


bool ewol::Widget::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	return true;
}


bool ewol::Widget::GenEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	bool ended = false;
	//EWOL_WARNING("Input event : " << IdInput << " pos(" << x << "," << y << ")");
	for(int32_t iii=m_inputAreaEvent.Size()-1; iii>=0; iii--) {
		if(    m_inputAreaEvent[iii].origin.x <= x
		    && m_inputAreaEvent[iii].origin.y <= y
		    && m_inputAreaEvent[iii].origin.x + m_inputAreaEvent[iii].size.x > x
		    && m_inputAreaEvent[iii].origin.y + m_inputAreaEvent[iii].size.y > y )
		{
			if(    (m_inputAreaEvent[iii].flags & (1<<(IdInput-1)) )
			    && (    (    (FLAG_EVENT_INPUT_MOTION          & m_inputAreaEvent[iii].flags) && EVENT_INPUT_TYPE_MOVE   == typeEvent)
			         || (    (FLAG_EVENT_INPUT_ENTER           & m_inputAreaEvent[iii].flags) && EVENT_INPUT_TYPE_ENTER  == typeEvent)
			         || (    (FLAG_EVENT_INPUT_LEAVE           & m_inputAreaEvent[iii].flags) && EVENT_INPUT_TYPE_LEAVE  == typeEvent)
			         || (    (FLAG_EVENT_INPUT_DOWN            & m_inputAreaEvent[iii].flags) && EVENT_INPUT_TYPE_DOWN   == typeEvent)
			         || (    (FLAG_EVENT_INPUT_UP              & m_inputAreaEvent[iii].flags) && EVENT_INPUT_TYPE_UP     == typeEvent)
			         || (    (FLAG_EVENT_INPUT_CLICKED         & m_inputAreaEvent[iii].flags) && EVENT_INPUT_TYPE_SINGLE == typeEvent)
			         || (    (FLAG_EVENT_INPUT_CLICKED_DOUBLE  & m_inputAreaEvent[iii].flags) && EVENT_INPUT_TYPE_DOUBLE == typeEvent)
			         || (    (FLAG_EVENT_INPUT_CLICKED_TRIPLE  & m_inputAreaEvent[iii].flags) && EVENT_INPUT_TYPE_TRIPLE == typeEvent)
			       )
			  )
			{
				ended = OnEventArea(m_inputAreaEvent[iii].generateEventId, x, y);
				if (true == ended) {
					break;
				}
				if (true == GenEventInputExternal(m_inputAreaEvent[iii].generateEventId, x, y)) {
					ended = true;
					break;
				}
			}
		}
	}
	if (false == ended) {
		return OnEventInput(IdInput, typeEvent, x, y);
	}
	return ended;
}

bool ewol::Widget::GenEventInputExternal(const char * generateEventId, etkFloat_t x, etkFloat_t y)
{
	bool ended = false;
	// For all external Event Requested : 
	for( int32_t jjj=0; jjj<m_externEvent.Size(); jjj++) {
		// Check if it is the same ID : 
		if (m_externEvent[jjj].generateEventId == generateEventId) {
			// get the widget Pointer:
			
			ewol::Widget * tmpWidget = ewol::widgetManager::Get(m_externEvent[jjj].widgetCall);
			if (NULL == tmpWidget) {
				EWOL_ERROR("Try to call an NULL Widget, it might be removed ... id=" << m_externEvent[jjj].widgetCall);
			} else {
				ended = tmpWidget->OnEventAreaExternal(GetWidgetId(), m_externEvent[jjj].generateEventIdExtern, NULL, x, y);
			}
			if (true == ended) {
				break;
			}
		}
	}
	return ended;
}

bool ewol::Widget::GenEventShortCut(bool shift, bool control, bool alt, bool meta, uint32_t unicodeValue)
{
	bool ended = false;
	//EWOL_WARNING("Input event : " << IdInput << " pos(" << x << "," << y << ")");
	for(int32_t iii=m_inputShortCutEvent.Size()-1; iii>=0; iii--) {
		if(    m_inputShortCutEvent[iii].shift == shift
		    && m_inputShortCutEvent[iii].control == control
		    && m_inputShortCutEvent[iii].alt == alt
		    && m_inputShortCutEvent[iii].meta == meta
		    && m_inputShortCutEvent[iii].UnicodeValue == unicodeValue)
		{
			/*
			ended = OnEventArea(m_inputShortCutEvent[iii].generateEventId, -1, -1);
			if (true == ended) {
				break;
			}
			*/
			if (true == GenEventInputExternal(m_inputShortCutEvent[iii].generateEventId, -1, -1)) {
				ended = true;
				break;
			}
		}
	}
	/*
	if (false == ended) {
		return OnEventInput(IdInput, typeEvent, -1, -1);
	}
	*/
	return ended;
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
	eventArea_ts newEvent;
	newEvent.generateEventId = generateEventId;
	newEvent.origin.x = origin.x + m_origin.x;
	newEvent.origin.y = origin.y + m_origin.y;
	newEvent.size = size;
	newEvent.flags = flags;
	m_inputAreaEvent.PushBack(newEvent);
	//EWOL_DEBUG("Add an area event...");
	return true;
}


bool ewol::Widget::AddEventShortCut(bool shift, bool control, bool alt, bool meta, uint32_t unicodeValue, const char * generateEventId)
{
	eventShortCut_ts newEvent;
	newEvent.generateEventId = generateEventId;
	newEvent.shift = shift;
	newEvent.control = control;
	newEvent.alt = alt;
	newEvent.meta = meta;
	newEvent.UnicodeValue = unicodeValue;
	m_inputShortCutEvent.PushBack(newEvent);
	return true;
}


bool ewol::Widget::AddEventShortCut(char * descriptiveString, const char * generateEventId)
{
	if(		NULL==descriptiveString
		||	0==strlen(descriptiveString))
	{
		return false;
	}
	bool shift = false;
	bool control = false;
	bool alt = false;
	bool meta = false;
	uint32_t UnicodeValue = 0;
	
	// parsing of the string :
	//"ctrl+shift+alt+meta+s"
	char * tmp = strstr(descriptiveString, "ctrl");
	if(NULL != tmp) {
		control = true;
	}
	tmp = strstr(descriptiveString, "shift");
	if(NULL != tmp) {
		shift = true;
	}
	tmp = strstr(descriptiveString, "alt");
	if(NULL != tmp) {
		alt = true;
	}
	tmp = strstr(descriptiveString, "meta");
	if(NULL != tmp) {
		meta = true;
	}
	UnicodeValue = descriptiveString[strlen(descriptiveString) -1];
	// add with generic Adding function ...
	return AddEventShortCut(shift, control, alt, meta, UnicodeValue, generateEventId);
}


bool ewol::Widget::ExternLinkOnEvent(const char * eventName, int32_t widgetId, const char * eventExternId)
{
	if(NULL == eventName || 0 > widgetId) {
		EWOL_ERROR("Try to add extern event with wrong input ..");
		return false;
	}
	
	eventExtern_ts tmpEvent;
	// search the ID ...
	for(int32_t iii=0; iii < m_ListEventAvaillable.Size(); iii++) {
		if (strcmp(m_ListEventAvaillable[iii], eventName) == 0) {
			tmpEvent.generateEventId = m_ListEventAvaillable[iii];
			tmpEvent.widgetCall = widgetId;
			tmpEvent.generateEventIdExtern = eventExternId;
			m_externEvent.PushBack(tmpEvent);
			return true;
		}
	}
	EWOL_ERROR("Try to add extern event with Unknow EventID : \"" << eventName << "\"" );
	return false;
}




void ewol::Widget::AddOObject(ewol::OObject* newObject, etk::String name, int32_t pos)
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
	if (pos < 0 || pos >= m_listOObject.Size() ) {
		m_listOObject.PushBack(newObject);
	} else {
		m_listOObject.Insert(pos, newObject);
	}
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



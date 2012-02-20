/**
 *******************************************************************************
 * @file ewol/widget/WidgetShortCut.cpp
 * @brief basic ewol short Cut widget (Sources)
 * @author Edouard DUPIN
 * @date 19/02/2012
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

#include <ewol/widget/WidgetShortCut.h>


ewol::WidgetShortCut::WidgetShortCut(void)
{
	// nothing to do ...
}

ewol::WidgetShortCut::~WidgetShortCut(void)
{
	//clean all the object
	m_inputShortCutEvent.Clear();
}




bool ewol::WidgetShortCut::AddEventShortCut(bool shift, bool control, bool alt, bool meta, uint32_t unicodeValue, const char * generateEventId)
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


bool ewol::WidgetShortCut::AddEventShortCut(char * descriptiveString, const char * generateEventId)
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



bool ewol::WidgetShortCut::GenEventShortCut(bool shift, bool control, bool alt, bool meta, uint32_t unicodeValue)
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
			if (true == GenEventInputExternal(m_inputShortCutEvent[iii].generateEventId, -1, -1)) {
				ended = true;
				break;
			}
		}
	}
	return ended;
}


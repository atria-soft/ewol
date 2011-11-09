/**
 *******************************************************************************
 * @file ewolTest.cpp
 * @brief ewol Test widget system (Sources)
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

#include <widget/ewolTest.h>

#include <ewolOObject.h>



//const char * ewolEventButtonPressed    = "ewol Button Pressed";



#undef __class__
#define __class__	"ewol::Test"

ewol::Test::Test(void)
{
	
}


ewol::Test::~Test(void)
{
	
}

bool ewol::Test::CalculateMinSize(void)
{
	m_minSize.x = 150;
	m_minSize.y = 150;
	return true;
}

void ewol::Test::OnRegenerateDisplay(void)
{
	// clean the object list ...
	ClearOObjectList();
	
	ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
	tmpOObjects->Rectangle( 0, 0, m_size.x, m_size.y,  1.0, 0.0, 0.0, 1.0);
	tmpOObjects->Rectangle( 3, 3, m_size.x-6, m_size.y-6,  1.0, 1.0, 1.0, 1.0);
	AddOObject(tmpOObjects, "BouttonDecoration");
	
	color_ts textColorFg;
	textColorFg.red = 0.0;
	textColorFg.green = 0.0;
	textColorFg.blue = 0.0;
	textColorFg.alpha = 1.0;
	
	
	// Regenerate the event Area:
	/*
	EventAreaRemoveAll();
	coord origin;
	coord size;
	origin.x = 3.0;
	origin.y = 3.0;
	size.x = m_size.x-6;
	size.y = m_size.y-6;
	AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventButtonPressed);
	*/
}

bool ewol::Test::OnEventArea(const char * generateEventId, double x, double y)
{
	bool eventIsOK = false;
/*
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventButtonPressed == generateEventId) {
		EWOL_INFO("BT pressed ... " << m_label);
		eventIsOK = true;
	}
*/
	return eventIsOK;
}

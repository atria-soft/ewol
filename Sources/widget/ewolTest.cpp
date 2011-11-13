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



const char * ewolEventTestPressed    = "ewol Test Pressed";



#undef __class__
#define __class__	"ewol::Test"

ewol::Test::Test(void)
{
	m_elementID = 0;
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
	
	tmpOObjects->SetColor(1.0, 0.0, 0.0, 1.0);
	tmpOObjects->Rectangle( 0, 0, m_size.x, m_size.y);
	tmpOObjects->SetColor(1.0, 1.0, 1.0, 1.0);
	tmpOObjects->Rectangle( 3, 3, m_size.x-6, m_size.y-6);
	
	tmpOObjects->SetColor(0.0, 0.0, 1.0, 1.0);
	
	if (0 == m_elementID) {
		tmpOObjects->Line(50, 50, 100,  50, 10); // 0°
	} else if (1 == m_elementID) {
		tmpOObjects->Line(50, 50, 100, 100, 15); // 45°
	} else if (2 == m_elementID) {
		tmpOObjects->Line(50, 50,  50, 100, 20); // 90°
	} else if (3 == m_elementID) {
		tmpOObjects->Line(50, 50,   0, 100, 5); // 135°
	} else if (4 == m_elementID) {
		tmpOObjects->Line(50, 50,   0,  50, 3); // 180°
	} else if (5 == m_elementID) {
		tmpOObjects->Line(50, 50,   0,   0, 2); // 225°
	} else if (6 == m_elementID) {
		tmpOObjects->Line(50, 50,  50,   0, 1); // °
	} else {
		tmpOObjects->Line(50, 50, 100,   0, 0.5); // °
	}
	
	if (0 == m_elementID) {
		tmpOObjects->SetColor(0.0, 1.0, 0.0, 1.0);
		tmpOObjects->Disc(200, 100, 5);
	} else if (1 == m_elementID) {
		tmpOObjects->SetColor(0.0, 1.0, 0.0, 1.0);
		tmpOObjects->Disc(200, 100, 10);
	} else if (2 == m_elementID) {
		tmpOObjects->SetColor(0.0, 1.0, 0.0, 1.0);
		tmpOObjects->Disc(200, 100, 15);
	} else if (3 == m_elementID) {
		tmpOObjects->SetColor(0.0, 1.0, 0.0, 1.0);
		tmpOObjects->Disc(200, 100, 20);
	} else if (4 == m_elementID) {
		tmpOObjects->SetColor(0.0, 1.0, 0.0, 1.0);
		tmpOObjects->Disc(200, 100, 25);
	} else if (5 == m_elementID) {
		tmpOObjects->SetColor(0.0, 1.0, 0.0, 0.75);
		tmpOObjects->Disc(200, 100, 100);
	} else if (6 == m_elementID) {
		tmpOObjects->SetColor(0.0, 1.0, 0.0, 0.75);
		tmpOObjects->Disc(200, 100, 200);
	} else {
		tmpOObjects->SetColor(0.0, 1.0, 0.0, 0.5);
		tmpOObjects->Disc(200, 100, 300);
	}
	tmpOObjects->SetColor(0.0, 0.0, 0.0, 1.0);
	if (0 == m_elementID) {
		tmpOObjects->Circle(100, 100, 10, 3);
	} else if (1 == m_elementID) {
		tmpOObjects->Circle(100, 100, 20, 3);
	} else if (2 == m_elementID) {
		tmpOObjects->Circle(100, 100, 30, 2);
	} else if (3 == m_elementID) {
		tmpOObjects->Circle(100, 100, 40, 1);
	} else if (4 == m_elementID) {
		tmpOObjects->Circle(100, 100, 50, 0.5);
	} else if (5 == m_elementID) {
		tmpOObjects->Circle(100, 100, 100, 25);
	} else if (6 == m_elementID) {
		tmpOObjects->Circle(100, 100, 100, 35);
	} else {
		tmpOObjects->Circle(100, 100, 100, 50);
	}
	tmpOObjects->SetColor(1.0, 1.0, 0.0, 1.0);
	tmpOObjects->DiscPart(150, 60, 60, 45, 180);
	tmpOObjects->SetColor(0.0, 0.0, 0.0, 1.0);
	tmpOObjects->CirclePart(150, 60, 60, 2, 45, 180);
	
	AddOObject(tmpOObjects, "BouttonDecoration");
	
	color_ts textColorFg;
	textColorFg.red = 0.0;
	textColorFg.green = 0.0;
	textColorFg.blue = 0.0;
	textColorFg.alpha = 1.0;
	
	// Regenerate the event Area:
	EventAreaRemoveAll();
	coord origin;
	coord size;
	origin.x = 3.0;
	origin.y = 3.0;
	size.x = m_size.x-6;
	size.y = m_size.y-6;
	AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventTestPressed);

	//tmpOObjects->SetColor(1.0, 0.0, 0.0, 1.0);
	//tmpOObjects->Circle(100, 100, 100, 35);
	/*
	tmpOObjects->SetPoint(-20, -20);
	tmpOObjects->SetPoint(50, 50);
	tmpOObjects->SetPoint(-20, 100);
	*/
	/*
	tmpOObjects->SetPoint(20, -20);
	tmpOObjects->SetPoint(50, 50);
	tmpOObjects->SetPoint(-20, 100);
	
	AddOObject(tmpOObjects, "BouttonDecoration");
	*/
	
}

bool ewol::Test::OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y)
{
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventTestPressed == generateEventId) {
		m_elementID++;
		if (m_elementID > 7 ) {
			m_elementID = 0;
		}
		OnRegenerateDisplay();
		eventIsOK = true;
	}
	return eventIsOK;
}

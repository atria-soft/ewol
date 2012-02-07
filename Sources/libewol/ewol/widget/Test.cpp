/**
 *******************************************************************************
 * @file ewol/widget/Test.cpp
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

#include <ewol/widget/Test.h>

#include <ewol/OObject.h>
#include <ewol/themeManager.h>



const char * ewolEventTestPressed    = "ewol Test Pressed";



#undef __class__
#define __class__	"ewol::Test"

ewol::Test::Test(void)
{
	AddEventId(ewolEventTestPressed);
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
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		ewol::OObject2DColored * tmpOObjects = NULL;
		
		tmpOObjects = new ewol::OObject2DColored;
		
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
		/*
		tmpOObjects = new ewol::OObject2DColored;
		ewol::theme::Generate(0, 0, *tmpOObjects, 50, 50, m_size.x*0.75, m_size.y*0.75);
		AddOObject(tmpOObjects, "themeObject");
		*/
	}
}

bool ewol::Test::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	EWOL_DEBUG("Event on Test ...");
	if (1 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			m_elementID++;
			if (m_elementID > 7 ) {
				m_elementID = 0;
			}
			MarkToReedraw();
			return true;
		}
	}
	return false;
}

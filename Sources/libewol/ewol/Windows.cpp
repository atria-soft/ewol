/**
 *******************************************************************************
 * @file ewol/Windows.cpp
 * @brief ewol window system (sources)
 * @author Edouard DUPIN
 * @date 20/10/2011
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

#include <etk/Types.h>
#include <etk/String.h>
#include <ewol/Widget.h>
#include <ewol/Windows.h>
#include <ewol/OObject.h>
#include <ewol/Texture.h>
#include <ewol/Font.h>
#include <ewol/ewol.h>
#include <ewol/importgl.h>



#undef __class__
#define __class__	"ewol::Windows"


//list of local events : 
const char * ewolEventWindowsClose    = "ewol Windows close";
const char * ewolEventWindowsMinimize = "ewol Windows minimize";
const char * ewolEventWindowsExpend   = "ewol Windows expend/unExpend";


ewol::Windows::Windows(void)
{
	SetCanHaveFocus(true);
	m_subWidget = NULL;
	// enable specific drawing system ...
	SpecificDrawEnable();
	
	SetDecorationDisable();
	if (true == m_hasDecoration) {
		ewol::OObject2DColored * myOObject = new ewol::OObject2DColored();
		// Close
		myOObject->SetColor(1.0, 0.0, 0.0, 1.0);
		myOObject->Rectangle( 0, 0, 20, 20);
		// Reduce
		myOObject->SetColor(0.0, 1.0, 0.0, 1.0);
		myOObject->Rectangle(20, 0, 20, 20);
		// Expend - Un-expend
		myOObject->SetColor(0.0, 0.0, 1.0, 1.0);
		myOObject->Rectangle(40, 0, 20, 20);
		coord origin;
		coord size;
		origin.x = 0.0;
		origin.y = 0.0;
		size.x = 20;
		size.y = 20;
		AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventWindowsClose);
		origin.x = 20.0;
		AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventWindowsMinimize);
		origin.x = 40.0;
		AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventWindowsExpend);
		
		AddOObject(myOObject, "leftBoutton");
		
		ewol::OObject2DText * myOObjectText = new ewol::OObject2DText();
		myOObjectText->Text(62, 2, "My Title ...");
		AddOObject(myOObjectText, "Title");
	}
}

ewol::Windows::~Windows(void)
{
	if (NULL != m_subWidget) {
		delete(m_subWidget);
		m_subWidget=NULL;
	}
}

bool ewol::Windows::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	if (NULL != m_subWidget) {
		m_subWidget->CalculateMinSize();
		// TODO : Check if min Size is possible ...
		// TODO : Herited from MinSize .. and expand ???
		m_subWidget->CalculateSize(m_size.x, m_size.y);
	}
	// regenerate all the display ...
	OnRegenerateDisplay();
	return true;
}


bool ewol::Windows::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	if (true == m_hasDecoration) {
		if(    x >= 60
		    && y <=20)
		{
			if(EVENT_INPUT_TYPE_MOVE == typeEvent && true == ewol::IsPressedInput(1) ) {
				ewol::StartMoveSystem();
			}
		}
		if(    x >= m_size.x - 20
		    && y >= m_size.y - 20)
		{
			if(EVENT_INPUT_TYPE_MOVE == typeEvent && true == ewol::IsPressedInput(1) ) {
				ewol::StartResizeSystem();
			}
		}
	}
	if (NULL != m_subWidget) {
		m_subWidget->GenEventInput(IdInput, typeEvent, x, y);
	}
	return true;
}

void ewol::Windows::SysDraw(void)
{

	//EWOL_DEBUG("Drow on (" << m_size.x << "," << m_size.y << ")");
	// set the size of the open GL system
	glViewport(0,0,m_size.x,m_size.y);
	
	// Clear the screen with transparency ...
	glClearColor(0.750, 0.750, 0.750, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#if defined(__PLATFORM__Android)
	glOrtho(-m_size.x/2, m_size.x/2, m_size.y/2, -m_size.y/2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(-m_size.x/2, -m_size.y/2, -1.0);
#else
	glOrtho(0., m_size.x, 0., -m_size.y, 1., 20.);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -m_size.y, -5);
#endif
	//http://www.khronos.org/opengles/documentation/opengles1_0/html/glBlendFunc.html
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	GenDraw();

	glDisable(GL_BLEND);
	return;
}

void ewol::Windows::OnRegenerateDisplay(void)
{
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
}


bool ewol::Windows::OnDraw(void)
{
	if (NULL != m_subWidget) {
		m_subWidget->GenDraw();
	}
	return true;
}

bool ewol::Windows::OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y)
{
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventWindowsClose == generateEventId) {
		EWOL_INFO("Request close of the windows");
		ewol::Stop();
		eventIsOK = true;
	} else if(ewolEventWindowsMinimize == generateEventId) {
		EWOL_INFO("Request Minimize of the windows");
		eventIsOK = true;
	} else if(ewolEventWindowsExpend == generateEventId) {
		EWOL_INFO("Request Expend of the windows");
		eventIsOK = true;
	}
	
	return eventIsOK;
}



void ewol::Windows::SetSubWidget(ewol::Widget * widget)
{
	if (NULL != m_subWidget) {
		EWOL_INFO("Remove current main windows Widget...");
		delete(m_subWidget);
		m_subWidget = NULL;
	}
	m_subWidget = widget;
	// Regenerate the size calculation :
	CalculateSize(m_size.x, m_size.y);
}

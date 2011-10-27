/**
 *******************************************************************************
 * @file ewolWindows.cpp
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

#include <etkTypes.h>
#include <etkString.h>
#include <ewolWidget.h>
#include <ewolWindows.h>
#include <ewolOObject.h>

#include <GL/gl.h>

//list of local events : 
const char * ewolEventWindowsClose    = "ewol Windows close";
const char * ewolEventWindowsMinimize = "ewol Windows minimize";
const char * ewolEventWindowsExpend   = "ewol Windows expend/unExpend";

bool ewol::Windows::CalculateSize(double availlableX, double availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	return true;
}


bool ewol::Windows::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, double x, double y)
{
/*
	if(    EVENT_INPUT_TYPE_UP == typeEvent
	    && 1 == IdInput)
	{
		EWOL_INFO("Request ???");
	}
	return true;
*/
	return false;
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
	glOrtho(0., m_size.x, 0., -m_size.y, 1., 20.);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -m_size.y, -5);
	
	//http://www.khronos.org/opengles/documentation/opengles1_0/html/glBlendFunc.html
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
	


	/*
	// create one display list
	indexListe = glGenLists(1);
	// compile the display list, store a triangle in it
	glNewList(indexListe, GL_COMPILE);
		glBegin(GL_QUADS);
			float plop2 = 0.2;
			//glVertex3fv(v0);
			glColor3f(1., 0., 0.); glVertex3f( plop2*m_size.x,       plop2*m_size.y, 0.);
			glColor3f(0., 1., 0.); glVertex3f( (1.0-plop2)*m_size.x, plop2*m_size.y, 0.);
			glColor3f(0., 0., 1.); glVertex3f( (1.0-plop2)*m_size.x, (1.0-plop2)*m_size.y, 0.);
			glColor3f(1., 1., 0.); glVertex3f( plop2*m_size.x,       (1.0-plop2)*m_size.y, 0.);
		glEnd();
	glEndList();
	*/


	static ewol::OObject2DColored myOObject;
	static bool isinit = false;
	
	
	if (false == isinit) {
		isinit=true;
		myOObject.Rectangle( 0, 0, 20, 20,  1.0, 0.0, 0.0, 1.0); // Close
		myOObject.Rectangle(20, 0, 20, 20,  0.0, 1.0, 0.0, 1.0); // Reduce
		myOObject.Rectangle(40, 0, 20, 20,  0.0, 0.0, 1.0, 1.0); // Expend - Un-expend
		
		AddEventArea({ 0.0,0.0}, {20, 20}, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED, ewolEventWindowsClose);
		AddEventArea({20.0,0.0}, {20, 20}, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED, ewolEventWindowsMinimize);
		AddEventArea({40.0,0.0}, {20, 20}, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED, ewolEventWindowsExpend);
		
		// Other ...
		myOObject.Rectangle(20, 30, 100, 50,  1.0, 0.0, 0.0, 1.0);
		myOObject.Rectangle(50, 50, 50,  50,  0.0, 1.0, 0.0, 1.0);
		myOObject.Rectangle(80, 80, 100, 50,  0.0, 0.0, 1.0, 1.0);
		myOObject.Rectangle(50, 00, 300, 300, 0.2, 0.2, 0.2, 0.5);
		
		//myOObject.Rectangle(-50, -50, 120, 120,  0.0, 1.0, 1.0, 0.5);
	}
	
	
	myOObject.Draw();


}


bool ewol::Windows::OnEventArea(const char * generateEventId, double x, double y)
{
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventWindowsClose == generateEventId) {
		EWOL_INFO("Request close of the windows");
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


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
#include <ewolWindows.h>
#include <GL/gl.h>



bool ewol::Windows::CalculateSize(double availlableX, double availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	return true;
}



void ewol::Windows::SysDraw(void)
{
	static double ploppp = 0.1;
	EWOL_DEBUG("Drow on (" << m_size.x << "," << m_size.y << ")");
	// set the size of the open GL system
	glViewport(0,0,m_size.x,m_size.y);
	
	// Clear the screen with transparency ...
	glClearColor(0.750, 0.750, 0.750, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0., m_size.x, 0., m_size.y, 1., 20.);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5);
	
	//EWOL_DEBUG("plop is " << ploppp << " devient " << (1.0-ploppp) );
	
	glBegin(GL_QUADS);
		glColor3f(1., 0., 0.); glVertex3f( ploppp*m_size.x,       ploppp*m_size.y, 0.);
		glColor3f(0., 1., 0.); glVertex3f( (1.0-ploppp)*m_size.x, ploppp*m_size.y, 0.);
		glColor3f(0., 0., 1.); glVertex3f( (1.0-ploppp)*m_size.x, (1.0-ploppp)*m_size.y, 0.);
		glColor3f(1., 1., 0.); glVertex3f( ploppp*m_size.x,       (1.0-ploppp)*m_size.y, 0.);
	glEnd();
	ploppp += 0.05;
	if (ploppp>0.5) {
		ploppp = 0;
	}
}
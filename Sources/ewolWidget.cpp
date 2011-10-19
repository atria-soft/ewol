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


ewol::Widget::Widget(void)
{
	m_origin.x = 0.0;
	m_origin.y = 0.0;
	m_minSize.x = -1.0;
	m_minSize.y = -1.0;
	m_size.x = 10.0;
	m_size.y = 10.0;
	m_maxSize.x = -1.0;
	m_maxSize.y = -1.0;
	m_expendX = false;
	m_expendY = false;
}

ewol::Widget::~Widget(void)
{
	
}



bool ewol::Widget::CalculateSize(double availlableX, double availlableY)
{
	return true;
}


bool ewol::Widget::GenEventInput(int32_t IdInput, eventInputType_te typeEvent, double X, double Y)
{
	return true;
}


bool ewol::Widget::GenEventShortCut(bool shift, bool control, bool alt, bool pomme, char UTF8_data[UTF8_MAX_SIZE])
{
	return true;
}


bool ewol::Widget::AddEventArea(coord origin, coord size, uint32_t flags, const char * generateEventId)
{
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




bool ewol::Widget::GenericDraw(void)
{
	return true;
}


//} // ???



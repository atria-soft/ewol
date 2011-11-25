/**
 *******************************************************************************
 * @file ewol/theme/EolColor.cpp
 * @brief basic ewol theme eol file color (Sources)
 * @author Edouard DUPIN
 * @date 23/11/2011
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


#include <ewol/theme/EolColor.h>

#undef __class__
#define __class__	"ewol::theme::EolColor"



ewol::theme::EolColor::EolColor(void)
{
	m_color.red = 0.0;
	m_color.green = 0.0;
	m_color.blue = 0.0;
	m_color.alpha = 1.0;
}

ewol::theme::EolColor::~EolColor(void)
{
	
}


void ewol::theme::EolColor::Parse(TiXmlNode * pNode)
{
	m_name = pNode->ToElement()->Attribute("name");
	const char *color = pNode->ToElement()->Attribute("val");
	if (NULL != color) {
		if (color[0] == '#') {
			// Find a direct Color
			unsigned int r=0;
			unsigned int v=0;
			unsigned int b=0;
			unsigned int a=0xFF;
			sscanf(color, "#%02x%02x%02x%02x", &r,&v,&b,&a);
			m_color.red = (etkFloat_t)r/255.0;
			m_color.green = (etkFloat_t)v/255.0;
			m_color.blue = (etkFloat_t)b/255.0;
			m_color.alpha = (etkFloat_t)a/255.0;
		} else if (color[0] == '&') {
			//find a reference Color
			
		} else {
			// must be a float color
			double r=0.0;
			double v=0.0;
			double b=0.0;
			double a=1.0;
			sscanf(color, "%lf;%lf;%lf;%lf", &r,&v,&b,&a);
			m_color.red = (etkFloat_t)r;
			m_color.green = (etkFloat_t)v;
			m_color.blue = (etkFloat_t)b;
			m_color.alpha = (etkFloat_t)a;
			if (m_color.red>1.0) { m_color.red = 1.0; }
			if (m_color.green>1.0) { m_color.green = 1.0; }
			if (m_color.blue>1.0) { m_color.blue = 1.0; }
			if (m_color.alpha>1.0) { m_color.alpha = 1.0; }
		}
	}
	if (NULL != color) {
		EWOL_INFO("COLOR name=\"" << m_name << "\" \"" << color << "\" ==> red="<< m_color.red <<" green="<< m_color.green <<" blue="<< m_color.blue <<" alpha="<< m_color.alpha );
	} else {
		EWOL_INFO("COLOR name=\"" << m_name << "\" \"\"???? ==> red="<< m_color.red <<" green="<< m_color.green <<" blue="<< m_color.blue <<" alpha="<< m_color.alpha );
	}
}


etk::String ewol::theme::EolColor::GetName(void)
{
	return m_name;
}


void ewol::theme::EolColor::SetName(etk::String & newName)
{
	m_name = newName;
}


bool ewol::theme::EolColor::HasName(etk::String & newName)
{
	return m_name == newName;
}


color_ts ewol::theme::EolColor::Get(void)
{
	return m_color;
}


void ewol::theme::EolColor::Set(color_ts newColor)
{
	m_color = newColor;
}


void ewol::theme::EolColor::Set(etkFloat_t red, etkFloat_t green, etkFloat_t blue, etkFloat_t alpha)
{
	m_color.red = red;
	m_color.green = green;
	m_color.blue = blue;
	m_color.alpha = alpha;
}

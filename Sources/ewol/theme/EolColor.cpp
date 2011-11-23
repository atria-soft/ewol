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
	
}


ewol::theme::EolColor::~EolColor(void)
{
	
}


void ewol::theme::EolColor::Load(const char * data, int32_t len)
{
	
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

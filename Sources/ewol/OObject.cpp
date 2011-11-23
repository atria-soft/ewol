/**
 *******************************************************************************
 * @file ewolOObject.cpp
 * @brief ewol OpenGl Object system (Sources)
 * @author Edouard DUPIN
 * @date 24/10/2011
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
#include <ewolOObject.h>
#include <GL/gl.h>


#undef __class__
#define __class__	"ewol::OObject"


ewol::OObject::OObject(void)
{
	m_name="";
}


ewol::OObject::~OObject(void)
{
	
}

void ewol::OObject::SetName(etk::String & name)
{
	m_name = name;
}

void ewol::OObject::SetName(const char * name)
{
	if (NULL != name) {
		m_name = name;
	}
}

etk::String ewol::OObject::GetName(void)
{
	return m_name;
}



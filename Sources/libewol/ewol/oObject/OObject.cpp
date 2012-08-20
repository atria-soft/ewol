/**
 *******************************************************************************
 * @file ewol/oObject/OObject.cpp
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

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/oObject/OObject.h>


#undef __class__
#define __class__	"ewol::OObject"


ewol::OObject::OObject(void)
{
	m_hasClipping = false;
	m_scaling.x = 1.0;
	m_scaling.y = 1.0;
	m_nbLoadedTime = 1;
}


ewol::OObject::~OObject(void)
{
	
}


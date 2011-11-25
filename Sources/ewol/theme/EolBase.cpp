/**
 *******************************************************************************
 * @file ewol/theme/EolBase.cpp
 * @brief basic ewol theme eol file basic element Virtual Classes (Sources)
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

#include <ewol/theme/EolBase.h>

#undef __class__
#define __class__	"ewol::theme::EolBase"


ewol::theme::EolBase::EolBase(void)
{
	
}

ewol::theme::EolBase::~EolBase(void)
{
	
}


void ewol::theme::EolBase::Parse(TiXmlNode * pNode)
{
	m_name = pNode->ToElement()->Attribute("name");
	EWOL_INFO("Group name=\"" << m_name << "\" " );
}


etk::String ewol::theme::EolBase::GetName(void)
{
	return m_name;
}


void ewol::theme::EolBase::SetName(etk::String & newName)
{
	m_name = newName;
}


bool ewol::theme::EolBase::HasName(etk::String & newName)
{
	return m_name == newName;
}


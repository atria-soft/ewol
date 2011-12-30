/**
 *******************************************************************************
 * @file ewol/widget/Spacer.cpp
 * @brief ewol Spacer widget system (Sources)
 * @author Edouard DUPIN
 * @date 29/12/2011
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

#include <ewol/widget/Spacer.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


#undef __class__
#define __class__	"ewol::Spacer"


ewol::Spacer::Spacer(void)
{
	m_size = 10;
	GenericDrawDisable();
	SetCanHaveFocus(false);
}

ewol::Spacer::~Spacer(void)
{
	
}

bool ewol::Spacer::CalculateMinSize(void)
{
	m_minSize.x = m_size;
	m_minSize.y = m_size;
	return true;
}


void ewol::Spacer::SetSize(etkFloat_t size)
{
	m_size = size;
}


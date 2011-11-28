/**
 *******************************************************************************
 * @file ewol/theme/EolBaseLine.cpp
 * @brief basic ewol theme eol file basic element type=Line (Sources)
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

#include <ewol/theme/EolBaseLine.h>

#undef __class__
#define __class__	"ewol::theme::EolBaseLine"


ewol::theme::EolBaseLine::EolBaseLine(void)
{
	//EWOL_DEBUG("new element Base : Line ...");
}

ewol::theme::EolBaseLine::~EolBaseLine(void)
{
	
}


void ewol::theme::EolBaseLine::Parse(TiXmlNode * pNode)
{
	m_color = pNode->ToElement()->Attribute("color");
	
	const char * tmp = pNode->ToElement()->Attribute("positionStart");
	if (NULL == tmp) {
		m_posStart.x=0;
		m_posStart.y=0;
	} else {
		double xxx,yyy;
		// optimize for multiple type input ...
		sscanf(tmp, "%lf;%lf", &xxx,&yyy);
		m_posStart.x=xxx;
		m_posStart.y=yyy;
	}
	tmp = pNode->ToElement()->Attribute("positionStop");
	if (NULL == tmp) {
		m_posStop.x=0;
		m_posStop.y=0;
	} else {
		double xxx,yyy;
		// optimize for multiple type input ...
		sscanf(tmp, "%lf;%lf", &xxx,&yyy);
		m_posStop.x=xxx;
		m_posStop.y=yyy;
	}
	tmp = pNode->ToElement()->Attribute("thickness");
	if (NULL == tmp) {
		m_thickness=0;
	} else {
		double th;
		// optimize for multiple type input ...
		sscanf(tmp, "%lf", &th);
		m_thickness=th;
	}
	EWOL_DEBUG("(l " << pNode->Row() << ")     Parse Base Element : \"line\" : pos(" << m_posStart.x << "," << m_posStart.y << ") to pos(" << m_posStop.x << "," << m_posStop.y << ") thickness=" << m_thickness);
}

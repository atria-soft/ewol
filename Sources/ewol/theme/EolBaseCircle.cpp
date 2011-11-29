/**
 *******************************************************************************
 * @file ewol/theme/EolBasicCircle.cpp
 * @brief basic ewol theme eol file basic element type=Circle (Sources)
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

#include <ewol/theme/Theme.h>
#include <ewol/theme/EolBaseCircle.h>

#undef __class__
#define __class__	"ewol::theme::EolBaseCircle"



ewol::theme::EolBaseCircle::EolBaseCircle(void)
{
	//EWOL_DEBUG("new element Base : Circle/Disk ...");
}

ewol::theme::EolBaseCircle::~EolBaseCircle(void)
{
	
}

void ewol::theme::EolBaseCircle::Parse(TiXmlNode * pNode)
{
	m_colorBG     = pNode->ToElement()->Attribute("colorBG");
	m_colorBorder = pNode->ToElement()->Attribute("colorBorder");
	
	const char * tmp = pNode->ToElement()->Attribute("position");
	if (NULL == tmp) {
		m_posCenter.x=0;
		m_posCenter.y=0;
	} else {
		double xxx,yyy;
		// optimize for multiple type input ...
		sscanf(tmp, "%lf;%lf", &xxx,&yyy);
		m_posCenter.x=xxx;
		m_posCenter.y=yyy;
	}
	
	tmp = pNode->ToElement()->Attribute("radius");
	if (NULL == tmp) {
		m_radius=0;
	} else {
		double tmpVal;
		// optimize for multiple type input ...
		sscanf(tmp, "%lf", &tmpVal);
		m_radius=tmpVal;
	}
	
	tmp = pNode->ToElement()->Attribute("thickness");
	if (NULL == tmp) {
		m_thickness=0.01;
	} else {
		double tmpVal;
		// optimize for multiple type input ...
		sscanf(tmp, "%lf", &tmpVal);
		m_thickness=tmpVal;
	}
	EWOL_DEBUG("(l " << pNode->Row() << ")     Parse Base Element : \"Circle\" : pos(" << m_posCenter.x << "," << m_posCenter.y << ") radius=" << m_radius
	           << " colorBG=\"" << m_colorBG << "\" colorBorder=\"" << m_colorBorder << "\" thickness=" << m_thickness);
}


void ewol::theme::EolBaseCircle::Generate(const ewol::theme::Theme * myTheme, const ewol::theme::EolElement * myElement, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY)
{
	if (m_colorBG != "") {
		color_ts selectedColorBG = {1.0, 1.0, 1.0, 1.0};
		bool res = false;
		if (NULL != myElement) {
			res = myElement->GetColor(m_colorBG, selectedColorBG);
		}
		// try from theme if not existed
		if(    false == res
		    && NULL != myTheme ) {
			myElement->GetColor(m_colorBG, selectedColorBG);
		}
		newObject.SetColor(selectedColorBG);
		newObject.Disc(posX + m_posCenter.x*sizeX, posY + m_posCenter.y*sizeY, m_radius*(sizeX+sizeY)/2);
	}
	if (m_colorBorder != "") {
		color_ts selectedColorBorder = {0.0, 0.0, 0.0, 1.0};
		bool res = false;
		// try get color for current element
		if (NULL != myElement) {
			res = myElement->GetColor(m_colorBorder, selectedColorBorder);
		}
		// try from theme if not existed
		if(    false == res
		    && NULL != myTheme ) {
			myElement->GetColor(m_colorBorder, selectedColorBorder);
		}
		newObject.SetColor(selectedColorBorder);
		newObject.Circle(posX + m_posCenter.x*sizeX, posY + m_posCenter.y*sizeY, m_radius*(sizeX+sizeY)/2, m_thickness*(sizeX+sizeY)/2);
	}
}





/**
 *******************************************************************************
 * @file ewol/widget/ColorBar.cpp
 * @brief ewol ColorBar widget system (Sources)
 * @author Edouard DUPIN
 * @date 05/03/2012
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

#include <ewol/widget/ColorBar.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


extern const char * const ewolEventColorBarChange    = "ewol-color-bar-change";



#undef __class__
#define __class__	"ColorBar"

ewol::ColorBar::ColorBar(void)
{
	AddEventId(ewolEventColorBarChange);
	
	#ifdef __PLATFORM__Android
		m_padding.y = 12;
		m_padding.x = 12;
	#else
		m_padding.y = 4;
		m_padding.x = 4;
	#endif
	m_currentUserPos.x=0;
	m_currentUserPos.y=0;
	m_currentColor.red   = 0.0;
	m_currentColor.green = 0.0;
	m_currentColor.blue  = 0.0;
	m_currentColor.alpha = 1.0;
	SetCanHaveFocus(true);
}

ewol::ColorBar::~ColorBar(void)
{
	
}

bool ewol::ColorBar::CalculateMinSize(void)
{
	m_minSize.x = 80;
	m_minSize.y = 80;
	MarkToReedraw();
	return true;
}

static color_ts s_listColor[7][3] = {
	{ { 1.0, 1.0, 1.0, 1.0 }, {1.0, 0.0, 0.0, 1.0 }, {0.0, 0.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0, 1.0 }, {1.0, 1.0, 0.0, 1.0 }, {0.0, 0.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0, 1.0 }, {0.0, 1.0, 0.0, 1.0 }, {0.0, 0.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0, 1.0 }, {0.0, 1.0, 1.0, 1.0 }, {0.0, 0.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0, 1.0 }, {0.0, 0.0, 1.0, 1.0 }, {0.0, 0.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0, 1.0 }, {1.0, 0.0, 1.0, 1.0 }, {0.0, 0.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0, 1.0 }, {1.0, 0.0, 0.0, 1.0 }, {0.0, 0.0, 0.0, 1.0 } }
};

color_ts ewol::ColorBar::GetCurrentColor(void)
{
	return m_currentColor;
}
void ewol::ColorBar::SetCurrentColor(color_ts newOne)
{
	m_currentColor = newOne;
	m_currentColor.alpha = 1.0;
}

void ewol::ColorBar::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		int32_t tmpSizeX = m_minSize.x;
		int32_t tmpSizeY = m_minSize.y;
		int32_t tmpOriginX = (m_size.x - m_minSize.x) / 2;
		int32_t tmpOriginY = (m_size.y - m_minSize.y) / 2;
		
		if (true==m_userFillX) {
			tmpSizeX = m_size.x;
			tmpOriginX = 0;
		}
		if (true==m_userFillY) {
			tmpSizeY = m_size.y;
			tmpOriginY = 0;
		}
		tmpOriginX += m_padding.x;
		tmpOriginY += m_padding.y;
		tmpSizeX -= 2*m_padding.x;
		tmpSizeY -= 2*m_padding.y;
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		
		tmpOriginX -= m_padding.x/2;
		tmpOriginY -= m_padding.y/2;
		tmpSizeX += m_padding.x;
		tmpSizeY += m_padding.y;
		
		for(int32_t iii=0; iii<6 ; iii++) {
			
			/* Step 1 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			tmpOObjects->SetColor(s_listColor[iii][0]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/6), tmpOriginY);
			tmpOObjects->SetColor(s_listColor[iii+1][1]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/6), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColor[iii][1]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/6), tmpOriginY+tmpSizeY/2);
			
			/* Step 2 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			tmpOObjects->SetColor(s_listColor[iii][0]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/6), tmpOriginY);
			tmpOObjects->SetColor(s_listColor[iii+1][0]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/6), tmpOriginY);
			tmpOObjects->SetColor(s_listColor[iii+1][1]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/6), tmpOriginY+tmpSizeY/2);
			
			/* Step 3 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			tmpOObjects->SetColor(s_listColor[iii][1]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/6), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColor[iii+1][2]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/6), tmpOriginY+tmpSizeY);
			tmpOObjects->SetColor(s_listColor[iii][2]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/6), tmpOriginY+tmpSizeY);
			
			/* Step 4 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			tmpOObjects->SetColor(s_listColor[iii][1]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/6), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColor[iii+1][1]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/6), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColor[iii+1][2]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/6), tmpOriginY+tmpSizeY);
			
			//tmpOObjects->Rectangle( tmpOriginX + iii*(tmpSizeX/7), tmpOriginY, tmpSizeX/7, tmpSizeY);
		}
		color_ts tmpColor;
		tmpColor.red   = 0.5;
		tmpColor.green = 0.5;
		tmpColor.blue  = 0.5;
		tmpColor.alpha = 1.0;
		tmpOObjects->SetColor(tmpColor);
		tmpOObjects->Circle(m_currentUserPos.x*m_size.x, m_currentUserPos.y*m_size.y, 3.0, 1.0);
		
		AddOObject(tmpOObjects);
	}
}


bool ewol::ColorBar::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos)
{
	//EWOL_DEBUG("Event on BT ...");
	if (1 == IdInput) {
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			// nothing to do ...
			//GenerateEventId(ewolEventButtonPressed);
			m_currentUserPos.x=pos.local.x/m_size.x;
			m_currentUserPos.y=pos.local.y/m_size.y;
			MarkToReedraw();
			//==> try to estimate color
			int32_t bandID = (int32_t)(pos.local.x/6);
			etkFloat_t relativePos = pos.local.x - (pos.local.x/6) * bandID;
			
			
			return true;
		}
	}
	return false;
}

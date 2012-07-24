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
	m_currentColor = etk::color::color_Black;
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
static color_ts s_listColorWhite(0xFFFFFFFF);
static color_ts s_listColorBlack(0x000000FF);
#define NB_BAND_COLOR		(6)
static color_ts s_listColor[NB_BAND_COLOR+1] = {
	0xFF0000FF,
	0xFFFF00FF,
	0x00FF00FF,
	0x00FFFFFF,
	0x0000FFFF,
	0xFF00FFFF,
	0xFF0000FF
};

color_ts ewol::ColorBar::GetCurrentColor(void)
{
	return m_currentColor;
}
void ewol::ColorBar::SetCurrentColor(color_ts newOne)
{
	m_currentColor = newOne;
	m_currentColor.alpha = 1.0;
	// estimate the cursor position :
	// TODO : Later when really needed ...
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
		
		for(int32_t iii=0; iii<NB_BAND_COLOR ; iii++) {
			
			/* Step 1 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			tmpOObjects->SetColor(s_listColorWhite);
			tmpOObjects->SetPoint(tmpOriginX + (iii)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY);
			tmpOObjects->SetColor(s_listColor[iii+1]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColor[iii]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			
			/* Step 2 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			tmpOObjects->SetColor(s_listColorWhite);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY);
			tmpOObjects->SetColor(s_listColorWhite);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY);
			tmpOObjects->SetColor(s_listColor[iii+1]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			/* Step 3 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			tmpOObjects->SetColor(s_listColor[iii]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColorBlack);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY);
			tmpOObjects->SetColor(s_listColorBlack);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY);
			/* Step 4 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			tmpOObjects->SetColor(s_listColor[iii]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColor[iii+1]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColorBlack);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY);
			/*
			tmpOObjects->SetColor(s_listColorWhite);
			tmpOObjects->SetPoint(tmpOriginX + (iii+0.5)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY);
			tmpOObjects->SetColor(s_listColor[iii+1]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColor[iii]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			
			tmpOObjects->SetColor(s_listColor[iii]);
			tmpOObjects->SetPoint(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColor[iii+1]);
			tmpOObjects->SetPoint(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2);
			tmpOObjects->SetColor(s_listColorBlack);
			tmpOObjects->SetPoint(tmpOriginX + (iii+0.5)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY);
			*/
		}
		color_ts tmpColor;
		if (m_currentUserPos.y > 0.5) {
			tmpColor.red   = 1.0;
			tmpColor.green = 1.0;
			tmpColor.blue  = 1.0;
		} else {
			tmpColor.red   = 0.0;
			tmpColor.green = 0.0;
			tmpColor.blue  = 0.0;
		}
		tmpColor.alpha = 1.0;
		tmpOObjects->SetColor(tmpColor);
		tmpOObjects->Circle(m_currentUserPos.x*m_size.x, m_currentUserPos.y*m_size.y, 3.0, 1.0);
		
		AddOObject(tmpOObjects);
	}
}

/**
 * @brief Event on an input of this Widget
 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::ColorBar::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos)
{
	Vector2D<float> relativePos = RelativePosition(pos);
	//EWOL_DEBUG("Event on BT ...");
	if (1 == IdInput) {
		relativePos.x = etk_max(etk_min(relativePos.x, m_size.x),0);
		relativePos.y = etk_max(etk_min(relativePos.y, m_size.y),0);
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_MOVE   == typeEvent) {
			// nothing to do ...
			m_currentUserPos.x=relativePos.x/m_size.x;
			m_currentUserPos.y=relativePos.y/m_size.y;
			MarkToReedraw();
			//==> try to estimate color
			EWOL_VERBOSE("event on (" << relativePos.x << "," << relativePos.y << ")");
			int32_t bandID = (int32_t)(relativePos.x/(m_size.x/6));
			float localPos = relativePos.x - (m_size.x/6) * bandID;
			float poroportionnalPos = localPos/(m_size.x/6);
			EWOL_VERBOSE("bandId=" << bandID << "  relative pos=" << localPos);
			color_ts estimateColor;
			estimateColor.alpha = 1.0;
			if (s_listColor[bandID].red == s_listColor[bandID+1].red) {
				estimateColor.red = s_listColor[bandID].red;
			} else if (s_listColor[bandID].red < s_listColor[bandID+1].red) {
				estimateColor.red = s_listColor[bandID].red + (s_listColor[bandID+1].red-s_listColor[bandID].red)*poroportionnalPos;
			} else {
				estimateColor.red = s_listColor[bandID+1].red + (s_listColor[bandID].red-s_listColor[bandID+1].red)*(1-poroportionnalPos);
			}
			if (s_listColor[bandID].green == s_listColor[bandID+1].green) {
				estimateColor.green = s_listColor[bandID].green;
			} else if (s_listColor[bandID].green < s_listColor[bandID+1].green) {
				estimateColor.green = s_listColor[bandID].green + (s_listColor[bandID+1].green-s_listColor[bandID].green)*poroportionnalPos;
			} else {
				estimateColor.green = s_listColor[bandID+1].green + (s_listColor[bandID].green-s_listColor[bandID+1].green)*(1-poroportionnalPos);
			}
			if (s_listColor[bandID].blue == s_listColor[bandID+1].blue) {
				estimateColor.blue = s_listColor[bandID].blue;
			} else if (s_listColor[bandID].blue < s_listColor[bandID+1].blue) {
				estimateColor.blue = s_listColor[bandID].blue + (s_listColor[bandID+1].blue-s_listColor[bandID].blue)*poroportionnalPos;
			} else {
				estimateColor.blue = s_listColor[bandID+1].blue + (s_listColor[bandID].blue-s_listColor[bandID+1].blue)*(1-poroportionnalPos);
			}
			// step 2 generate the white and black ...
			if (relativePos.y == (m_size.y/2)) {
				// nothing to do ... just get the current color ...
			} else if (relativePos.y < (m_size.y/2)) {
				float poroportionnalWhite = 1.0-relativePos.y/(m_size.y/2);
				estimateColor.red   = estimateColor.red   + (1.0 - estimateColor.red  )*poroportionnalWhite;
				estimateColor.green = estimateColor.green + (1.0 - estimateColor.green)*poroportionnalWhite;
				estimateColor.blue  = estimateColor.blue  + (1.0 - estimateColor.blue )*poroportionnalWhite;
			} else {
				float poroportionnalBlack = (relativePos.y-(m_size.y/2))/(m_size.y/2);
				estimateColor.red   = estimateColor.red   - (estimateColor.red  )*poroportionnalBlack;
				estimateColor.green = estimateColor.green - (estimateColor.green)*poroportionnalBlack;
				estimateColor.blue  = estimateColor.blue  - (estimateColor.blue )*poroportionnalBlack;
			}
			/*
			char colorText[256];
			sprintf(colorText, "#%02X%02X%02X%02X",
			        (uint8_t)(estimateColor.red   * 0xFF),
			        (uint8_t)(estimateColor.green * 0xFF),
			        (uint8_t)(estimateColor.blue  * 0xFF),
			        (uint8_t)(estimateColor.alpha * 0xFF));
			EWOL_DEBUG("new color : " << colorText);
			*/
			if(    m_currentColor.red   != estimateColor.red
			    || m_currentColor.green != estimateColor.green
			    || m_currentColor.blue  != estimateColor.blue
			    || m_currentColor.alpha != estimateColor.alpha) {
				m_currentColor = estimateColor;
				GenerateEventId(ewolEventColorBarChange);
			}
			return true;
		}
	}
	return false;
}

/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/ColorBar.h>

#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>

#include <draw/Color.h>

extern const char * const ewolEventColorBarChange    = "ewol-color-bar-change";



#undef __class__
#define __class__	"ColorBar"

ewol::ColorBar::ColorBar(void)
{
	AddEventId(ewolEventColorBarChange);
	
	#ifdef __TARGET_OS__Android
		m_padding.y = 12;
		m_padding.x = 12;
	#else
		m_padding.y = 4;
		m_padding.x = 4;
	#endif
	m_currentUserPos.x=0;
	m_currentUserPos.y=0;
	m_currentColor = draw::color::black;
	SetCanHaveFocus(true);
}

ewol::ColorBar::~ColorBar(void)
{
	
}


bool ewol::ColorBar::CalculateMinSize(void)
{
	m_minSize.x = 80;
	m_minSize.y = 80;
	MarkToRedraw();
	return true;
}
static draw::Color s_listColorWhite(0xFFFFFFFF);
static draw::Color s_listColorBlack(0x000000FF);
#define NB_BAND_COLOR		(6)
static draw::Color s_listColor[NB_BAND_COLOR+1] = {
	0xFF0000FF,
	0xFFFF00FF,
	0x00FF00FF,
	0x00FFFFFF,
	0x0000FFFF,
	0xFF00FFFF,
	0xFF0000FF
};

draw::Color ewol::ColorBar::GetCurrentColor(void)
{
	return m_currentColor;
}
void ewol::ColorBar::SetCurrentColor(draw::Color newOne)
{
	m_currentColor = newOne;
	m_currentColor.a = 0xFF;
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
		
		if (true==m_userFill.x) {
			tmpSizeX = m_size.x;
			tmpOriginX = 0;
		}
		if (true==m_userFill.y) {
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
		draw::Color tmpColor;
		if (m_currentUserPos.y > 0.5) {
			tmpColor = draw::color::white;
		} else {
			tmpColor = draw::color::black;
		}
		tmpOObjects->SetColor(tmpColor);
		tmpOObjects->Circle(m_currentUserPos.x*m_size.x, m_currentUserPos.y*m_size.y, 3.0, 1.0);
		
		AddOObject(tmpOObjects);
	}
}


bool ewol::ColorBar::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, etk::Vector2D<float> pos)
{
	etk::Vector2D<float> relativePos = RelativePosition(pos);
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
			MarkToRedraw();
			//==> try to estimate color
			EWOL_VERBOSE("event on (" << relativePos.x << "," << relativePos.y << ")");
			int32_t bandID = (int32_t)(relativePos.x/(m_size.x/6));
			float localPos = relativePos.x - (m_size.x/6) * bandID;
			float poroportionnalPos = localPos/(m_size.x/6);
			EWOL_VERBOSE("bandId=" << bandID << "  relative pos=" << localPos);
			draw::Color estimateColor = draw::color::white;
			if (s_listColor[bandID].r == s_listColor[bandID+1].r) {
				estimateColor.r = s_listColor[bandID].r;
			} else if (s_listColor[bandID].r < s_listColor[bandID+1].r) {
				estimateColor.r = s_listColor[bandID].r + (s_listColor[bandID+1].r-s_listColor[bandID].r)*poroportionnalPos;
			} else {
				estimateColor.r = s_listColor[bandID+1].r + (s_listColor[bandID].r-s_listColor[bandID+1].r)*(1-poroportionnalPos);
			}
			if (s_listColor[bandID].g == s_listColor[bandID+1].g) {
				estimateColor.g = s_listColor[bandID].g;
			} else if (s_listColor[bandID].g < s_listColor[bandID+1].g) {
				estimateColor.g = s_listColor[bandID].g + (s_listColor[bandID+1].g-s_listColor[bandID].g)*poroportionnalPos;
			} else {
				estimateColor.g = s_listColor[bandID+1].g + (s_listColor[bandID].g-s_listColor[bandID+1].g)*(1-poroportionnalPos);
			}
			if (s_listColor[bandID].b == s_listColor[bandID+1].b) {
				estimateColor.b = s_listColor[bandID].b;
			} else if (s_listColor[bandID].b < s_listColor[bandID+1].b) {
				estimateColor.b = s_listColor[bandID].b + (s_listColor[bandID+1].b-s_listColor[bandID].b)*poroportionnalPos;
			} else {
				estimateColor.b = s_listColor[bandID+1].b + (s_listColor[bandID].b-s_listColor[bandID+1].b)*(1-poroportionnalPos);
			}
			// step 2 generate the white and black ...
			if (relativePos.y == (m_size.y/2)) {
				// nothing to do ... just get the current color ...
			} else if (relativePos.y < (m_size.y/2)) {
				float poroportionnalWhite = 1.0-relativePos.y/(m_size.y/2);
				estimateColor.r = estimateColor.r + (1.0 - estimateColor.r)*poroportionnalWhite;
				estimateColor.g = estimateColor.g + (1.0 - estimateColor.g)*poroportionnalWhite;
				estimateColor.b = estimateColor.b + (1.0 - estimateColor.b)*poroportionnalWhite;
			} else {
				float poroportionnalBlack = (relativePos.y-(m_size.y/2))/(m_size.y/2);
				estimateColor.r = estimateColor.r - (estimateColor.r)*poroportionnalBlack;
				estimateColor.g = estimateColor.g - (estimateColor.g)*poroportionnalBlack;
				estimateColor.b = estimateColor.b - (estimateColor.b)*poroportionnalBlack;
			}
			/*
			char colorText[256];
			sprintf(colorText, "#%02X%02X%02X%02X",
			        (uint8_t)(estimateColor.r   * 0xFF),
			        (uint8_t)(estimateColor.g * 0xFF),
			        (uint8_t)(estimateColor.b  * 0xFF),
			        (uint8_t)(estimateColor.a * 0xFF));
			EWOL_DEBUG("new color : " << colorText);
			*/
			if(    m_currentColor != estimateColor) {
				m_currentColor = estimateColor;
				GenerateEventId(ewolEventColorBarChange);
			}
			return true;
		}
	}
	return false;
}


/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/ColorBar.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>

#include <draw/Color.h>

extern const char * const ewolEventColorBarChange    = "ewol-color-bar-change";



#undef __class__
#define __class__	"ColorBar"

widget::ColorBar::ColorBar(void)
{
	AddEventId(ewolEventColorBarChange);
	m_currentUserPos.setValue(0,0);
	m_currentColor = draw::color::black;
	SetCanHaveFocus(true);
	SetMouseLimit(1);
}

widget::ColorBar::~ColorBar(void)
{
	
}


void widget::ColorBar::CalculateMinMaxSize(void)
{
	m_minSize.setValue(160, 80);
	MarkToRedraw();
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

draw::Color widget::ColorBar::GetCurrentColor(void)
{
	return m_currentColor;
}
void widget::ColorBar::SetCurrentColor(draw::Color newOne)
{
	m_currentColor = newOne;
	m_currentColor.a = 0xFF;
	// estimate the cursor position :
	// TODO : Later when really needed ...
}

void widget::ColorBar::OnDraw(void)
{
	m_draw.Draw();
}


void widget::ColorBar::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		m_draw.Clear();
		
		int32_t tmpSizeX = m_minSize.x();
		int32_t tmpSizeY = m_minSize.y();
		int32_t tmpOriginX = (m_size.x() - m_minSize.x()) / 2;
		int32_t tmpOriginY = (m_size.y() - m_minSize.y()) / 2;
		
		if (true==m_userFill.x()) {
			tmpSizeX = m_size.x();
			tmpOriginX = 0;
		}
		if (true==m_userFill.y()) {
			tmpSizeY = m_size.y();
			tmpOriginY = 0;
		}
		
		for(int32_t iii=0; iii<NB_BAND_COLOR ; iii++) {
			/* Step 1 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			m_draw.SetColor(s_listColorWhite);
			m_draw.SetPos(vec3(tmpOriginX + (iii)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY, 0) );
			m_draw.AddVertex();
			m_draw.SetColor(s_listColor[iii+1]);
			m_draw.SetPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0) );
			m_draw.AddVertex();
			m_draw.SetColor(s_listColor[iii]);
			m_draw.SetPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0) );
			m_draw.AddVertex();
			/* Step 2 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			m_draw.SetColor(s_listColorWhite);
			m_draw.SetPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY, 0) );
			m_draw.AddVertex();
			m_draw.SetColor(s_listColorWhite);
			m_draw.SetPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY, 0) );
			m_draw.AddVertex();
			m_draw.SetColor(s_listColor[iii+1]);
			m_draw.SetPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0) );
			m_draw.AddVertex();
			/* Step 3 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			m_draw.SetColor(s_listColor[iii]);
			m_draw.SetPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0));
			m_draw.AddVertex();
			m_draw.SetColor(s_listColorBlack);
			m_draw.SetPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY, 0));
			m_draw.AddVertex();
			m_draw.SetColor(s_listColorBlack);
			m_draw.SetPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY, 0));
			m_draw.AddVertex();
			/* Step 4 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			m_draw.SetColor(s_listColor[iii]);
			m_draw.SetPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0));
			m_draw.AddVertex();
			m_draw.SetColor(s_listColor[iii+1]);
			m_draw.SetPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0));
			m_draw.AddVertex();
			m_draw.SetColor(s_listColorBlack);
			m_draw.SetPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY, 0));
			m_draw.AddVertex();
		}
		if (m_currentUserPos.y() > 0.5) {
			m_draw.SetColor(draw::color::white);
		} else {
			m_draw.SetColor(draw::color::black);
		}
		m_draw.SetPos(vec3(m_currentUserPos.x()*m_size.x(), m_currentUserPos.y()*m_size.y(), 0) );
		m_draw.SetThickness(1);
		m_draw.Circle(3.0);
		
	}
}


bool widget::ColorBar::OnEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = RelativePosition(_event.GetPos());
	//EWOL_DEBUG("Event on BT ...");
	if (1 == _event.GetId()) {
		relativePos.setValue( etk_max(etk_min(relativePos.x(), m_size.x()),0),
		                      etk_max(etk_min(relativePos.y(), m_size.y()),0));
		if(    ewol::keyEvent::statusSingle == _event.GetStatus()
		    || ewol::keyEvent::statusMove   == _event.GetStatus()) {
			// nothing to do ...
			m_currentUserPos.setValue( relativePos.x()/m_size.x(),
			                           relativePos.y()/m_size.y() );
			MarkToRedraw();
			//==> try to estimate color
			EWOL_VERBOSE("event on (" << relativePos.x() << "," << relativePos.y() << ")");
			int32_t bandID = (int32_t)(relativePos.x()/(m_size.x()/6));
			float localPos = relativePos.x() - (m_size.x()/6) * bandID;
			float poroportionnalPos = localPos/(m_size.x()/6);
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
			if (m_currentUserPos.y() == 0.5) {
				// nothing to do ... just get the current color ...
			} else if (m_currentUserPos.y() < 0.5) {
				float poroportionnalWhite = (0.5-m_currentUserPos.y())*2.0;
				estimateColor.r = estimateColor.r + (0xFF-estimateColor.r)*poroportionnalWhite;
				estimateColor.g = estimateColor.g + (0xFF-estimateColor.g)*poroportionnalWhite;
				estimateColor.b = estimateColor.b + (0xFF-estimateColor.b)*poroportionnalWhite;
			} else {
				float poroportionnalBlack = (m_currentUserPos.y()-0.5)*2.0;
				estimateColor.r = estimateColor.r - estimateColor.r*poroportionnalBlack;
				estimateColor.g = estimateColor.g - estimateColor.g*poroportionnalBlack;
				estimateColor.b = estimateColor.b - estimateColor.b*poroportionnalBlack;
			}
			if(m_currentColor != estimateColor) {
				m_currentColor = estimateColor;
				GenerateEventId(ewolEventColorBarChange);
			}
			return true;
		}
	}
	return false;
}


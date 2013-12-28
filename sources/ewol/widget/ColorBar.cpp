/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/ColorBar.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>

#include <etk/Color.h>

const char * const ewol::widget::ColorBar::eventChange    = "change";



#undef __class__
#define __class__ "ColorBar"

ewol::widget::ColorBar::ColorBar(void) {
	addObjectType("ewol::widget::ColorBar");
	addEventId(eventChange);
	m_currentUserPos.setValue(0,0);
	m_currentColor = etk::color::black;
	setCanHaveFocus(true);
	setMouseLimit(1);
}

ewol::widget::ColorBar::~ColorBar(void) {
	
}


void ewol::widget::ColorBar::calculateMinMaxSize(void) {
	m_minSize.setValue(160, 80);
	markToRedraw();
}

static etk::Color<> s_listColorWhite(0xFFFFFFFF);
static etk::Color<> s_listColorBlack(0x000000FF);
#define NB_BAND_COLOR		(6)
static etk::Color<> s_listColor[NB_BAND_COLOR+1] = {
	0xFF0000FF,
	0xFFFF00FF,
	0x00FF00FF,
	0x00FFFFFF,
	0x0000FFFF,
	0xFF00FFFF,
	0xFF0000FF
};

etk::Color<> ewol::widget::ColorBar::getCurrentColor(void) {
	return m_currentColor;
}
void ewol::widget::ColorBar::setCurrentColor(etk::Color<> newOne) {
	m_currentColor = newOne;
	m_currentColor.setA(0xFF);
	// estimate the cursor position :
	// TODO : Later when really needed ...
}

void ewol::widget::ColorBar::onDraw(void) {
	m_draw.draw();
}


void ewol::widget::ColorBar::onRegenerateDisplay(void) {
	if (true == needRedraw()) {
		// clean the object list ...
		m_draw.clear();
		
		int32_t tmpSizeX = m_minSize.x();
		int32_t tmpSizeY = m_minSize.y();
		int32_t tmpOriginX = (m_size.x() - m_minSize.x()) / 2;
		int32_t tmpOriginY = (m_size.y() - m_minSize.y()) / 2;
		
		if (true == m_userFill.x()) {
			tmpSizeX = m_size.x();
			tmpOriginX = 0;
		}
		if (true == m_userFill.y()) {
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
			m_draw.setColor(s_listColorWhite);
			m_draw.setPos(vec3(tmpOriginX + (iii)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY, 0) );
			m_draw.addVertex();
			m_draw.setColor(s_listColor[iii+1]);
			m_draw.setPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0) );
			m_draw.addVertex();
			m_draw.setColor(s_listColor[iii]);
			m_draw.setPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0) );
			m_draw.addVertex();
			/* Step 2 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			m_draw.setColor(s_listColorWhite);
			m_draw.setPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY, 0) );
			m_draw.addVertex();
			m_draw.setColor(s_listColorWhite);
			m_draw.setPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY, 0) );
			m_draw.addVertex();
			m_draw.setColor(s_listColor[iii+1]);
			m_draw.setPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0) );
			m_draw.addVertex();
			/* Step 3 : 
			 *              
			 *   **         
			 *   ****       
			 *   ******     
			 *   ********   
			 */
			m_draw.setColor(s_listColor[iii]);
			m_draw.setPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0));
			m_draw.addVertex();
			m_draw.setColor(s_listColorBlack);
			m_draw.setPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY, 0));
			m_draw.addVertex();
			m_draw.setColor(s_listColorBlack);
			m_draw.setPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY, 0));
			m_draw.addVertex();
			/* Step 4 : 
			 *   ********     
			 *     ******     
			 *       ****     
			 *         **     
			 *                
			 */
			m_draw.setColor(s_listColor[iii]);
			m_draw.setPos(vec3(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0));
			m_draw.addVertex();
			m_draw.setColor(s_listColor[iii+1]);
			m_draw.setPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0));
			m_draw.addVertex();
			m_draw.setColor(s_listColorBlack);
			m_draw.setPos(vec3(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY, 0));
			m_draw.addVertex();
		}
		if (m_currentUserPos.y() > 0.5) {
			m_draw.setColor(etk::color::white);
		} else {
			m_draw.setColor(etk::color::black);
		}
		m_draw.setPos(vec3(m_currentUserPos.x()*m_size.x(), m_currentUserPos.y()*m_size.y(), 0) );
		m_draw.setThickness(1);
		m_draw.circle(3.0);
		
	}
}


bool ewol::widget::ColorBar::onEventInput(const ewol::event::Input& _event) {
	vec2 relativePos = relativePosition(_event.getPos());
	//EWOL_DEBUG("Event on BT ...");
	if (1 == _event.getId()) {
		relativePos.setValue( etk_max(etk_min(relativePos.x(), m_size.x()),0),
		                      etk_max(etk_min(relativePos.y(), m_size.y()),0));
		if(    ewol::key::statusSingle == _event.getStatus()
		    || ewol::key::statusMove   == _event.getStatus()) {
			// nothing to do ...
			m_currentUserPos.setValue( relativePos.x()/m_size.x(),
			                           relativePos.y()/m_size.y() );
			markToRedraw();
			// == > try to estimate color
			EWOL_VERBOSE("event on (" << relativePos.x() << "," << relativePos.y() << ")");
			int32_t bandID = (int32_t)(relativePos.x()/(m_size.x()/6));
			float localPos = relativePos.x() - (m_size.x()/6) * bandID;
			float poroportionnalPos = localPos/(m_size.x()/6);
			EWOL_VERBOSE("bandId=" << bandID << "  relative pos=" << localPos);
			etk::Color<> estimateColor = etk::color::white;
			if (s_listColor[bandID].r() == s_listColor[bandID+1].r()) {
				estimateColor.setR(s_listColor[bandID].r());
			} else if (s_listColor[bandID].r() < s_listColor[bandID+1].r()) {
				estimateColor.setR(s_listColor[bandID].r() + (s_listColor[bandID+1].r()-s_listColor[bandID].r())*poroportionnalPos);
			} else {
				estimateColor.setR(s_listColor[bandID+1].r() + (s_listColor[bandID].r()-s_listColor[bandID+1].r())*(1-poroportionnalPos));
			}
			if (s_listColor[bandID].g() == s_listColor[bandID+1].g()) {
				estimateColor.setG(s_listColor[bandID].g());
			} else if (s_listColor[bandID].g() < s_listColor[bandID+1].g()) {
				estimateColor.setG(s_listColor[bandID].g() + (s_listColor[bandID+1].g()-s_listColor[bandID].g())*poroportionnalPos);
			} else {
				estimateColor.setG(s_listColor[bandID+1].g() + (s_listColor[bandID].g()-s_listColor[bandID+1].g())*(1-poroportionnalPos));
			}
			if (s_listColor[bandID].b() == s_listColor[bandID+1].b()) {
				estimateColor.setB(s_listColor[bandID].b());
			} else if (s_listColor[bandID].b() < s_listColor[bandID+1].b()) {
				estimateColor.setB(s_listColor[bandID].b() + (s_listColor[bandID+1].b()-s_listColor[bandID].b())*poroportionnalPos);
			} else {
				estimateColor.setB(s_listColor[bandID+1].b() + (s_listColor[bandID].b()-s_listColor[bandID+1].b())*(1-poroportionnalPos));
			}
			// step 2 generate the white and black ...
			if (m_currentUserPos.y() == 0.5) {
				// nothing to do ... just get the current color ...
			} else if (m_currentUserPos.y() < 0.5) {
				float poroportionnalWhite = (0.5-m_currentUserPos.y())*2.0;
				estimateColor.setR(estimateColor.r() + (0xFF-estimateColor.r())*poroportionnalWhite);
				estimateColor.setG(estimateColor.g() + (0xFF-estimateColor.g())*poroportionnalWhite);
				estimateColor.setB(estimateColor.b() + (0xFF-estimateColor.b())*poroportionnalWhite);
			} else {
				float poroportionnalBlack = (m_currentUserPos.y()-0.5)*2.0;
				estimateColor.setR(estimateColor.r() - estimateColor.r()*poroportionnalBlack);
				estimateColor.setG(estimateColor.g() - estimateColor.g()*poroportionnalBlack);
				estimateColor.setB(estimateColor.b() - estimateColor.b()*poroportionnalBlack);
			}
			if(m_currentColor != estimateColor) {
				m_currentColor = estimateColor;
				generateEventId(eventChange);
			}
			return true;
		}
	}
	return false;
}


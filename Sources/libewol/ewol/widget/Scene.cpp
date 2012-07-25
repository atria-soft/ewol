/**
 *******************************************************************************
 * @file ewol/widget/Scene.cpp
 * @brief ewol Scene widget system (Sources)
 * @author Edouard DUPIN
 * @date 01/04/2012
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

#include <ewol/widget/Scene.h>
#include <math.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>
#include <ewol/importgl.h>

/**
 * @brief Initilise the basic widget property ==> due to the android system
 * @note all widget that have template might have this initializer ...
 * @param ---
 * @return ---
 */
void ewol::WIDGET_SceneInit(void)
{
	
}

#undef __class__
#define __class__	"Scene"

ewol::Scene::Scene(void)
{
	m_isRunning = true;
	SetCanHaveFocus(true);
	PeriodicCallSet(true);
	m_lastCallTime = -1;
	m_zoom = 1.0;
}


ewol::Scene::~Scene(void)
{
	
}


void ewol::Scene::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean elements
		for (int32_t iii=0; iii<m_sceneElement.animated[m_currentCreateId].Size(); iii++) {
			if (NULL != m_sceneElement.animated[m_currentCreateId][iii]) {
				m_sceneElement.animated[m_currentCreateId][iii]->Clear();
			}
		}
		for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
			for (int32_t iii=0; iii<m_sceneElement.listAnimatedElements[jjj].Size(); iii++) {
				if (NULL != m_sceneElement.listAnimatedElements[jjj][iii]) {
					// find an empty slot ...
					m_sceneElement.listAnimatedElements[jjj][iii]->Draw(m_currentCreateId);
				}
			}
		}
		m_needFlipFlop = true;
	}
}

/**
 * @brief Common widget drawing function (called by the drawing thread [Android, X11, ...])
 * @param ---
 * @return ---
 */
//TODO : Il y a un bug : seg fault ... je ne sais pas trop ou ...
void ewol::Scene::OnDraw(DrawProperty& displayProp)
{
	//EWOL_ERROR(" On draw : " << m_currentDrawId);
	// draw elements
	for (int32_t iii=0; iii<m_sceneElement.animated[m_currentDrawId].Size(); iii++) {
		if (NULL != m_sceneElement.animated[m_currentDrawId][iii]) {
			m_sceneElement.animated[m_currentDrawId][iii]->Draw();
		}
	}
}



/**
 * @brief Periodic call of this widget
 * @param localTime curent system time
 * @return ---
 */
void ewol::Scene::PeriodicCall(int64_t localTime)
{
	// First time : 
	if (-1 == m_lastCallTime) {
		m_lastCallTime = localTime;
	}
	// check if the processing is availlable
	if (false == m_isRunning) {
		m_lastCallTime = localTime;
		MarkToReedraw();
		return;
	}
	// cut the processing in small slot of time to prevent error in the real-time Display (Android call us between 30 to 60 fps)
	int32_t deltaTime = (int32_t) (localTime - m_lastCallTime);
	//EWOL_DEBUG(" currentTime = " << localTime << " last=" << m_lastCallTime << "  delta=" << deltaTime);
	while (deltaTime >= CYCLIC_CALL_PERIODE_US) {
		//EWOL_DEBUG(" process = " << CYCLIC_CALL_PERIODE_US);
		m_lastCallTime += CYCLIC_CALL_PERIODE_US;
		deltaTime -= CYCLIC_CALL_PERIODE_US;
		ScenePeriodicCall(m_lastCallTime, CYCLIC_CALL_PERIODE_US);
		//EWOL_ERROR("Periodic Call ... " << localTime);
		for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
			for (int32_t iii=0; iii<m_sceneElement.listAnimatedElements[jjj].Size(); iii++) {
				if (NULL != m_sceneElement.listAnimatedElements[jjj][iii]) {
					// check if the element request an auto Kill ...
					if (true == m_sceneElement.listAnimatedElements[jjj][iii]->Process(m_lastCallTime, CYCLIC_CALL_PERIODE_US) ) {
						m_sceneElement.RmElement(jjj, iii);
					}
				}
			}
		}
	}
	MarkToReedraw();
}

/**
 * @brief extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
 * @note This function is virtual for the scrolled widget, and the more complicated OpenGl widget
 * @param ---
 * @return ---
 */
void ewol::Scene::GenDraw(DrawProperty displayProp)
{
	glPushMatrix();
	// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
	glViewport( m_origin.x,
	            m_origin.y,
	            m_size.x,
	            m_size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrthoEwol(-m_size.x/2, m_size.x/2, -m_size.y/2, m_size.y/2, -100, 100);
	float ratio = m_size.x / m_size.y;
	//EWOL_INFO("ratio : " << ratio);
	if (ratio >= 0.0) {
		glOrthoEwol(-ratio, ratio, -1, 1, -1, 1);
	} else {
		ratio = 1.0/ratio;
		glOrthoEwol(-1, 1, -ratio, ratio, -1, 1);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	m_zoom = 1.0/1000.0;
	// Clear the screen with transparency ...
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glScalef(m_zoom, m_zoom, m_zoom);
	//glTranslatef(-m_maxSize.x/2, -m_maxSize.y/2, -1.0);
	
	// Call the widget drawing methode
	OnDraw(displayProp);
	glPopMatrix();
}

/**
 * @brief Convert the absolute position in the local Position (Relative)
 * @param[in] pos Absolute position that you request convertion
 * @return the relative position
 */
Vector2D<float> ewol::Scene::RelativePosition(Vector2D<float>  pos)
{
	pos.x -= m_origin.x;
	pos.y -= m_origin.y;
	pos.x -= m_size.x/2;
	pos.y -= m_size.y/2;
	pos.x *= m_zoom;
	pos.y *= m_zoom;
	
	return pos;
};
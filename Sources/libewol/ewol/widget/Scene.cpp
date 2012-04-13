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
	m_sceneElement.id = 1;
}


ewol::Scene::~Scene(void)
{
	
}

//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_SCENE = "Scene";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Scene::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_SCENE << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_SCENE) {
		return true;
	} else {
		if(true == ewol::WidgetScrooled::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_SCENE << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Scene::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_SCENE;
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
		// clean effects
		for (int32_t iii=0; iii<m_sceneElement.effects[m_currentCreateId].Size(); iii++) {
			if (NULL != m_sceneElement.effects[m_currentCreateId][iii]) {
				m_sceneElement.effects[m_currentCreateId][iii]->Clear();
			}
		}
		for (int32_t iii=0; iii<m_sceneElement.listAnimatedElements.Size(); iii++) {
			if (NULL != m_sceneElement.listAnimatedElements[iii]) {
				// find an empty slot ...
				m_sceneElement.listAnimatedElements[iii]->Draw(m_sceneElement.animated[m_currentCreateId], m_sceneElement.effects[m_currentCreateId]);
			}
		}
	}
}

/**
 * @brief Common widget drawing function (called by the drawing thread [Android, X11, ...])
 * @param ---
 * @return ---
 */
void ewol::Scene::OnDraw(void)
{
	//EWOL_ERROR(" On draw : " << m_currentDrawId);
	// draw background
	// TODO : ...
	// draw elements
	for (int32_t iii=0; iii<m_sceneElement.animated[m_currentDrawId].Size(); iii++) {
		if (NULL != m_sceneElement.animated[m_currentDrawId][iii]) {
			m_sceneElement.animated[m_currentDrawId][iii]->Draw();
		}
	}
	// draw effects
	for (int32_t iii=0; iii<m_sceneElement.effects[m_currentDrawId].Size(); iii++) {
		if (NULL != m_sceneElement.effects[m_currentDrawId][iii]) {
			m_sceneElement.effects[m_currentDrawId][iii]->Draw();
		}
	}
	m_needFlipFlop = true;
}


int32_t ewol::SceneElement::AddElement(ewol::GameElement* newElement)
{
	if (NULL == newElement) {
		return -1;
	}
	for (int32_t iii=0; iii<listAnimatedElements.Size(); iii++) {
		if (NULL == listAnimatedElements[iii]) {
			// find an empty slot ...
			listAnimatedElements[iii] = newElement;
			return iii;
		}
	}
	//did not find empty slot : 
	listAnimatedElements.PushBack(newElement);
	return listAnimatedElements.Size()-1;
}

#define CYCLIC_CALL_PERIODE_US     (10000)
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
		for (int32_t iii=0; iii<m_sceneElement.listAnimatedElements.Size(); iii++) {
			if (NULL != m_sceneElement.listAnimatedElements[iii]) {
				// check if the element request an auto Kill ...
				if (true == m_sceneElement.listAnimatedElements[iii]->Process(m_lastCallTime, CYCLIC_CALL_PERIODE_US, m_sceneElement) ) {
					delete(m_sceneElement.listAnimatedElements[iii]);
					m_sceneElement.listAnimatedElements[iii] = NULL;
				}
			}
		}
	}
	MarkToReedraw();
}



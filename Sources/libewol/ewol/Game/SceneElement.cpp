/**
 *******************************************************************************
 * @file ewol/Game/SceneElement.cpp
 * @brief ewol Scene widget system (Sources)
 * @author Edouard DUPIN
 * @date 17/04/2012
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

#include <ewol/Debug.h>
#include <ewol/Game/GameElement.h>
#include <ewol/Game/SceneElement.h>


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


bool ewol::SceneElement::GetElementProperty(gameElementGenericProperty_ts &element, int32_t id)
{
	for (int32_t iii=0; iii<listAnimatedElements.Size(); iii++) {
		if (NULL != listAnimatedElements[iii]) {
			if (true == listAnimatedElements[iii]->GetElementProperty(element, id) ) {
				return true;
			}
		}
	}
	return false;
}


int32_t ewol::SceneElement::GetNearestEnemy(coord2D_ts position, int32_t groupId)
{
	int32_t findId = -1;
	etkFloat_t lastQuadDistance = 9999999999999999.0;
	for (int32_t iii=0; iii<listAnimatedElements.Size(); iii++) {
		if (NULL != listAnimatedElements[iii]) {
			int32_t newID = listAnimatedElements[iii]->GetNearestEnemy(position, groupId, lastQuadDistance);
			if (-1 != newID) {
				findId = newID;
			}
		}
	}
	return findId;
}



bool ewol::SceneElement::HaveImpact(int32_t group, int32_t type, coord2D_ts position, etkFloat_t size)
{
	for (int32_t iii=0; iii<listAnimatedElements.Size(); iii++) {
		if (NULL != listAnimatedElements[iii]) {
			if (true == listAnimatedElements[iii]->HaveImpact(group, type, position, size )) {
				return true;
			}
		}
	}
	return false;
}

void ewol::SceneElement::Explosion(int32_t group, int32_t type, coord2D_ts position, etkFloat_t pxAtenuation, etkFloat_t power)
{
	for (int32_t iii=0; iii<listAnimatedElements.Size(); iii++) {
		if (NULL != listAnimatedElements[iii]) {
			listAnimatedElements[iii]->Explosion(group, type, position, pxAtenuation, power);
		}
	}
}
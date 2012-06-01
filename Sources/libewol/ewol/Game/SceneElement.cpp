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
#include <ewol/Game/GameElementLua.h>
#include <ewol/Game/SceneElement.h>

static uint32_t createUniqueId(uint16_t uniqueID, uint16_t position)
{
	return ((uint32_t)uniqueID<< 16) + (uint32_t)position;
}


ewol::SceneElement::SceneElement(void)
{
	m_id = 1;
	numberOfGroup = 0;
	for (int32_t iii=0; iii<MAX_GROUP_NUMBER; iii++) {
		for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
			groupEnemy[iii][jjj] = -1;
		}
	}
};

uint32_t ewol::SceneElement::AddElement(int32_t group, ewol::GameElement* newElement)
{
	if (NULL == newElement) {
		EWOL_ERROR("newElement is empty ==> not added at the system ...");
		return 0;
	}
	if (group < 0 || group >= MAX_GROUP_NUMBER) {
		EWOL_ERROR("group is wrong " << group << "!=[0," << MAX_GROUP_NUMBER << "]==> not added at the system ...");
		return 0;
	}
	for (int32_t iii=0; iii<listAnimatedElements[group].Size(); iii++) {
		if (NULL == listAnimatedElements[group][iii]) {
			// find an empty slot ...
			listAnimatedElements[group][iii] = newElement;
			return createUniqueId(newElement->GetUniqueId(), iii);
		}
	}
	//did not find empty slot :
	listAnimatedElements[group].PushBack(newElement);
	if (listAnimatedElements[group].Size()>0) {
		return createUniqueId(newElement->GetUniqueId(), listAnimatedElements[group].Size()-1);
	} else {
		return 0;
	}
}

uint32_t ewol::SceneElement::AddElementNamed(int32_t group, etk::UString &elementName)
{
	// try to find the file :
	etk::UString tmpName = "elementGame/";
	tmpName += elementName;
	tmpName += ".lua";
	etk::File fileElement(tmpName, etk::FILE_TYPE_DATA);
	if (false == fileElement.Exist()) {
		EWOL_ERROR("Can not find Game element : " << elementName << " ==> " << tmpName);
		return 0;
	}
	EWOL_VERBOSE("We find Game element : " << elementName << " ==> " << tmpName);
	ewol::GameElementLua * tmpElement = new ewol::GameElementLua(*this, tmpName, group);
	if (NULL == tmpElement) {
		EWOL_ERROR("Can not Allocat : " << elementName);
		return 0;
	}
	return AddElement(group, tmpElement);
}


ewol::GameElement* ewol::SceneElement::GetElement(uint32_t idElement)
{
	if (0 == idElement) {
		return NULL;
	}
	uint16_t realUniqueId = (uint16_t)((idElement >> 16 ) & 0x0000FFFF);
	uint16_t posInList    = (uint16_t)(idElement & 0x0000FFFF);
	for (int32_t iii=0; iii<numberOfGroup; iii++) {
		if(    posInList < listAnimatedElements[iii].Size()
		    && NULL != listAnimatedElements[iii][posInList]
		    && realUniqueId == listAnimatedElements[iii][posInList]->GetUniqueId()) {
			return listAnimatedElements[iii][posInList];
		}
	}
	return NULL;
}


uint32_t ewol::SceneElement::GetNearestEnemy(coord2D_ts position, int32_t groupId)
{
	uint32_t result = 0;
	etkFloat_t lastQuadDistance = 9999999999999999.0;
	int32_t jjj=0;
	if (groupId <0 || groupId >= MAX_GROUP_NUMBER) {
		EWOL_ERROR("incorect group number : " << groupId);
		return 0;
	}
	while (groupEnemy[groupId][jjj] != -1) {
		for (int32_t iii=0; iii<listAnimatedElements[groupEnemy[groupId][jjj]].Size(); iii++) {
			if (NULL != listAnimatedElements[groupEnemy[groupId][jjj]][iii]) {
				coord2D_ts tmpPos = listAnimatedElements[groupEnemy[groupId][jjj]][iii]->PositionGet();
				etkFloat_t distance = quadDist(position, tmpPos);
				if (distance <= lastQuadDistance) {
					lastQuadDistance = distance;
					result = createUniqueId(listAnimatedElements[groupEnemy[groupId][jjj]][iii]->GetUniqueId(), iii);
				}
			}
		}
		jjj++;
	}
	return result;
}


bool ewol::SceneElement::HaveImpact(int32_t group, int32_t type, coord2D_ts position, etkFloat_t size)
{
	for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
		for (int32_t iii=0; iii<listAnimatedElements[jjj].Size(); iii++) {
			if (NULL != listAnimatedElements[jjj][iii]) {
				if (true == listAnimatedElements[jjj][iii]->HaveImpact(group, type, position, size )) {
					return true;
				}
			}
		}
	}
	return false;
}

void ewol::SceneElement::Explosion(int32_t group, int32_t type, coord2D_ts position, etkFloat_t pxAtenuation, etkFloat_t power)
{
	for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
		for (int32_t iii=0; iii<listAnimatedElements[jjj].Size(); iii++) {
			if (NULL != listAnimatedElements[jjj][iii]) {
				listAnimatedElements[jjj][iii]->Explosion(group, type, position, pxAtenuation, power);
			}
		}
	}
}


uint32_t ewol::SceneElement::GetElementAtPos(coord2D_ts position, int32_t maxDistanceDetection)
{
	uint32_t result = 0;
	etkFloat_t lastQuadDistance = 9999999999999999.0;
	for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
		for (int32_t iii=0; iii<listAnimatedElements[jjj].Size(); iii++) {
			if (NULL != listAnimatedElements[jjj][iii]) {
				coord2D_ts tmpPos = listAnimatedElements[jjj][iii]->PositionGet();
				etkFloat_t distance = quadDist(position, tmpPos);
				if (distance <= lastQuadDistance) {
					lastQuadDistance = distance;
					result = createUniqueId(listAnimatedElements[jjj][iii]->GetUniqueId(), iii);
				}
			}
		}
	}
	if (lastQuadDistance > maxDistanceDetection*maxDistanceDetection) {
		return 0;
	}
	return result;
}

void ewol::SceneElement::SetEventInput(uint32_t id, coord2D_ts position)
{
	EWOL_TODO("but when ...");
}

void ewol::SceneElement::SetEventExternButton(uint32_t id, int32_t btId, int32_t state)
{
	EWOL_TODO("but when ...");
}

void ewol::SceneElement::SetEventExternJoystick(uint32_t id, int32_t joyId, etkFloat_t angle, etkFloat_t distance, int32_t state)
{
	EWOL_TODO("but when ...");
}


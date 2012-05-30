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

void ewol::SceneElement::AddElement(int32_t group, ewol::GameElement* newElement)
{
	elementIdentifier_ts tmpElement;
	if (NULL == newElement) {
		EWOL_ERROR("newElement is empty ==> not added at the system ...");
		return;
	}
	if (group < 0 || group >= MAX_GROUP_NUMBER) {
		EWOL_ERROR("group is wrong " << group << "!=[0," << MAX_GROUP_NUMBER << "]==> not added at the system ...");
		return;
	}
	for (int32_t iii=0; iii<listAnimatedElements[group].Size(); iii++) {
		if (NULL == listAnimatedElements[group][iii]) {
			// find an empty slot ...
			listAnimatedElements[group][iii] = newElement;
			return;
		}
	}
	//did not find empty slot : 
	listAnimatedElements[group].PushBack(newElement);
}

void ewol::SceneElement::AddElementNamed(int32_t group, etk::UString &elementName)
{
	// try to find the file :
	etk::UString tmpName = "elementGame/";
	tmpName += elementName;
	tmpName += ".lua";
	etk::File fileElement(tmpName, etk::FILE_TYPE_DATA);
	if (false == fileElement.Exist()) {
		EWOL_ERROR("Can not find Game element : " << elementName << " ==> " << tmpName);
		return;
	}
	EWOL_INFO("We find Game element : " << elementName << " ==> " << tmpName);
	ewol::GameElementLua * tmpElement = new ewol::GameElementLua(*this, tmpName, group);
	if (NULL == tmpElement) {
		EWOL_ERROR("Can not Allocat : " << elementName);
		return;
	}
	AddElement(group, tmpElement);
}

bool ewol::SceneElement::GetElementProperty(gameElementGenericProperty_ts &element, elementIdentifier_ts& id)
{
	if (id.group > numberOfGroup || id.group > MAX_GROUP_NUMBER) {
		return false;
	}
	if (id.listId >= listAnimatedElements[id.group].Size()) {
		return false;
	}
	if (NULL == listAnimatedElements[id.group][id.listId]) {
		return false;
	}
	// copy the properties
	element.id = id;
	// get the sub properties ...
	return listAnimatedElements[id.group][id.listId]->GetElementProperty(element, id.id);
}


ewol::elementIdentifier_ts ewol::SceneElement::GetNearestEnemy(coord2D_ts position, int32_t groupId)
{
	elementIdentifier_ts findId;
	findId.id = -1;
	findId.group = -1;
	findId.listId = -1;
	etkFloat_t lastQuadDistance = 9999999999999999.0;
	int32_t jjj=0;
	while (groupEnemy[groupId][jjj] != -1) {
		for (int32_t iii=0; iii<listAnimatedElements[groupEnemy[groupId][jjj]].Size(); iii++) {
			if (NULL != listAnimatedElements[groupEnemy[groupId][jjj]][iii]) {
				int32_t newID = listAnimatedElements[groupEnemy[groupId][jjj]][iii]->GetNearestEnemy(position, lastQuadDistance);
				if (-1 != newID) {
					findId.id = newID;
					findId.group = groupEnemy[groupId][jjj];
					findId.listId = iii;
				}
			}
		}
		jjj++;
	}
	return findId;
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

ewol::elementIdentifier_ts ewol::SceneElement::GetElementAtPos(coord2D_ts position, int32_t maxDistanceDetection)
{
	elementIdentifier_ts findId;
	findId.id = -1;
	findId.group = -1;
	findId.listId = -1;
	etkFloat_t lastQuadDistance = 9999999999999999.0;
	int32_t jjj=0;
	for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
		for (int32_t iii=0; iii<listAnimatedElements[jjj].Size(); iii++) {
			if (NULL != listAnimatedElements[jjj][iii]) {
				int32_t newID = listAnimatedElements[jjj][iii]->GetNearestEnemy(position, lastQuadDistance);
				if (-1 != newID) {
					findId.id = newID;
					findId.group = jjj;
					findId.listId = iii;
				}
			}
		}
	}
	if (lastQuadDistance > maxDistanceDetection*maxDistanceDetection) {
		findId.id = -1;
		findId.group = -1;
		findId.listId = -1;
	}
	return findId;
}

void ewol::SceneElement::SetEventInput(elementIdentifier_ts& id, coord2D_ts position)
{
	EWOL_TODO("but when ...");
}

void ewol::SceneElement::SetEventExternButton(elementIdentifier_ts& id, int32_t btId, int32_t state)
{
	EWOL_TODO("but when ...");
}

void ewol::SceneElement::SetEventExternJoystick(elementIdentifier_ts& id, int32_t joyId, etkFloat_t angle, etkFloat_t distance, int32_t state)
{
	EWOL_TODO("but when ...");
}


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

#include <ewol/ewol.h>
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
	numberOfGroup = MAX_GROUP_NUMBER;
	for (int32_t iii=0; iii<MAX_GROUP_NUMBER; iii++) {
		int32_t kkk = 0;
		for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER-1; jjj++) {
			if (iii == kkk) {
				kkk++;
			}
			groupEnemy[iii][jjj] = kkk;
			kkk++;
		}
		groupEnemy[iii][MAX_GROUP_NUMBER-1] = -1;
	}
	retreviveElement = 0;
	allocatedElements = 0;
}

ewol::SceneElement::~SceneElement(void)
{
	EWOL_DEBUG("Remove sceane, allocated element : " << allocatedElements << " and retreive : " << retreviveElement);
	// clean all element allocated :
	for (int32_t jjj=0; jjj<listGarbage.size(); jjj++) {
		if (NULL != listGarbage[jjj]) {
			delete(listGarbage[jjj]);
			listGarbage[jjj] = NULL;
		}
	}
	listGarbage.clear();
	for (int32_t jjj=0; jjj<listCreatorElement.size(); jjj++) {
		if (NULL != listCreatorElement[jjj]) {
			delete(listCreatorElement[jjj]);
			listCreatorElement[jjj] = NULL;
		}
	}
	listCreatorElement.clear();
	for (int32_t iii=0; iii<MAX_GROUP_NUMBER; iii++) {
		for (int32_t jjj=0; jjj<listAnimatedElements[iii].size(); jjj++) {
			if (NULL != listAnimatedElements[iii][jjj]) {
				delete(listAnimatedElements[iii][jjj]);
				listAnimatedElements[iii][jjj] = NULL;
			}
		}
		listAnimatedElements[iii].clear();
	}
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		for (int32_t jjj=0; jjj<animated[iii].size(); jjj++) {
			if (NULL != animated[iii][jjj]) {
				delete(animated[iii][jjj]);
				animated[iii][jjj] = NULL;
			}
		}
		animated[iii].clear();
	}
}

void ewol::SceneElement::RegisterElementType(etk::UString name, creatorElement_tf * loadElement, etk::UString userString)
{
	// TODO : Check if the element already existed
	ewol::listRegisteElement * tmpElement = new listRegisteElement();
	if (NULL == tmpElement) {
		EWOL_ERROR("Memory error in allocation registered element");
		return;
	}
	tmpElement->name = name;
	tmpElement->userString = userString;
	tmpElement->loadElement = loadElement;
	listCreatorElement.push_back(tmpElement);
}


void ewol::SceneElement::RmElement(int32_t group, int32_t idElement)
{
	if (group < 0 || group >= MAX_GROUP_NUMBER) {
		EWOL_ERROR("group is wrong " << group << "!=[0," << MAX_GROUP_NUMBER << "]==> not rm at the system ...");
		return;
	}
	if (idElement < 0 || idElement >= listAnimatedElements[group].size()) {
		EWOL_ERROR("idElement is wrong " << idElement << "!=[0," << listAnimatedElements[group].size() << "]==> not rm at the system ...");
		return;
	}
	if (NULL == listAnimatedElements[group][idElement]) {
		return;
	}
	// try to find an empty slot : 
	for (int32_t iii=0; iii<listGarbage.size(); iii++) {
		if (NULL == listGarbage[iii]) {
			// find an empty slot ...
			listGarbage[iii] = listAnimatedElements[group][idElement];
			listAnimatedElements[group][idElement] = NULL;
			return;
		}
	}
	listAnimatedElements[group][idElement]->UnInit();
	listGarbage.push_back(listAnimatedElements[group][idElement]);
	listAnimatedElements[group][idElement] = NULL;
	return;
}

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
	// for statistic
	newElement->Init();
	newElement->GroupSet(group);
	for (int32_t iii=0; iii<listAnimatedElements[group].size(); iii++) {
		if (NULL == listAnimatedElements[group][iii]) {
			// find an empty slot ...
			listAnimatedElements[group][iii] = newElement;
			return createUniqueId(newElement->GetUniqueId(), iii);
		}
	}
	//did not find empty slot :
	listAnimatedElements[group].push_back(newElement);
	if (listAnimatedElements[group].size()>0) {
		return createUniqueId(newElement->GetUniqueId(), listAnimatedElements[group].size()-1);
	} else {
		return 0;
	}
}

uint32_t ewol::SceneElement::AddElementNamed(int32_t group, etk::UString &elementName)
{
	// try to fined it in the garbase :
	for (int32_t iii=0; iii<listGarbage.size(); iii++) {
		if (NULL != listGarbage[iii]) {
			// check his name : 
			if (true == listGarbage[iii]->HasName(elementName)) {
				// we find a previous element loaded ==> retreve it
				int32_t idElementBackAdded = AddElement(group, listGarbage[iii]);
				listGarbage[iii] = NULL;
				retreviveElement++;
				return idElementBackAdded;
			}
		}
	}
	ewol::GameElement* newElement=NULL;
	// find in registered element
	for (int32_t iii=0; iii<listCreatorElement.size(); iii++) {
		if (NULL != listCreatorElement[iii]) {
			// check his name : 
			if (listCreatorElement[iii]->name == elementName) {
				// create a new element : 
				allocatedElements ++;
				newElement = (*listCreatorElement[iii]->loadElement)(*this, elementName, listCreatorElement[iii]->userString);
				// we find a previous element loaded ==> retreve it
				return AddElement(group, newElement);
			}
		}
	}
	allocatedElements++;
	return AddElement(group, newElement);
}


ewol::GameElement* ewol::SceneElement::GetElement(uint32_t idElement)
{
	if (0 == idElement) {
		return NULL;
	}
	uint16_t realUniqueId = (uint16_t)((idElement >> 16 ) & 0x0000FFFF);
	uint16_t posInList    = (uint16_t)(idElement & 0x0000FFFF);
	for (int32_t iii=0; iii<numberOfGroup; iii++) {
		if(    posInList < listAnimatedElements[iii].size()
		    && NULL != listAnimatedElements[iii][posInList]
		    && realUniqueId == listAnimatedElements[iii][posInList]->GetUniqueId()) {
			return listAnimatedElements[iii][posInList];
		}
	}
	return NULL;
}


uint32_t ewol::SceneElement::GetNearestEnemy(Vector2D<float> position, int32_t groupId)
{
	uint32_t result = 0;
	float lastQuadDistance = 9999999999999999.0;
	int32_t jjj=0;
	if (groupId <0 || groupId >= MAX_GROUP_NUMBER) {
		EWOL_ERROR("incorect group number : " << groupId);
		return 0;
	}
	while (groupEnemy[groupId][jjj] != -1) {
		int32_t gId = groupEnemy[groupId][jjj];
		if (gId == groupId) {
			EWOL_ERROR("groupId=" << gId << " is ennemy of groupId:" << groupId);
		}
		for (int32_t iii=0; iii<listAnimatedElements[gId].size(); iii++) {
			if (NULL != listAnimatedElements[gId][iii]) {
				if (true == listAnimatedElements[gId][iii]->CanBeCibledGet()) {
					Vector2D<float> tmpPos = listAnimatedElements[gId][iii]->PositionGet();
					float distance = quadDist(position, tmpPos);
					if (distance <= lastQuadDistance) {
						lastQuadDistance = distance;
						result = createUniqueId(listAnimatedElements[gId][iii]->GetUniqueId(), iii);
					}
				}
			}
		}
		jjj++;
	}
	return result;
}


bool ewol::SceneElement::HaveImpact(int32_t group, int32_t type, Vector2D<float> position, float size)
{
	for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
		if (group != jjj) {
			for (int32_t iii=0; iii<listAnimatedElements[jjj].size(); iii++) {
				if (NULL != listAnimatedElements[jjj][iii]) {
					if (true == listAnimatedElements[jjj][iii]->HaveImpact(group, type, position, size )) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

void ewol::SceneElement::Explosion(int32_t group, int32_t type, Vector2D<float> position, float pxAtenuation, float power)
{
	for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
		for (int32_t iii=0; iii<listAnimatedElements[jjj].size(); iii++) {
			if (NULL != listAnimatedElements[jjj][iii]) {
				if (true == listAnimatedElements[jjj][iii]->Explosion(group, type, position, pxAtenuation, power) ) {
					RmElement(jjj, iii);
				}
			}
		}
	}
}


uint32_t ewol::SceneElement::GetElementAtPos(Vector2D<float> position, int32_t maxDistanceDetection)
{
	uint32_t result = 0;
	float lastQuadDistance = 9999999999999999.0;
	for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
		for (int32_t iii=0; iii<listAnimatedElements[jjj].size(); iii++) {
			if (NULL != listAnimatedElements[jjj][iii]) {
				Vector2D<float> tmpPos = listAnimatedElements[jjj][iii]->PositionGet();
				float distance = quadDist(position, tmpPos);
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

void ewol::SceneElement::SetEventInput(uint32_t id, Vector2D<float> position)
{
	EWOL_TODO("but when ...");
}

void ewol::SceneElement::SetEventExternButton(uint32_t id, int32_t btId, int32_t state)
{
	EWOL_TODO("but when ...");
}

void ewol::SceneElement::SetEventExternJoystick(uint32_t id, int32_t joyId, float angle, float distance, int32_t state)
{
	EWOL_TODO("but when ...");
}


/**
 * @brief Load or get a previous loaded sprite, it will be done on the current Sprite list
 * @param[in,out] listOfElement Reference on the list of sprite that we need to find if it exist or added a new one
 * @param[in] fileName Sprite name
 * @param[in] maxSize maximum size of the sprite
 * @return the id of the sprite requested or -1 if it does not existed
 */
int32_t ewol::SceneElement::LoadSprite(etk::UString fileName, float maxSize)
{
	for (int32_t iii=0; iii<animated[0].size(); iii++) {
		if (animated[0][iii] != NULL) {
			if (animated[0][iii]->HasName(fileName) == true) {
				// count the number of element registered ...
				animated[0][iii]->IncreaseLoadedTime();
				return iii;
			}
		}
	}
	for(int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		// we did not find the sprite ==> created it ...
		ewol::Sprite* tmpSprite = new ewol::Sprite(fileName, maxSize, maxSize);
		if (NULL == tmpSprite) {
			EWOL_ERROR("Allocation error on the sprite : " << fileName);
			return -1;
		}
		// add it : 
		animated[iii].push_back(tmpSprite);
	}
	return animated[0].size() -1;
}

/**
 * @brief UnLoad or not(if needed) the sprite selected, it will be done on the current Sprite list
 * @param[in,out] listOfElement Reference on the list of sprite that we need to find if it exist or added a new one
 * @param[in] spriteId Sprite registered id
 * @return ---
 */
void ewol::SceneElement::UnLoadSprite(int32_t spriteId)
{
	if (spriteId >= 0 && spriteId < animated[0].size()) {
		if (animated[0][spriteId] != NULL) {
			// count the number of element registered ...
			if (true == animated[0][spriteId]->DecreaseLoadedTime() ) {
				// must remove the sprite ==> pb with the double buffer ...
				// TODO : ==> for all double buffer ...
				for(int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
					
				}
			}
		}
	}
}
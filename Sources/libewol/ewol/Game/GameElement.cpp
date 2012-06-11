/**
 *******************************************************************************
 * @file ewol/Game/GameElement.cpp
 * @brief ewol game element system (Sources)
 * @author Edouard DUPIN
 * @date 06/04/2012
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


#include <ewol/Game/GameElement.h>


#undef __class__
#define __class__	"GameElement"

/**
 * @brief Constructor : here are requested all the needed sprite and effect that can be used in the game
 * @param ---
 * @return ---
 */
ewol::GameElement::GameElement(SceneElement & sceneElement, etk::UString& tmpName) : m_fileNameConfig(tmpName), m_sceneElement(sceneElement)
{
	m_uniqueId = sceneElement.GetUniqueId();
	m_group = -1;
	m_type = -1;
	m_visible = true;
	m_position.x = 0.0;
	m_position.y = 0.0;
	m_speed.x = 0.0;
	m_speed.y = 0.0;
	m_size = 64.0;
	m_angle = 0.0;
	m_gravity = 0.0;
	m_fileNameConfig = tmpName;
	m_canBeCibled = false;
	m_life = 0;
}


/**
 * @brief Load or get a previous loaded sprite, it will be done on the current Sprite list
 * @param[in,out] listOfElement Reference on the list of sprite that we need to find if it exist or added a new one
 * @param[in] fileName Sprite name
 * @param[in] maxSize maximum size of the sprite
 * @return the id of the sprite requested or -1 if it does not existed
 */
int32_t ewol::GameElement::LoadSprite(etk::VectorType<ewol::Sprite*> listOfElement[NB_BOUBLE_BUFFER], etk::UString fileName, Vector2D<float> maxSize)
{
	for (int32_t iii=0; iii<listOfElement[0].Size(); iii++) {
		if (listOfElement[0][iii] != NULL) {
			if (listOfElement[0][iii]->HasName(fileName) == true) {
				// count the number of element registered ...
				listOfElement[0][iii]->IncreaseLoadedTime();
				return iii;
			}
		}
	}
	for(int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		// we did not find the sprite ==> created it ...
		ewol::Sprite* tmpSprite = new ewol::Sprite(fileName, maxSize.x, maxSize.y);
		if (NULL == tmpSprite) {
			EWOL_ERROR("Allocation error on the sprite : " << fileName);
			return -1;
		}
		// add it : 
		listOfElement[iii].PushBack(tmpSprite);
	}
	return listOfElement[0].Size() -1;
}

/**
 * @brief UnLoad or not(if needed) the sprite selected, it will be done on the current Sprite list
 * @param[in,out] listOfElement Reference on the list of sprite that we need to find if it exist or added a new one
 * @param[in] spriteId Sprite registered id
 * @return ---
 */
void ewol::GameElement::UnLoadSprite(etk::VectorType<ewol::Sprite*> listOfElement[NB_BOUBLE_BUFFER], int32_t spriteId)
{
	if (spriteId >= 0 && spriteId < listOfElement[0].Size()) {
		if (listOfElement[0][spriteId] != NULL) {
			// count the number of element registered ...
			if (true == listOfElement[0][spriteId]->DecreaseLoadedTime() ) {
				// must remove the sprite ==> pb with the double buffer ...
				// TODO : ==> for all double buffer ...
				for(int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
					
				}
			}
		}
	}
}


float quadDist(Vector2D<float> pos1, Vector2D<float> pos2)
{
	float tmpVal1 = pos1.x - pos2.x;
	float tmpVal2 = pos1.y - pos2.y;
	
	return tmpVal1*tmpVal1 + tmpVal2*tmpVal2;
}


void ewol::GameElement::GetElementProperty(gameElementGenericProperty_ts &element)
{
	element.type = m_type;
	element.position = m_position;
	element.speed.x = m_speed.x;
	element.speed.y = m_speed.y;
	element.size.x = m_size;
	element.size.y = m_size;
	element.angle = m_angle;
}


bool ewol::GameElement::HaveImpact(int32_t group, int32_t type, Vector2D<float> position, float size)
{
	// check if it was in the same group
	if (group == m_group) {
		return false;
	}
	float quadDistance = quadDist(m_position, position);
	float radiusElement = m_size * m_size;
	if (radiusElement < quadDistance) {
		//distance is greten than expected
		return false;
	}
	return true;
}


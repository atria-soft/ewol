/**
 *******************************************************************************
 * @file ewol/game/GameElement.cpp
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


#include <ewol/game/GameElement.h>


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
	m_speed = 0.0;
	m_size = 64.0;
	m_sizeDisplay = 64.0;
	m_angle = 0.0;
	m_mass = 0.0;
	m_fileNameConfig = tmpName;
	m_canBeCibled = false;
	m_canHaveImpact = true;
	m_life = 0;
	m_enable = true;
}




float quadDist(Vector2D<float> pos1, Vector2D<float> pos2)
{
	float tmpVal1 = pos1.x - pos2.x;
	float tmpVal2 = pos1.y - pos2.y;
	
	return tmpVal1*tmpVal1 + tmpVal2*tmpVal2;
}


bool ewol::GameElement::HaveImpact(int32_t group, int32_t type, Vector2D<float> position, float size)
{
	if (true != m_canHaveImpact) {
		return false;
	}
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


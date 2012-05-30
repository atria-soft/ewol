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


/**
 * @brief Constructor : here are requested all the needed sprite and effect that can be used in the game
 * @param ---
 * @return ---
 */
ewol::GameElement::GameElement(SceneElement & sceneElement, etk::UString& tmpName) : m_fileNameConfig(tmpName), m_sceneElement(sceneElement)
{
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
}


/**
 * @brief Requuest the draw of the current element, it will be done on the current Sprite list
 * @param[in,out] listOfElement Reference on the list of sprite that we need to find if it exist or added a new one
 * @param[in] fileName Sprite name
 * @param[in] maxSize maximum size of the sprite
 * @return the id of the sprite requested or -1 if it does not existed
 */
int32_t ewol::GameElement::LoadSprite(etk::VectorType<ewol::Sprite*> listOfElement[NB_BOUBLE_BUFFER], etk::UString fileName, coord2D_ts maxSize)
{
	for (int32_t iii=0; iii<listOfElement[0].Size(); iii++) {
		if (listOfElement[0][iii] != NULL) {
			if (listOfElement[0][iii]->HasName(fileName) == true) {
				// TODO : control the size ...
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


etkFloat_t quadDist(coord2D_ts pos1, coord2D_ts pos2)
{
	etkFloat_t tmpVal1 = pos1.x - pos2.x;
	etkFloat_t tmpVal2 = pos1.y - pos2.y;
	
	return tmpVal1*tmpVal1 + tmpVal2*tmpVal2;
}


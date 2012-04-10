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
ewol::GameElement::GameElement(void)
{
	m_visible = true;
	m_position.x = 0.0;
	m_position.y = 0.0;
	m_acceleration.x = 0.0;
	m_acceleration.y = 0.0;
	m_size = 64.0;
	m_angle = 0.0;
}


/**
 * @brief Destructor : This does not remove the sprite requested, they will be supressed when the scene is removed ...
 * @param ---
 * @return ---
 */
ewol::GameElement::~GameElement(void)
{
	
}


/**
 * @brief Periodicly this fuction will be call tu change property of all the dynamic obbjects
 * @param[in] time Current game time (start at 0)
 * @param[in] deltaTime delta time before the previous call
 * @param[in,out] listOfSprite Reference on the list where the display must be done for every sprite
 * @param[in,out] listOfEffects Reference on the list where the display must be done for every effects
 * @return true if the object must be remove
 * @return false if the object must be keep
 */
bool ewol::GameElement::Process(int64_t time, int32_t deltaTime, etk::VectorType<ewol::GameElement*> & listOfElement)
{
	// nothing to do here ...
	return false;
}


/**
 * @brief Requuest the draw of the current element, it will be done on the current Sprite list
 * @param[in,out] listOfSprite Reference on the list where the display must be done for every sprite
 * @param[in,out] listOfEffects Reference on the list where the display must be done for every effects
 * @return ---
 */
void ewol::GameElement::Draw(etk::VectorType<ewol::Sprite*> & listOfSprite, etk::VectorType<ewol::Sprite*> & listOfEffects)
{
	// nothing to do here ...
}


/**
 * @brief an element has been remove, just remove reference on it or ID on IT, it can be replace whith an other that have no link
 * @param[in] idOfElement Id of the element that has been removed
 * @return ---
 */
void ewol::GameElement::RemoveElement(int32_t idOfElement)
{
	// nothing to do here ...
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


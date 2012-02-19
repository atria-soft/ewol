/**
 *******************************************************************************
 * @file ewol/widget/Drawable.cpp
 * @brief basic ewol drawable widget (Sources)
 * @author Edouard DUPIN
 * @date 18/02/2012
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

#include <ewol/widget/Drawable.h>


ewol::Drawable::Drawable(void)
{
	// nothing to do ...
}

ewol::Drawable::~Drawable(void)
{
	//clean all the object
	for (int32_t jjj=0; jjj<NB_BOUBLE_BUFFER; jjj++) {
		for (int32_t iii=0; iii<m_listOObject[jjj].Size(); iii++) {
			delete(m_listOObject[jjj][iii]);
			m_listOObject[jjj][iii] = NULL;
		}
		m_listOObject[jjj].Clear();
	}
}


void ewol::Drawable::AddOObject(ewol::OObject* newObject, int32_t pos)
{
	if (NULL == newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	//EWOL_INFO("UPDATE AT size : (" << m_size.x << "," << m_size.y << ")");
	newObject->UpdateSize(m_size.x, m_size.y);
	//EWOL_INFO("UPDATE AT origin : (" << m_origin.x << "," << m_origin.y << ")");
	//newObject->UpdateOrigin(m_origin.x, m_origin.y);
	if (pos < 0 || pos >= m_listOObject[m_currentCreateId].Size() ) {
		m_listOObject[m_currentCreateId].PushBack(newObject);
	} else {
		m_listOObject[m_currentCreateId].Insert(pos, newObject);
	}
	m_needFlipFlop = true;
}


void ewol::Drawable::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject[m_currentCreateId].Size(); iii++) {
		delete(m_listOObject[m_currentCreateId][iii]);
		m_listOObject[m_currentCreateId][iii] = NULL;
	}
	m_listOObject[m_currentCreateId].Clear();
}

bool ewol::Drawable::OnDraw(void)
{
	for (int32_t iii=0; iii<m_listOObject[m_currentDrawId].Size(); iii++) {
		if (NULL != m_listOObject[m_currentDrawId][iii]) {
			m_listOObject[m_currentDrawId][iii]->Draw();
		}
	}
	return true;
}




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
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();

}



void ewol::Drawable::AddOObject(ewol::OObject* newObject, int32_t pos)
{
	if (NULL == newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (pos < 0 || pos >= m_listOObject.Size() ) {
		m_listOObject.PushBack(newObject);
	} else {
		m_listOObject.Insert(pos, newObject);
	}
}


void ewol::Drawable::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}

void ewol::Drawable::OnDraw(DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (NULL != m_listOObject[iii]) {
			m_listOObject[iii]->Draw();
		}
	}
}




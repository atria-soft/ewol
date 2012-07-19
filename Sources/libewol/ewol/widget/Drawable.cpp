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


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_DRAWABLE = "Drawable";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Drawable::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_DRAWABLE << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_DRAWABLE) {
		return true;
	} else {
		if(true == ewol::Widget::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_DRAWABLE << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Drawable::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_DRAWABLE;
}



void ewol::Drawable::AddOObject(ewol::OObject* newObject, int32_t pos)
{
	if (NULL == newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
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

void ewol::Drawable::OnDraw(DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_listOObject[m_currentDrawId].Size(); iii++) {
		if (NULL != m_listOObject[m_currentDrawId][iii]) {
			m_listOObject[m_currentDrawId][iii]->Draw();
		}
	}
}




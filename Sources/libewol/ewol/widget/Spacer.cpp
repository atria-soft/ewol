/**
 *******************************************************************************
 * @file ewol/widget/Spacer.cpp
 * @brief ewol Spacer widget system (Sources)
 * @author Edouard DUPIN
 * @date 29/12/2011
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

#include <ewol/widget/Spacer.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


#undef __class__
#define __class__	"Spacer"


ewol::Spacer::Spacer(void)
{
	m_size = 10;
	SetCanHaveFocus(false);
}

ewol::Spacer::~Spacer(void)
{
	
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_SPACER = "Spacer";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Spacer::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_SPACER << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_SPACER) {
		return true;
	} else {
		if(true == ewol::Drawable::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_SPACER << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Spacer::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_SPACER;
}


bool ewol::Spacer::CalculateMinSize(void)
{
	m_minSize.x = m_size;
	m_minSize.y = m_size;
	return true;
}


void ewol::Spacer::SetSize(float size)
{
	m_size = size;
	MarkToReedraw();
}


/**
 *******************************************************************************
 * @file ewol/widget/ProgressBar.cpp
 * @brief ewol progress bar widget system (Sources)
 * @author Edouard DUPIN
 * @date 14/04/2012
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

#include <ewol/widget/ProgressBar.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>

#undef __class__
#define __class__	"Slider"

const int32_t dotRadius = 6;

ewol::ProgressBar::ProgressBar(void)
{
	m_value = 0.0;
	
	m_textColorFg = etk::color::color_Black;
	
	m_textColorBgOn.red   = 0;
	m_textColorBgOn.green = 0xFF;
	m_textColorBgOn.blue  = 0;
	m_textColorBgOn.alpha = 0xFF;
	
	m_textColorBgOff = etk::color::color_Black;
	m_textColorBgOff.alpha = 0x3F;
	SetCanHaveFocus(true);
}

ewol::ProgressBar::~ProgressBar(void)
{
	
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_PROGRESS_BAR = "ProgressBar";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::ProgressBar::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_PROGRESS_BAR << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_PROGRESS_BAR) {
		return true;
	} else {
		if(true == ewol::Drawable::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_PROGRESS_BAR << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::ProgressBar::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_PROGRESS_BAR;
}




bool ewol::ProgressBar::CalculateMinSize(void)
{
	m_minSize.x = etk_max(m_userMinSize.x, 40);
	m_minSize.y = etk_max(m_userMinSize.y, dotRadius*2);
	MarkToReedraw();
	return true;
}


void ewol::ProgressBar::ValueSet(float val)
{
	m_value = etk_avg(0.0, val, 1.0);
	MarkToReedraw();
}


float ewol::ProgressBar::ValueGet(void)
{
	return m_value;
}


void ewol::ProgressBar::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		
		tmpOObjects->SetColor(m_textColorFg);
		
		int32_t tmpSizeX = m_size.x - 10;
		int32_t tmpSizeY = m_size.y - 10;
		int32_t tmpOriginX = 5;
		int32_t tmpOriginY = 5;
		tmpOObjects->SetColor(m_textColorBgOn);
		tmpOObjects->Rectangle( tmpOriginX, tmpOriginY, tmpSizeX*m_value, tmpSizeY);
		tmpOObjects->SetColor(m_textColorBgOff);
		tmpOObjects->Rectangle( tmpOriginX+tmpSizeX*m_value, tmpOriginY, tmpSizeX*(1.0-m_value), tmpSizeY);
		
		tmpOObjects->SetColor(m_textColorFg);
		tmpOObjects->RectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, 1);
		
		AddOObject(tmpOObjects);
	}
}




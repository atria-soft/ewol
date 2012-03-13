/**
 *******************************************************************************
 * @file ewol/widget/widgetE2D.cpp
 * @brief ewol file e2d widget system (Sources)
 * @author Edouard DUPIN
 * @date 13/03/2012
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

#include <ewol/widget/widgetE2D.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>
#include <ewol/OObject/e2d.h>


extern const char * const ewolEventE2DPressed    = "ewol widget e2d Pressed";

#undef __class__
#define __class__	"widgetE2D"


ewol::widgetE2D::widgetE2D(void)
{
	AddEventId(ewolEventE2DPressed);
	m_fileName = "";
}


ewol::widgetE2D::~widgetE2D(void)
{
	
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_E2D = "widgetE2D";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::widgetE2D::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_E2D << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_E2D) {
		return true;
	} else {
		if(true == ewol::Drawable::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_E2D << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::widgetE2D::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_E2D;
}


void ewol::widgetE2D::SetElement(etk::File fileName)
{
	m_fileName = fileName;
	MarkToReedraw();
}

void ewol::widgetE2D::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		if (m_fileName != "") {
			ewol::oobject::e2d * tmpE2D = new ewol::oobject::e2d(m_fileName);
			tmpE2D->SetSize(m_size);
			AddOObject(tmpE2D);
		}
	}
}

/**
 * @brief Event on an input of this Widget
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::widgetE2D::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos)
{
	//EWOL_DEBUG("Event on e2d object ...");
	if (1 == IdInput) {
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			// nothing to do ...
			//EWOL_DEBUG("    ==> generate event : " << ewolEventE2DPressed);
			GenerateEventId(ewolEventE2DPressed);
			return true;
		}
	}
	return false;
}


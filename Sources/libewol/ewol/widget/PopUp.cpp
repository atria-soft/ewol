/**
 *******************************************************************************
 * @file ewol/widget/PopUp.cpp
 * @brief ewol pop-up widget system (Sources)
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



#include <ewol/widget/PopUp.h>
#include <ewol/WidgetManager.h>

#undef __class__
#define __class__	"PopUp"

ewol::PopUp::PopUp(void)
{
	m_userExpendX = true;
	m_userExpendY = true;

	m_colorBackGroung.red   = 1.0;
	m_colorBackGroung.green = 1.0;
	m_colorBackGroung.blue  = 1.0;
	m_colorBackGroung.alpha = 1.0;

	m_colorEmptyArea.red   = 0.0;
	m_colorEmptyArea.green = 0.0;
	m_colorEmptyArea.blue  = 0.0;
	m_colorEmptyArea.alpha = 0.50;
	
	for (int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_subWidget[iii] = 0;
	}
}

ewol::PopUp::~PopUp(void)
{
	SubWidgetRemove();
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_POP_UP = "PopUp";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::PopUp::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_POP_UP << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_POP_UP) {
		return true;
	} else {
		if(true == ewol::Drawable::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_POP_UP << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::PopUp::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_POP_UP;
}



bool ewol::PopUp::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	//EWOL_DEBUG("CalculateSize(" << availlableX << "," << availlableY << ")");
	// pop-up fill all the display :
	m_size.x = availlableX;
	m_size.y = availlableY;
	
	if (NULL != m_subWidget[m_currentCreateId]) {
		coord2D_ts subWidgetSize;
		coord2D_ts subWidgetOrigin;
		subWidgetSize = m_subWidget[m_currentCreateId]->GetMinSize();
		if (true == m_subWidget[m_currentCreateId]->CanExpentX()) {
			subWidgetSize.x = m_size.x;
		}
		if (true == m_subWidget[m_currentCreateId]->CanExpentY()) {
			subWidgetSize.y = m_size.y;
		}
		if (m_displayRatio>0.1 && m_displayRatio<=1) {
			subWidgetSize.x = etk_max(m_size.x*m_displayRatio, subWidgetSize.x);
			subWidgetSize.y = etk_max(m_size.y*m_displayRatio, subWidgetSize.y);
		}
		// force to be an integer ...
		subWidgetSize.x = (int32_t)subWidgetSize.x;
		subWidgetSize.y = (int32_t)subWidgetSize.y;
		// set config to the Sub-widget
		subWidgetOrigin.x = (int32_t)(m_size.x - m_origin.x - subWidgetSize.x)/2 + m_origin.x;
		subWidgetOrigin.y = (int32_t)(m_size.y - m_origin.y - subWidgetSize.y)/2 + m_origin.y;
		
		m_subWidget[m_currentCreateId]->SetOrigin(subWidgetOrigin.x, subWidgetOrigin.y);
		m_subWidget[m_currentCreateId]->CalculateSize(subWidgetSize.x, subWidgetSize.y);
	}
	MarkToReedraw();
	return true;
}


bool ewol::PopUp::CalculateMinSize(void)
{
	//EWOL_DEBUG("CalculateMinSize");
	m_userExpendX=false;
	m_userExpendY=false;
	m_minSize.x = 50.0;
	m_minSize.y = 50.0;
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->CalculateMinSize();
		coord2D_ts tmpSize = m_subWidget[m_currentCreateId]->GetMinSize();
		m_minSize.x = tmpSize.x;
		m_minSize.y = tmpSize.y;
	}
	//EWOL_DEBUG("CalculateMinSize(" << m_minSize.x << "," << m_minSize.y << ")");
	MarkToReedraw();
	return true;
}

void ewol::PopUp::SetMinSise(etkFloat_t x, etkFloat_t y)
{
	EWOL_ERROR("Pop-up can not have a user Minimum size (herited from under elements)");
}

void ewol::PopUp::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings X (herited from under elements)");
}

void ewol::PopUp::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Pop-up can not have a user expend settings Y (herited from under elements)");
}


void ewol::PopUp::SubWidgetSet(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	SubWidgetRemove();
	m_subWidget[m_currentCreateId] = newWidget;
	m_needFlipFlop = true;
	MarkToReedraw();
}


void ewol::PopUp::SubWidgetRemove(void)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->MarkToRemove();;
		m_subWidget[m_currentCreateId] = NULL;
	}
	m_needFlipFlop = true;
	MarkToReedraw();
}

void ewol::PopUp::OnDraw(void)
{
	// draw upper classes
	ewol::Drawable::OnDraw();
	if (NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->GenDraw();
	}
}


void ewol::PopUp::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
	}
	// generate a white background and take gray on other surfaces
	ClearOObjectList();
	ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
	AddOObject(BGOObjects);
	
	BGOObjects->SetColor(m_colorEmptyArea);
	BGOObjects->Rectangle(0, 0, m_size.x, m_size.y);
	// set the area in white ...
	if (NULL != m_subWidget[m_currentCreateId]) {
		coord2D_ts tmpSize = m_subWidget[m_currentCreateId]->GetSize();
		coord2D_ts tmpOrigin = m_subWidget[m_currentCreateId]->GetOrigin();
		BGOObjects->SetColor(m_colorBackGroung);
		BGOObjects->Rectangle(tmpOrigin.x, tmpOrigin.y, tmpSize.x, tmpSize.y);
	}
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->OnRegenerateDisplay();
	}
}


/**
 * @brief Get the widget at the specific windows absolute position
 * @param[in] pos gAbsolute position of the requested widget knowledge
 * @return NULL No widget found
 * @return pointer on the widget found
 */
ewol::Widget * ewol::PopUp::GetWidgetAtPos(coord2D_ts pos)
{
	// calculate relative position
	coord2D_ts relativePos = RelativePosition(pos);
	// for the element in the pop-up ...
	if (NULL != m_subWidget[m_currentCreateId]) {
		coord2D_ts tmpSize = m_subWidget[m_currentCreateId]->GetSize();
		coord2D_ts tmpOrigin = m_subWidget[m_currentCreateId]->GetOrigin();
		if(    (tmpOrigin.x <= relativePos.x && tmpOrigin.x + tmpSize.x >= relativePos.x)
		    && (tmpOrigin.y <= relativePos.y && tmpOrigin.y + tmpSize.y >= relativePos.y) )
		{
			return m_subWidget[m_currentCreateId]->GetWidgetAtPos(pos);
		} else {
			//EWOL_INFO("Event ouside the Pop-up");
		}
	}
	// otherwise the event go to this widget ...
	return this;
}


void ewol::PopUp::SetDisplayRatio(etkFloat_t ratio)
{
	m_displayRatio = ratio;
}


/**
 * @brief Event generated to inform a flip-flop has occured on the current widget
 * @param ---
 * @return ---
 */
void ewol::PopUp::OnFlipFlopEvent(void)
{
	bool needFlipFlop = m_needFlipFlop;
	// call herited classes
	ewol::Widget::OnFlipFlopEvent();
	// internal saving
	if (true == needFlipFlop) {
		m_subWidget[m_currentCreateId] = m_subWidget[m_currentDrawId];
	}
	// in every case, we propagate the flip-flop EVENT
	if(NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->OnFlipFlopEvent();
	}
}

/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void ewol::PopUp::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::Drawable::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	if(m_subWidget[m_currentCreateId] == removeObject) {
		EWOL_DEBUG("Remove pop-up sub Element ==> destroyed object");
		m_subWidget[m_currentCreateId] = NULL;
		m_needFlipFlop = true;
	}
}


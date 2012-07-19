/**
 *******************************************************************************
 * @file ewol/widget/Layer.cpp
 * @brief ewol layer widget system (Sources)
 * @author Edouard DUPIN
 * @date 11/04/2012
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


#include <ewol/widget/Layer.h>
#include <ewol/WidgetManager.h>


#undef __class__
#define __class__	"Layer"


ewol::Layer::Layer(void)
{
	// set contamination enable
	LockExpendContamination();
}

ewol::Layer::~Layer(void)
{
	SubWidgetRemoveAll();
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_LAYER = __class__;

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Layer::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_LAYER << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_LAYER) {
		return true;
	} else {
		if(true == ewol::Widget::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_LAYER << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Layer::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_LAYER;
}



bool ewol::Layer::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("Update Size");
	m_size.x = availlableX;
	m_size.y = availlableY;
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii]->SetOrigin(m_origin.x, m_origin.y);
			m_subWidget[m_currentCreateId][iii]->CalculateSize(m_size.x, m_size.y);
		}
	}
	MarkToReedraw();
	return true;
}


bool ewol::Layer::CalculateMinSize(void)
{
	m_userExpendX=false;
	m_userExpendY=false;
	m_minSize.x = 0.0;
	m_minSize.y = 0.0;
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii]->CalculateMinSize();
			if (true == m_subWidget[m_currentCreateId][iii]->CanExpentX()) {
				m_userExpendX = true;
			}
			if (true == m_subWidget[m_currentCreateId][iii]->CanExpentY()) {
				m_userExpendY = true;
			}
			Vector2D<float> tmpSize = m_subWidget[m_currentCreateId][iii]->GetMinSize();
			m_minSize.x = etk_max(tmpSize.x, m_minSize.x);
			m_minSize.y = etk_max(tmpSize.y, m_minSize.y);
		}
	}
	return true;
}

void ewol::Layer::SetMinSise(float x, float y)
{
	EWOL_ERROR("Layer can not have a user Minimum size (herited from under elements)");
}

void ewol::Layer::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Layer can not have a user expend settings X (herited from under elements)");
}

bool ewol::Layer::CanExpentX(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpendX;
}

void ewol::Layer::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings Y (herited from under elements)");
}

bool ewol::Layer::CanExpentY(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpendY;
}

void ewol::Layer::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination = lockExpend;
}

//etk::VectorType<ewol::Widget*> m_SubWidget;

void ewol::Layer::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		m_subWidget[m_currentCreateId][iii]->MarkToRemove();
		m_subWidget[m_currentCreateId][iii] = NULL;
	}
	m_subWidget[m_currentCreateId].Clear();
}


void ewol::Layer::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget[m_currentCreateId].PushBack(newWidget);
}


void ewol::Layer::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (newWidget == m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii]->MarkToRemove();
			m_subWidget[m_currentCreateId][iii] = NULL;
			m_subWidget[m_currentCreateId].Erase(iii);
			return;
		}
	}
}

void ewol::Layer::SubWidgetUnLink(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (newWidget == m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii] = NULL;
			m_subWidget[m_currentCreateId].Erase(iii);
			return;
		}
	}
}


void ewol::Layer::OnDraw(DrawProperty& displayProp)
{
	// draw is done in the invert sense of inserting ... the first element inserted is on the top and the last is on the buttom
	for (int32_t iii=m_subWidget[m_currentDrawId].Size()-1; iii>=0; iii--) {
		if (NULL != m_subWidget[m_currentDrawId][iii]) {
			m_subWidget[m_currentDrawId][iii]->GenDraw(displayProp);
		}
	}
}



void ewol::Layer::OnRegenerateDisplay(void)
{
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii]->OnRegenerateDisplay();
		}
	}
}


/**
 * @brief Get the widget at the specific windows absolute position
 * @param[in] pos gAbsolute position of the requested widget knowledge
 * @return NULL No widget found
 * @return pointer on the widget found
 */
ewol::Widget * ewol::Layer::GetWidgetAtPos(Vector2D<float> pos)
{
	// for all element in the sizer ...
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			Vector2D<float> tmpSize = m_subWidget[m_currentCreateId][iii]->GetSize();
			Vector2D<float> tmpOrigin = m_subWidget[m_currentCreateId][iii]->GetOrigin();
			if(    (tmpOrigin.x <= pos.x && tmpOrigin.x + tmpSize.x >= pos.x)
			    && (tmpOrigin.y <= pos.y && tmpOrigin.y + tmpSize.y >= pos.y) )
			{
				ewol::Widget * tmpWidget = m_subWidget[m_currentCreateId][iii]->GetWidgetAtPos(pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
			}
		}
	}
	// otherwise the event go to this widget ...
	return this;
}


/**
 * @brief Event generated to inform a flip-flop has occured on the current widget
 * @param ---
 * @return ---
 */
void ewol::Layer::OnFlipFlopEvent(void)
{
	bool needFlipFlop = m_needFlipFlop;
	// call herited classes
	ewol::Widget::OnFlipFlopEvent();
	// internal saving
	if (true == needFlipFlop) {
		m_subWidget[m_currentCreateId] = m_subWidget[m_currentDrawId];
	}
	// in every case, we propagate the flip-flop EVENT
	for(int32_t iii=0; iii<m_subWidget[m_currentDrawId].Size(); iii++) {
		if(NULL != m_subWidget[m_currentDrawId][iii]) {
			m_subWidget[m_currentDrawId][iii]->OnFlipFlopEvent();
		}
	}
}


/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void ewol::Layer::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::Widget::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	for(int32_t iii=m_subWidget[m_currentCreateId].Size()-1; iii>=0; iii--) {
		if(m_subWidget[m_currentCreateId][iii] == removeObject) {
			EWOL_DEBUG("Remove sizer sub Element [" << iii << "] ==> destroyed object");
			m_subWidget[m_currentCreateId][iii] = NULL;
			m_subWidget[m_currentCreateId].Erase(iii);
			m_needFlipFlop = true;
		}
	}
}



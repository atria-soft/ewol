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


bool ewol::Layer::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("Update Size");
	m_size.x = availlableX;
	m_size.y = availlableY;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->SetOrigin(m_origin.x, m_origin.y);
			m_subWidget[iii]->CalculateSize(m_size.x, m_size.y);
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
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinSize();
			if (true == m_subWidget[iii]->CanExpentX()) {
				m_userExpendX = true;
			}
			if (true == m_subWidget[iii]->CanExpentY()) {
				m_userExpendY = true;
			}
			Vector2D<float> tmpSize = m_subWidget[iii]->GetMinSize();
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
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		m_subWidget[iii]->MarkToRemove();
		m_subWidget[iii] = NULL;
	}
	m_subWidget.Clear();
}


void ewol::Layer::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
}


void ewol::Layer::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			m_subWidget[iii]->MarkToRemove();
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			return;
		}
	}
}

void ewol::Layer::SubWidgetUnLink(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			return;
		}
	}
}


void ewol::Layer::OnDraw(DrawProperty& displayProp)
{
	// draw is done in the invert sense of inserting ... the first element inserted is on the top and the last is on the buttom
	for (int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	}
}



void ewol::Layer::OnRegenerateDisplay(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
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
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			Vector2D<float> tmpSize = m_subWidget[iii]->GetSize();
			Vector2D<float> tmpOrigin = m_subWidget[iii]->GetOrigin();
			if(    (tmpOrigin.x <= pos.x && tmpOrigin.x + tmpSize.x >= pos.x)
			    && (tmpOrigin.y <= pos.y && tmpOrigin.y + tmpSize.y >= pos.y) )
			{
				ewol::Widget * tmpWidget = m_subWidget[iii]->GetWidgetAtPos(pos);
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
	for(int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if(m_subWidget[iii] == removeObject) {
			EWOL_DEBUG("Remove sizer sub Element [" << iii << "] ==> destroyed object");
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
		}
	}
}



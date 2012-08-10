/**
 *******************************************************************************
 * @file ewol/widget/SizerHori.cpp
 * @brief ewol horisantal sizer widget system (Sources)
 * @author Edouard DUPIN
 * @date 07/11/2011
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



#include <ewol/widget/SizerHori.h>
#include <ewol/WidgetManager.h>

#undef __class__
#define __class__	"SizerHori"

ewol::SizerHori::SizerHori(void)
{
	// set contamination enable
	LockExpendContamination();
}

ewol::SizerHori::~SizerHori(void)
{
	SubWidgetRemoveAll();
}


bool ewol::SizerHori::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("Update Size");
	m_size.x = availlableX;
	m_size.y = availlableY;
	// calculate unExpendable Size :
	float unexpendableSize=0.0;
	int32_t nbWidgetFixedSize=0;
	int32_t nbWidgetNotFixedSize=0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			Vector2D<float> tmpSize = m_subWidget[iii]->GetMinSize();
			unexpendableSize += tmpSize.x;
			if (false == m_subWidget[iii]->CanExpentX()) {
				nbWidgetFixedSize++;
			} else {
				nbWidgetNotFixedSize++;
			}
		}
	}
	float sizeToAddAtEveryOne = 0;
	// 2 cases : 1 or more can Expend, or all is done ...
	if (0 != nbWidgetNotFixedSize) {
		sizeToAddAtEveryOne = (m_size.x - unexpendableSize) / nbWidgetNotFixedSize;
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	Vector2D<float> tmpOrigin;
	tmpOrigin.x = m_origin.x;
	tmpOrigin.y = m_origin.y;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			Vector2D<float> tmpSize = m_subWidget[iii]->GetMinSize();
			// Set the origin :
			//EWOL_DEBUG("Set ORIGIN : " << tmpOrigin.x << "," << tmpOrigin.y << ")");
			m_subWidget[iii]->SetOrigin(tmpOrigin.x, tmpOrigin.y);
			// Now Update his Size  his size in X and the curent sizer size in Y:
			if (true == m_subWidget[iii]->CanExpentX()) {
				m_subWidget[iii]->CalculateSize(tmpSize.x+sizeToAddAtEveryOne, m_size.y);
				tmpOrigin.x += tmpSize.x+sizeToAddAtEveryOne;
			} else {
				m_subWidget[iii]->CalculateSize(tmpSize.x, m_size.y);
				tmpOrigin.x += tmpSize.x;
			}
		}
	}
	MarkToReedraw();
	return true;
}


bool ewol::SizerHori::CalculateMinSize(void)
{
	//EWOL_DEBUG("Update minimum Size");
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
			m_minSize.x += tmpSize.x;
			if (tmpSize.y>m_minSize.y) {
				m_minSize.y = tmpSize.y;
			}
		}
	}
	//EWOL_DEBUG("Result : expend  X="<< m_userExpendX << " Y=" << m_userExpendY);
	//EWOL_DEBUG("         minSize ("<< m_minSize.x << "," << m_minSize.y << ")");
	return true;
}

void ewol::SizerHori::SetMinSize(float x, float y)
{
	EWOL_ERROR("Sizer can not have a user Minimum size (herited from under elements)");
}

void ewol::SizerHori::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings X (herited from under elements)");
}

bool ewol::SizerHori::CanExpentX(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpendX;
}

void ewol::SizerHori::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings Y (herited from under elements)");
}

bool ewol::SizerHori::CanExpentY(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpendY;
}

void ewol::SizerHori::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination = lockExpend;
}

//etk::VectorType<ewol::Widget*> m_subWidget[m_currentCreateId];

void ewol::SizerHori::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->MarkToRemove();
			m_subWidget[iii] = NULL;
		}
	}
	m_subWidget.Clear();
}


void ewol::SizerHori::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
}


void ewol::SizerHori::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->MarkToRemove();
				m_subWidget[iii] = NULL;
			}
			m_subWidget.Erase(iii);
			return;
		}
	}
}

void ewol::SizerHori::SubWidgetUnLink(ewol::Widget* newWidget)
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


void ewol::SizerHori::OnDraw(DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	}
}


void ewol::SizerHori::OnRegenerateDisplay(void)
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
ewol::Widget * ewol::SizerHori::GetWidgetAtPos(Vector2D<float> pos)
{
	if (true == IsHide()) {
		return NULL;
	}
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
				// stop searching
				break;
			}
		}
	}
	// TODO : Check if we have a mover, otherwire return NULL;
	return NULL;
	//return this;
}


/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void ewol::SizerHori::OnObjectRemove(ewol::EObject * removeObject)
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






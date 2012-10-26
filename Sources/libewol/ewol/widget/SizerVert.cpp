/**
 *******************************************************************************
 * @file ewol/widget/SizerVert.cpp
 * @brief ewol vertical sizer widget system (Sources)
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


#include <ewol/widget/SizerVert.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__	"SizerVert"


ewol::SizerVert::SizerVert(void)
{
	// set contamination enable
	LockExpendContamination();
}

ewol::SizerVert::~SizerVert(void)
{
	SubWidgetRemoveAll();
}


bool ewol::SizerVert::CalculateSize(float availlableX, float availlableY)
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
			etk::Vector2D<float> tmpSize = m_subWidget[iii]->GetMinSize();
			unexpendableSize += tmpSize.y;
			if (false == m_subWidget[iii]->CanExpentY()) {
				nbWidgetFixedSize++;
			} else {
				nbWidgetNotFixedSize++;
			}
		}
	}
	// 2 cases : 1 or more can Expend, or all is done ...
	float sizeToAddAtEveryOne = 0;
	// 2 cases : 1 or more can Expend, or all is done ...
	if (0 != nbWidgetNotFixedSize) {
		sizeToAddAtEveryOne = (m_size.y - unexpendableSize) / nbWidgetNotFixedSize;
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	etk::Vector2D<float> tmpOrigin;
	tmpOrigin.x = m_origin.x;
	tmpOrigin.y = m_origin.y;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			etk::Vector2D<float> tmpSize = m_subWidget[iii]->GetMinSize();
			// Set the origin :
			//EWOL_DEBUG("Set ORIGIN : " << tmpOrigin.x << "," << tmpOrigin.y << ")");
			m_subWidget[iii]->SetOrigin(tmpOrigin.x, tmpOrigin.y);
			// Now Update his Size  his size in X and the curent sizer size in Y:
			if (true == m_subWidget[iii]->CanExpentY()) {
				m_subWidget[iii]->CalculateSize(m_size.x, tmpSize.y+sizeToAddAtEveryOne);
				tmpOrigin.y += tmpSize.y+sizeToAddAtEveryOne;
			} else {
				m_subWidget[iii]->CalculateSize(m_size.x, tmpSize.y);
				tmpOrigin.y += tmpSize.y;
			}
		}
	}
	MarkToRedraw();
	return true;
}


bool ewol::SizerVert::CalculateMinSize(void)
{
	//EWOL_DEBUG("Update minimum Size");
	m_userExpend.x=false;
	m_userExpend.y=false;
	m_minSize.x = 0.0;
	m_minSize.y = 0.0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinSize();
			if (true == m_subWidget[iii]->CanExpentX()) {
				m_userExpend.x = true;
			}
			if (true == m_subWidget[iii]->CanExpentY()) {
				m_userExpend.y = true;
			}
			etk::Vector2D<float> tmpSize = m_subWidget[iii]->GetMinSize();
			//EWOL_DEBUG("             Get minSize[" << iii << "] ("<< tmpSize.x << "," << tmpSize.y << ")");
			m_minSize.y += tmpSize.y;
			if (tmpSize.x>m_minSize.x) {
				m_minSize.x = tmpSize.x;
			}
		}
	}
	//EWOL_DEBUG("Result : expend  X="<< m_userExpend.x << " Y=" << m_userExpend.y);
	//EWOL_DEBUG("         minSize ("<< m_minSize.x << "," << m_minSize.y << ")");
	return true;
}

void ewol::SizerVert::SetMinSize(float x, float y)
{
	EWOL_ERROR("Sizer can not have a user Minimum size (herited from under elements)");
}

void ewol::SizerVert::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings X (herited from under elements)");
}

bool ewol::SizerVert::CanExpentX(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpend.x;
}

void ewol::SizerVert::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings Y (herited from under elements)");
}

bool ewol::SizerVert::CanExpentY(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpend.y;
}

void ewol::SizerVert::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination = lockExpend;
}

//etk::Vector<ewol::Widget*> m_SubWidget;

void ewol::SizerVert::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		delete(m_subWidget[iii]);
		m_subWidget[iii] = NULL;
	}
	m_subWidget.Clear();
}


void ewol::SizerVert::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
}


void ewol::SizerVert::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			delete(m_subWidget[iii]);
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			return;
		}
	}
}

void ewol::SizerVert::SubWidgetUnLink(ewol::Widget* newWidget)
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


void ewol::SizerVert::OnDraw(DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	}
}



void ewol::SizerVert::OnRegenerateDisplay(void)
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
ewol::Widget * ewol::SizerVert::GetWidgetAtPos(etk::Vector2D<float> pos)
{
	if (true == IsHide()) {
		return NULL;
	}
	// for all element in the sizer ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			etk::Vector2D<float> tmpSize = m_subWidget[iii]->GetSize();
			etk::Vector2D<float> tmpOrigin = m_subWidget[iii]->GetOrigin();
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
void ewol::SizerVert::OnObjectRemove(ewol::EObject * removeObject)
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



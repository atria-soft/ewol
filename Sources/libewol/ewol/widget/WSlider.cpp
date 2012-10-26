/**
 *******************************************************************************
 * @file ewol/widget/WSlider.cpp
 * @brief ewol widget slider widget system (Sources)
 * @author Edouard DUPIN
 * @date 05/06/2012
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


#include <ewol/widget/WSlider.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>


#undef __class__
#define __class__	"WSlider"


ewol::WSlider::WSlider(void)
{
	// set contamination enable
	LockExpendContamination();
	m_windowsDestination = 0;
	m_slidingProgress = 0;
	m_windowsSources = 0;
	m_underExpend.x = false;
	m_underExpend.y = false;
}

ewol::WSlider::~WSlider(void)
{
	SubWidgetRemoveAll();
}


bool ewol::WSlider::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("Update Size");
	m_size.x = availlableX;
	m_size.y = availlableY;
	
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SetOrigin(m_origin.x, m_origin.y);
				m_subWidget[iii]->CalculateSize(m_size.x, m_size.y);
			}
		}
	} else {
		int32_t iii = m_windowsSources;
		if (iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SetOrigin(m_origin.x - (m_size.x*(float)m_slidingProgress/1000.0),  m_origin.y);
				m_subWidget[iii]->CalculateSize(m_size.x, m_size.y);
			}
		}
		iii = m_windowsDestination;
		if (iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SetOrigin(m_origin.x - (m_size.x*((float)m_slidingProgress/1000.0) - m_size.x),  m_origin.y);
				m_subWidget[iii]->CalculateSize(m_size.x, m_size.y);
			}
		}
	}
	MarkToRedraw();
	return true;
}


bool ewol::WSlider::CalculateMinSize(void)
{
	EWOL_DEBUG("Calculate MinSize");
	m_underExpend.x=false;
	m_underExpend.y=false;
	m_minSize.x = 0.0;
	m_minSize.y = 0.0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinSize();
			if (true == m_subWidget[iii]->CanExpentX()) {
				m_underExpend.x = true;
			}
			if (true == m_subWidget[iii]->CanExpentY()) {
				m_underExpend.y = true;
			}
			etk::Vector2D<float> tmpSize = m_subWidget[iii]->GetMinSize();
			m_minSize.x = etk_max(tmpSize.x, m_minSize.x);
			m_minSize.y = etk_max(tmpSize.y, m_minSize.y);
		}
	}
	return true;
}

void ewol::WSlider::SetMinSise(float x, float y)
{
	EWOL_ERROR("Layer can not have a user Minimum size (herited from under elements)");
}

bool ewol::WSlider::CanExpentX(void)
{
	if (m_userExpend.x == true) {
		return true;
	}
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_underExpend.x;
}

bool ewol::WSlider::CanExpentY(void)
{
	if (m_userExpend.y == true) {
		return true;
	}
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_underExpend.y;
}

void ewol::WSlider::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination = lockExpend;
}

//etk::Vector<ewol::Widget*> m_SubWidget;

void ewol::WSlider::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		delete(m_subWidget[iii]);
		m_subWidget[iii] = NULL;
	}
	m_subWidget.Clear();
}


void ewol::WSlider::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


void ewol::WSlider::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			delete(m_subWidget[iii]);
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			MarkToRedraw();
			ewol::RequestUpdateSize();
			return;
		}
	}
}

void ewol::WSlider::SubWidgetUnLink(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			ewol::RequestUpdateSize();
			MarkToRedraw();
			return;
		}
	}
}

void ewol::WSlider::SubWidgetSelectSet(int32_t id)
{
	if (id<0 || id > m_subWidget.Size()) {
		EWOL_ERROR("Can not change to a widget not present");
	}
	m_windowsDestination = id;
	m_slidingProgress = 0;
	PeriodicCallSet(true);
	MarkToRedraw();
}

/**
 * @brief Periodic call of this widget
 * @param localTime curent system time
 * @return ---
 */
void ewol::WSlider::PeriodicCall(int64_t localTime)
{
	if (m_slidingProgress >= 1000) {
		// end of periodic :
		PeriodicCallSet(false);
		m_windowsSources = m_windowsDestination;
	} else {
		m_slidingProgress += 30;
		m_slidingProgress = etk_avg(0, m_slidingProgress, 1000);
	}
	CalculateSize(m_size.x, m_size.y);
	MarkToRedraw();
}


void ewol::WSlider::OnDraw(DrawProperty& displayProp)
{
	if (m_windowsDestination == m_windowsSources) {
		//EWOL_DEBUG("Draw : " << m_windowsDestination);
		int32_t iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	} else {
		//EWOL_DEBUG("Draw : " << m_windowsSources << "=>" << m_windowsDestination << "progress=" << ((float)m_slidingProgress/1000.) );
		// draw Sources :
		int32_t iii = m_windowsSources;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
		// Draw Destination : 
		iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	}
}



void ewol::WSlider::OnRegenerateDisplay(void)
{
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
	} else {
		int32_t iii = m_windowsSources;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
		iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
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
ewol::Widget * ewol::WSlider::GetWidgetAtPos(etk::Vector2D<float> pos)
{
	// TODO : Review this ...
	if (m_windowsDestination<0 || m_windowsDestination > m_subWidget.Size()) {
		// error ...
		return NULL;
	}
	int32_t iii = m_windowsDestination;
	
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
void ewol::WSlider::OnObjectRemove(ewol::EObject * removeObject)
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



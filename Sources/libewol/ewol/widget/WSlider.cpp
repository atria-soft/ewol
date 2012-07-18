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
#include <ewol/WidgetManager.h>


#undef __class__
#define __class__	"WSlider"


ewol::WSlider::WSlider(void)
{
	// set contamination enable
	LockExpendContamination();
	m_windowsDestination = 0;
	m_slidingProgress = 0;
	m_windowsSources = 0;
}

ewol::WSlider::~WSlider(void)
{
	SubWidgetRemoveAll();
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_W_SLIDER = __class__;

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::WSlider::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_W_SLIDER << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_W_SLIDER) {
		return true;
	} else {
		if(true == ewol::Widget::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_W_SLIDER << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::WSlider::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_W_SLIDER;
}



bool ewol::WSlider::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("Update Size");
	m_size.x = availlableX;
	m_size.y = availlableY;
	
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii < m_subWidget[m_currentCreateId].Size()) {
			if (NULL != m_subWidget[m_currentCreateId][iii]) {
				m_subWidget[m_currentCreateId][iii]->SetOrigin(m_origin.x, m_origin.y);
				m_subWidget[m_currentCreateId][iii]->CalculateSize(m_size.x, m_size.y);
			}
		}
	} else {
		int32_t iii = m_windowsSources;
		if (iii < m_subWidget[m_currentCreateId].Size()) {
			if (NULL != m_subWidget[m_currentCreateId][iii]) {
				m_subWidget[m_currentCreateId][iii]->SetOrigin(m_origin.x - (m_size.x*(float)m_slidingProgress/1000.0),  m_origin.y);
				m_subWidget[m_currentCreateId][iii]->CalculateSize(m_size.x, m_size.y);
			}
		}
		iii = m_windowsDestination;
		if (iii < m_subWidget[m_currentCreateId].Size()) {
			if (NULL != m_subWidget[m_currentCreateId][iii]) {
				m_subWidget[m_currentCreateId][iii]->SetOrigin(m_origin.x - (m_size.x*((float)m_slidingProgress/1000.0) - m_size.x),  m_origin.y);
				m_subWidget[m_currentCreateId][iii]->CalculateSize(m_size.x, m_size.y);
			}
		}
	}
	MarkToReedraw();
	return true;
}


bool ewol::WSlider::CalculateMinSize(void)
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

void ewol::WSlider::SetMinSise(float x, float y)
{
	EWOL_ERROR("Layer can not have a user Minimum size (herited from under elements)");
}

void ewol::WSlider::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Layer can not have a user expend settings X (herited from under elements)");
}

bool ewol::WSlider::CanExpentX(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpendX;
}

void ewol::WSlider::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings Y (herited from under elements)");
}

bool ewol::WSlider::CanExpentY(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpendY;
}

void ewol::WSlider::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination = lockExpend;
}

//etk::VectorType<ewol::Widget*> m_SubWidget;

void ewol::WSlider::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_subWidget[m_currentCreateId].Size(); iii++) {
		m_subWidget[m_currentCreateId][iii]->MarkToRemove();
		m_subWidget[m_currentCreateId][iii] = NULL;
	}
	m_subWidget[m_currentCreateId].Clear();
}


void ewol::WSlider::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget[m_currentCreateId].PushBack(newWidget);
}


void ewol::WSlider::SubWidgetRemove(ewol::Widget* newWidget)
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

void ewol::WSlider::SubWidgetUnLink(ewol::Widget* newWidget)
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

void ewol::WSlider::SubWidgetSelectSet(int32_t id)
{
	if (id<0 || id > m_subWidget[m_currentCreateId].Size()) {
		EWOL_ERROR("Can not change to a widget not present");
	}
	m_windowsDestination = id;
	m_slidingProgress = 0;
	PeriodicCallSet(true);
	MarkToReedraw();
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
	MarkToReedraw();
}


void ewol::WSlider::OnDraw(void)
{
	if (m_windowsDestination == m_windowsSources) {
		//EWOL_DEBUG("Draw : " << m_windowsDestination);
		int32_t iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget[m_currentDrawId].Size()) {
			return;
		}
		if (NULL != m_subWidget[m_currentDrawId][iii]) {
			m_subWidget[m_currentDrawId][iii]->GenDraw();
		}
	} else {
		//EWOL_DEBUG("Draw : " << m_windowsSources << "=>" << m_windowsDestination << "progress=" << ((float)m_slidingProgress/1000.) );
		// draw Sources :
		int32_t iii = m_windowsSources;
		if (iii<0 || iii > m_subWidget[m_currentDrawId].Size()) {
			return;
		}
		if (NULL != m_subWidget[m_currentDrawId][iii]) {
			m_subWidget[m_currentDrawId][iii]->GenDraw();
		}
		// Draw Destination : 
		iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget[m_currentDrawId].Size()) {
			return;
		}
		if (NULL != m_subWidget[m_currentDrawId][iii]) {
			m_subWidget[m_currentDrawId][iii]->GenDraw();
		}
	}
}



void ewol::WSlider::OnRegenerateDisplay(void)
{
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget[m_currentCreateId].Size()) {
			return;
		}
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii]->OnRegenerateDisplay();
		}
	} else {
		int32_t iii = m_windowsSources;
		if (iii<0 || iii > m_subWidget[m_currentCreateId].Size()) {
			return;
		}
		if (NULL != m_subWidget[m_currentCreateId][iii]) {
			m_subWidget[m_currentCreateId][iii]->OnRegenerateDisplay();
		}
		iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget[m_currentCreateId].Size()) {
			return;
		}
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
ewol::Widget * ewol::WSlider::GetWidgetAtPos(Vector2D<float> pos)
{
	// TODO : Review this ...
	if (m_windowsDestination<0 || m_windowsDestination > m_subWidget[m_currentCreateId].Size()) {
		// error ...
		return NULL;
	}
	int32_t iii = m_windowsDestination;
	
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
	// otherwise the event go to this widget ...
	return this;
}


/**
 * @brief Event generated to inform a flip-flop has occured on the current widget
 * @param ---
 * @return ---
 */
void ewol::WSlider::OnFlipFlopEvent(void)
{
	bool needFlipFlop = m_needFlipFlop;
	// call herited classes
	ewol::Widget::OnFlipFlopEvent();
	// internal saving
	if (true == needFlipFlop) {
		m_subWidget[m_currentCreateId] = m_subWidget[m_currentDrawId];
	}
	if (m_windowsDestination<0 || m_windowsDestination > m_subWidget[m_currentDrawId].Size()) {
		// error ...
		return;
	}
	if(NULL != m_subWidget[m_currentDrawId][m_windowsDestination]) {
		m_subWidget[m_currentDrawId][m_windowsDestination]->OnFlipFlopEvent();
	}
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
	for(int32_t iii=m_subWidget[m_currentCreateId].Size()-1; iii>=0; iii--) {
		if(m_subWidget[m_currentCreateId][iii] == removeObject) {
			EWOL_DEBUG("Remove sizer sub Element [" << iii << "] ==> destroyed object");
			m_subWidget[m_currentCreateId][iii] = NULL;
			m_subWidget[m_currentCreateId].Erase(iii);
			m_needFlipFlop = true;
		}
	}
}



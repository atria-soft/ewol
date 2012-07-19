/**
 *******************************************************************************
 * @file ewol/Windows.cpp
 * @brief ewol window system (sources)
 * @author Edouard DUPIN
 * @date 20/10/2011
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

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/Widget.h>
#include <ewol/Windows.h>
#include <ewol/OObject.h>
#include <ewol/Texture.h>
#include <ewol/Font.h>
#include <ewol/ewol.h>
#include <ewol/importgl.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/eventInputManagement.h>



#undef __class__
#define __class__	"Windows"


//list of local events : 
extern const char * const ewolEventWindowsHideKeyboard   = "ewol Windows hideKeyboard";


ewol::Windows::Windows(void)
{
	SetCanHaveFocus(true);
	for(int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_subWidget[iii] = NULL;
	}
	SetDecorationDisable();
	//KeyboardShow(KEYBOARD_MODE_CODE);
}

ewol::Windows::~Windows(void)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->MarkToRemove();
		m_subWidget[m_currentCreateId]=NULL;
	}
	
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentCreateId].Size(); iii++) {
		if (NULL != m_popUpWidgetList[m_currentCreateId][iii]) {
			m_popUpWidgetList[m_currentCreateId][iii]->MarkToRemove();
			m_popUpWidgetList[m_currentCreateId][iii]=NULL;
		}
	}
	m_popUpWidgetList[m_currentCreateId].Clear();
}

//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_WINDOWS = "Windows";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Windows::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_WINDOWS << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_WINDOWS) {
		return true;
	} else {
		if(true == ewol::Widget::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_WINDOWS << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Windows::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_WINDOWS;
}


bool ewol::Windows::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("calculateMinSize on : " << m_currentCreateId);
	m_size.x = availlableX;
	m_size.y = availlableY;
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->CalculateMinSize();
		// TODO : Check if min Size is possible ...
		// TODO : Herited from MinSize .. and expand ???
		m_subWidget[m_currentCreateId]->CalculateSize(m_size.x, m_size.y);
	}
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentCreateId].Size(); iii++) {
		if (NULL != m_popUpWidgetList[m_currentCreateId][iii]) {
			m_popUpWidgetList[m_currentCreateId][iii]->CalculateMinSize();
			m_popUpWidgetList[m_currentCreateId][iii]->CalculateSize(m_size.x, m_size.y);
		}
	}
	return true;
}

/**
 * @brief Get the widget at the specific windows absolute position
 * @param[in] pos gAbsolute position of the requested widget knowledge
 * @return NULL No widget found
 * @return pointer on the widget found
 */
ewol::Widget * ewol::Windows::GetWidgetAtPos(Vector2D<float> pos)
{
	// calculate relative position
	Vector2D<float> relativePos = RelativePosition(pos);
	// event go directly on the pop-up
	if (0 < m_popUpWidgetList[m_currentCreateId].Size()) {
		if (NULL == m_popUpWidgetList[m_currentCreateId][m_popUpWidgetList[m_currentCreateId].Size()-1]) {
			m_popUpWidgetList[m_currentCreateId].PopBack();
		} else {
			return m_popUpWidgetList[m_currentCreateId][m_popUpWidgetList[m_currentCreateId].Size()-1]->GetWidgetAtPos(pos);
		}
	// otherwise in the normal windows
	} else if (NULL != m_subWidget[m_currentCreateId]) {
		return m_subWidget[m_currentCreateId]->GetWidgetAtPos(pos);
	}
	// otherwise the event go to this widget ...
	return this;
}


void ewol::Windows::SysDraw(void)
{

	//EWOL_DEBUG("Drow on (" << m_size.x << "," << m_size.y << ")");
	// set the size of the open GL system
	glViewport(0,0,m_size.x,m_size.y);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	ewol::DrawProperty displayProp;
	displayProp.m_windowsSize = m_size;
	displayProp.m_origin.x = 0;
	displayProp.m_origin.y = 0;
	displayProp.m_size = m_size;
	
	GenDraw(displayProp);

	glDisable(GL_BLEND);
	return;
}

void ewol::Windows::OnRegenerateDisplay(void)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->OnRegenerateDisplay();
	}
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentCreateId].Size(); iii++) {
		if (NULL != m_popUpWidgetList[m_currentCreateId][iii]) {
			m_popUpWidgetList[m_currentCreateId][iii]->OnRegenerateDisplay();
		}
	}
}


void ewol::Windows::OnDraw(ewol::DrawProperty& displayProp)
{

	// Clear the screen with transparency ...
	glClearColor(0.750, 0.750, 0.750, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//EWOL_WARNING(" WINDOWS draw on " << m_currentDrawId);
	// first display the windows on the display
	if (NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->GenDraw(displayProp);
		//EWOL_DEBUG("Draw Windows");
	}
	// second display the pop-up
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentDrawId].Size(); iii++) {
		if (NULL != m_popUpWidgetList[m_currentDrawId][iii]) {
			m_popUpWidgetList[m_currentDrawId][iii]->GenDraw(displayProp);
			//EWOL_DEBUG("Draw Pop-up");
		}
	}
}



void ewol::Windows::SetSubWidget(ewol::Widget * widget)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		EWOL_INFO("Remove current main windows Widget...");
		m_subWidget[m_currentCreateId]->MarkToRemove();
		m_subWidget[m_currentCreateId] = NULL;
	}
	m_subWidget[m_currentCreateId] = widget;
	// Regenerate the size calculation :
	CalculateSize(m_size.x, m_size.y);
	m_needFlipFlop = true;
}


void ewol::Windows::PopUpWidgetPush(ewol::Widget * widget)
{
	m_popUpWidgetList[m_currentCreateId].PushBack(widget);
	// Regenerate the size calculation :
	CalculateSize(m_size.x, m_size.y);
	m_needFlipFlop = true;
	ewol::eventInput::NewLayerSet();
}


/**
 * @brief Event generated to inform a flip-flop has occured on the current widget
 * @param ---
 * @return ---
 */
void ewol::Windows::OnFlipFlopEvent(void)
{
	bool needFlipFlop = m_needFlipFlop;
	// call herited classes
	ewol::Widget::OnFlipFlopEvent();
	// internal saving
	if (true == needFlipFlop) {
		m_subWidget[m_currentCreateId]       = m_subWidget[m_currentDrawId];
		m_popUpWidgetList[m_currentCreateId] = m_popUpWidgetList[m_currentDrawId];
	}
	// in every case, we propagate the flip-flop EVENT
	if (NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->OnFlipFlopEvent();
	}
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentDrawId].Size(); iii++) {
		if(NULL != m_popUpWidgetList[m_currentDrawId][iii]) {
			m_popUpWidgetList[m_currentDrawId][iii]->OnFlipFlopEvent();
		}
	}
}

/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void ewol::Windows::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::Widget::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	
	if (m_subWidget[m_currentCreateId] == removeObject) {
		EWOL_DEBUG("Remove main element of the windows ==> destroyed object");
		m_subWidget[m_currentCreateId] = NULL;
		m_needFlipFlop = true;
	}
	for(int32_t iii=m_popUpWidgetList[m_currentCreateId].Size()-1; iii>=0; iii--) {
		if(m_popUpWidgetList[m_currentCreateId][iii] == removeObject) {
			EWOL_DEBUG("Remove Pop-up [" << iii << "] element of the windows ==> destroyed object");
			m_popUpWidgetList[m_currentCreateId][iii] = NULL;
			m_popUpWidgetList[m_currentCreateId].Erase(iii);
			m_needFlipFlop = true;
		}
	}
}



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
#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/oObject/OObject.h>
#include <ewol/openGL/openGL.h>
#include <ewol/texture/Texture.h>
#include <ewol/font/Font.h>
#include <ewol/ewol.h>
#include <ewol/openGL/openGL.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/os/eSystem.h>



#undef __class__
#define __class__	"Windows"


//list of local events : 
extern const char * const ewolEventWindowsHideKeyboard   = "ewol Windows hideKeyboard";


ewol::Windows::Windows(void)
{
	SetCanHaveFocus(true);
	m_subWidget = NULL;
	SetDecorationDisable();
	//KeyboardShow(KEYBOARD_MODE_CODE);
}

ewol::Windows::~Windows(void)
{
	if (NULL != m_subWidget) {
		delete(m_subWidget);
		m_subWidget=NULL;
	}
	
	for(int32_t iii=0; iii<m_popUpWidgetList.Size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			delete(m_popUpWidgetList[iii]);
			m_popUpWidgetList[iii]=NULL;
		}
	}
	m_popUpWidgetList.Clear();
}


bool ewol::Windows::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("calculateMinSize on : " << m_currentCreateId);
	m_size.x = availlableX;
	m_size.y = availlableY;
	if (NULL != m_subWidget) {
		m_subWidget->CalculateMinSize();
		// TODO : Check if min Size is possible ...
		// TODO : Herited from MinSize .. and expand ???
		m_subWidget->CalculateSize(m_size.x, m_size.y);
	}
	for(int32_t iii=0; iii<m_popUpWidgetList.Size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			m_popUpWidgetList[iii]->CalculateMinSize();
			m_popUpWidgetList[iii]->CalculateSize(m_size.x, m_size.y);
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
	if (0 < m_popUpWidgetList.Size()) {
		if (NULL == m_popUpWidgetList[m_popUpWidgetList.Size()-1]) {
			m_popUpWidgetList.PopBack();
		} else {
			return m_popUpWidgetList[m_popUpWidgetList.Size()-1]->GetWidgetAtPos(pos);
		}
	// otherwise in the normal windows
	} else if (NULL != m_subWidget) {
		return m_subWidget->GetWidgetAtPos(pos);
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
	
	// clear the matrix system :
	etk::Matrix newOne;
	ewol::openGL::SetBasicMatrix(newOne);
	
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
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
	for(int32_t iii=0; iii<m_popUpWidgetList.Size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			m_popUpWidgetList[iii]->OnRegenerateDisplay();
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
	if (NULL != m_subWidget) {
		m_subWidget->GenDraw(displayProp);
		//EWOL_DEBUG("Draw Windows");
	}
	// second display the pop-up
	for(int32_t iii=0; iii<m_popUpWidgetList.Size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			m_popUpWidgetList[iii]->GenDraw(displayProp);
			//EWOL_DEBUG("Draw Pop-up");
		}
	}
}



void ewol::Windows::SetSubWidget(ewol::Widget * widget)
{
	if (NULL != m_subWidget) {
		EWOL_INFO("Remove current main windows Widget...");
		delete(m_subWidget);
		m_subWidget = NULL;
	}
	m_subWidget = widget;
	// Regenerate the size calculation :
	CalculateSize(m_size.x, m_size.y);
}


void ewol::Windows::PopUpWidgetPush(ewol::Widget * widget)
{
	m_popUpWidgetList.PushBack(widget);
	// Regenerate the size calculation :
	CalculateSize(m_size.x, m_size.y);
	// TODO : it is dansgerous to access directly to the system ...
	eSystem::ResetIOEvent();
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
	
	if (m_subWidget == removeObject) {
		EWOL_DEBUG("Remove main element of the windows ==> destroyed object");
		m_subWidget = NULL;
	}
	for(int32_t iii=m_popUpWidgetList.Size()-1; iii>=0; iii--) {
		if(m_popUpWidgetList[iii] == removeObject) {
			EWOL_DEBUG("Remove Pop-up [" << iii << "] element of the windows ==> destroyed object");
			m_popUpWidgetList[iii] = NULL;
			m_popUpWidgetList.Erase(iii);
		}
	}
}



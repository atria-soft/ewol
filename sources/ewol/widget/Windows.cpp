/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <etk/UString.h>
#include <ewol/ewol.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/os/eSystem.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/widget/WidgetManager.h>



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
	m_size.setValue(availlableX, availlableY);
	if (NULL != m_subWidget) {
		m_subWidget->CalculateMinSize();
		// TODO : Check if min Size is possible ...
		// TODO : Herited from MinSize .. and expand ???
		m_subWidget->CalculateSize(m_size.x(), m_size.y());
	}
	for(int32_t iii=0; iii<m_popUpWidgetList.Size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			m_popUpWidgetList[iii]->CalculateMinSize();
			m_popUpWidgetList[iii]->CalculateSize(m_size.x(), m_size.y());
		}
	}
	return true;
}


ewol::Widget * ewol::Windows::GetWidgetAtPos(vec2 pos)
{
	// calculate relative position
	vec2 relativePos = RelativePosition(pos);
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
	glViewport(0,0,m_size.x(),m_size.y());
	
	ewol::openGL::Disable(ewol::openGL::FLAG_DITHER);
	//ewol::openGL::Disable(ewol::openGL::FLAG_BLEND);
	ewol::openGL::Disable(ewol::openGL::FLAG_STENCIL_TEST);
	ewol::openGL::Disable(ewol::openGL::FLAG_ALPHA_TEST);
	ewol::openGL::Disable(ewol::openGL::FLAG_FOG);
	#ifndef __TARGET_OS__Android
		glPixelZoom(1.0,1.0);
	#endif
	ewol::openGL::Disable(ewol::openGL::FLAG_TEXTURE_2D);
	ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	
	ewol::openGL::Enable(ewol::openGL::FLAG_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// clear the matrix system :
	mat4 newOne;
	ewol::openGL::SetBasicMatrix(newOne);
	
	ewol::DrawProperty displayProp;
	displayProp.m_windowsSize = m_size;
	displayProp.m_origin.setValue(0,0);
	displayProp.m_size = m_size;
	
	GenDraw(displayProp);

	ewol::openGL::Disable(ewol::openGL::FLAG_BLEND);
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

//#define TEST_PERFO_WINDOWS

void ewol::Windows::OnDraw(ewol::DrawProperty& displayProp)
{
	#ifdef TEST_PERFO_WINDOWS
	int64_t ___startTime0 = ewol::GetTime();
	#endif
	
	// Clear the screen with transparency ...
	glClearColor(0.750, 0.750, 0.750, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime0 = (float)(ewol::GetTime() - ___startTime0) / 1000.0f;
	EWOL_ERROR("      Windows000  : " << ___localTime0 << "ms ");
	
	int64_t ___startTime1 = ewol::GetTime();
	#endif
	//EWOL_WARNING(" WINDOWS draw on " << m_currentDrawId);
	// first display the windows on the display
	if (NULL != m_subWidget) {
		m_subWidget->GenDraw(displayProp);
		//EWOL_DEBUG("Draw Windows");
	}
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime1 = (float)(ewol::GetTime() - ___startTime1) / 1000.0f;
	EWOL_ERROR("      Windows111  : " << ___localTime1 << "ms ");
	
	int64_t ___startTime2 = ewol::GetTime();
	#endif
	// second display the pop-up
	for(int32_t iii=0; iii<m_popUpWidgetList.Size(); iii++) {
		if (NULL != m_popUpWidgetList[iii]) {
			m_popUpWidgetList[iii]->GenDraw(displayProp);
			//EWOL_DEBUG("Draw Pop-up");
		}
	}
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime2 = (float)(ewol::GetTime() - ___startTime2) / 1000.0f;
	EWOL_ERROR("      Windows222  : " << ___localTime2 << "ms ");
	#endif
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
	CalculateSize(m_size.x(), m_size.y());
}


void ewol::Windows::PopUpWidgetPush(ewol::Widget * widget)
{
	m_popUpWidgetList.PushBack(widget);
	// Regenerate the size calculation :
	CalculateSize(m_size.x(), m_size.y());
	// TODO : it is dansgerous to access directly to the system ...
	eSystem::ResetIOEvent();
}


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



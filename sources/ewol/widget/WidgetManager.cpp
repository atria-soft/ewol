/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/widget/Joystick.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/ButtonColor.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Slider.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/ProgressBar.h>
#include <ewol/widget/Layer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Image.h>
#include <ewol/widget/Gird.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Scroll.h>
#include <ewol/widget/PopUp.h>
#include <etk/Vector.h>

#undef __class__
#define __class__	"WidgetManager"

ewol::WidgetManager::WidgetManager(void) :
	m_focusWidgetDefault(NULL),
	m_focusWidgetCurrent(NULL),
	m_havePeriodic(false),
	m_haveRedraw(true),
	m_applWakeUpTime(0),
	m_lastPeriodicCallTime(0)
{
	EWOL_DEBUG("==> Init Widget-Manager");
	// set the basic time properties :
	m_applWakeUpTime = ewol::GetTime();
	m_lastPeriodicCallTime = ewol::GetTime();
	
	widget::Button::Init(*this);
	widget::ButtonColor::Init(*this);
	widget::Spacer::Init(*this);
	widget::Slider::Init(*this);
	widget::Sizer::Init(*this);
	widget::ProgressBar::Init(*this);
	widget::Layer::Init(*this);
	widget::Label::Init(*this);
	widget::Image::Init(*this);
	widget::Gird::Init(*this);
	widget::Entry::Init(*this);
	widget::CheckBox::Init(*this);
	widget::Scroll::Init(*this);
	widget::ContextMenu::Init(*this);
	widget::PopUp::Init(*this);
}

ewol::WidgetManager::~WidgetManager(void)
{
	EWOL_DEBUG("==> Un-Init Widget-Manager");
	EWOL_INFO("Realease all FOCUS");
	FocusSetDefault(NULL);
	FocusRelease();
	
	m_listOfPeriodicWidget.Clear();
	m_creatorList.Clear();
}

void ewol::WidgetManager::Rm(ewol::Widget * newWidget)
{
	PeriodicCallRm(newWidget);
	FocusRemoveIfRemove(newWidget);
}

/* *************************************************************************
 * Focus Area : 
 * *************************************************************************/

void ewol::WidgetManager::FocusKeep(ewol::Widget * newWidget)
{
	if (NULL == newWidget) {
		// nothing to do ...
		return;
	}
	if (false == newWidget->CanHaveFocus()) {
		EWOL_VERBOSE("Widget can not have Focus, id=" << 9999999999.999);
		return;
	}
	if (newWidget == m_focusWidgetCurrent) {
		// nothing to do ...
		return;
	}
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Rm Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
		m_focusWidgetCurrent->RmFocus();
	}
	m_focusWidgetCurrent = newWidget;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
		m_focusWidgetCurrent->SetFocus();
	}
}


void ewol::WidgetManager::FocusSetDefault(ewol::Widget * newWidget)
{
	if (NULL != newWidget && false == newWidget->CanHaveFocus()) {
		EWOL_VERBOSE("Widget can not have Focus, id=" << newWidget->GetId() );
		return;
	}
	if (m_focusWidgetDefault == m_focusWidgetCurrent) {
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Rm Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
			m_focusWidgetCurrent->RmFocus();
		}
		m_focusWidgetCurrent = newWidget;
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Set Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
			m_focusWidgetCurrent->SetFocus();
		}
	}
	m_focusWidgetDefault = newWidget;
}


void ewol::WidgetManager::FocusRelease(void)
{
	if (m_focusWidgetDefault == m_focusWidgetCurrent) {
		// nothink to do ...
		return;
	}
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Rm Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
		m_focusWidgetCurrent->RmFocus();
	}
	m_focusWidgetCurrent = m_focusWidgetDefault;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
		m_focusWidgetCurrent->SetFocus();
	}
}


ewol::Widget * ewol::WidgetManager::FocusGet(void)
{
	return m_focusWidgetCurrent;
}

void ewol::WidgetManager::FocusRemoveIfRemove(ewol::Widget * newWidget)
{
	if (m_focusWidgetCurrent == newWidget) {
		EWOL_WARNING("Release Focus when remove widget");
		FocusRelease();
	}
	if (m_focusWidgetDefault == newWidget) {
		EWOL_WARNING("Release default Focus when remove widget");
		FocusSetDefault(NULL);
	}
}



void ewol::WidgetManager::PeriodicCallAdd(ewol::Widget * pWidget)
{
	for (int32_t iii=0; iii < m_listOfPeriodicWidget.Size(); iii++) {
		if (m_listOfPeriodicWidget[iii] == pWidget) {
			return;
		}
	}
	for (int32_t iii=0; iii < m_listOfPeriodicWidget.Size(); iii++) {
		if (NULL == m_listOfPeriodicWidget[iii]) {
			m_listOfPeriodicWidget[iii] = pWidget;
			return;
		}
	}
	m_listOfPeriodicWidget.PushBack(pWidget);
	m_havePeriodic = true;
}

void ewol::WidgetManager::PeriodicCallRm(ewol::Widget * pWidget)
{
	int32_t nbElement = 0;
	for (int32_t iii=m_listOfPeriodicWidget.Size()-1; iii>=0 ; iii--) {
		if (m_listOfPeriodicWidget[iii] == pWidget) {
			m_listOfPeriodicWidget[iii] = NULL;
		} else {
			nbElement++;
		}
	}
	if (0 == nbElement) {
		m_havePeriodic = false;
	}
}

void ewol::WidgetManager::PeriodicCall(int64_t _localTime)
{
	int64_t previousTime = m_lastPeriodicCallTime;
	m_lastPeriodicCallTime = _localTime;
	if (m_listOfPeriodicWidget.Size() <= 0) {
		return;
	}
	float deltaTime = (float)(_localTime - previousTime)/1000000.0;
	
	EventTime myTime(_localTime, m_applWakeUpTime, deltaTime, deltaTime);
	
	EWOL_VERBOSE("periodic : " << _localTime);
	for (int32_t iii=m_listOfPeriodicWidget.Size()-1; iii>=0 ; iii--) {
		if (NULL != m_listOfPeriodicWidget[iii]) {
			int64_t deltaTimeCallUser = m_listOfPeriodicWidget[iii]->SystemGetCallDeltaTime();
			if (deltaTimeCallUser<=0) {
				myTime.SetDeltaCall(deltaTime);
				EWOL_VERBOSE("[" << iii << "] periodic : " << myTime);
				m_listOfPeriodicWidget[iii]->SystemSetLastCallTime(_localTime);
				m_listOfPeriodicWidget[iii]->PeriodicCall(myTime);
			} else {
				int64_t lastCallTime = m_listOfPeriodicWidget[iii]->SystemGetLastCallTime();
				if (lastCallTime == 0) {
					lastCallTime = _localTime;
				}
				float deltaLocalTime = (float)(_localTime-lastCallTime)/1000000.0;;
				if (deltaLocalTime>= lastCallTime) {
					myTime.SetDeltaCall(deltaLocalTime);
					EWOL_VERBOSE("[" << iii << "] periodic : " << myTime);
					m_listOfPeriodicWidget[iii]->SystemSetLastCallTime(_localTime);
					m_listOfPeriodicWidget[iii]->PeriodicCall(myTime);
				}
			}
		}
	}
}

bool ewol::WidgetManager::PeriodicCallHave(void)
{
	return m_havePeriodic;
}


void ewol::WidgetManager::MarkDrawingIsNeeded(void)
{
	m_haveRedraw = true;
}

bool ewol::WidgetManager::IsDrawingNeeded(void)
{
	bool tmp = m_haveRedraw;
	m_haveRedraw = false;
	return tmp;
}



// element that generate the list of elements
void ewol::WidgetManager::AddWidgetCreator(const etk::UString& _name, ewol::WidgetManager::creator_tf _pointer)
{
	if (NULL==_pointer) {
		return;
	}
	if (true==m_creatorList.Exist(_name)) {
		EWOL_WARNING("Replace Creator of a specify widget : " << _name);
		m_creatorList[_name] = _pointer;
		return;
	}
	EWOL_INFO("Add Creator of a specify widget : " << _name);
	m_creatorList.Add(_name, _pointer);
}

ewol::Widget* ewol::WidgetManager::Create(const etk::UString& _name)
{
	if (true==m_creatorList.Exist(_name)) {
		ewol::WidgetManager::creator_tf pointerFunction = m_creatorList[_name];
		if (NULL != pointerFunction) {
			return pointerFunction();
		}
	}
	EWOL_WARNING("try to create an UnExistant widget : " << _name);
	return NULL;
}

bool ewol::WidgetManager::Exist(const etk::UString& _name)
{
	return m_creatorList.Exist(_name);
}

etk::UString ewol::WidgetManager::List(void)
{
	etk::UString tmpVal;
	for (int32_t iii=0; iii<m_creatorList.Size() ; iii++) {
		tmpVal += m_creatorList.GetKey(iii);
		tmpVal += ",";
	}
	return tmpVal;
}



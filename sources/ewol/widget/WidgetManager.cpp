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

static bool IsInit = false;

// For the focus Management
static ewol::Widget * m_focusWidgetDefault = NULL;
static ewol::Widget * m_focusWidgetCurrent = NULL;
static etk::Vector<ewol::Widget*> l_listOfPeriodicWidget;
static bool l_havePeriodic = false;

static bool l_haveRedraw = true;


class dataStructCreator
{
	public:
		etk::UString name;
		ewol::widgetManager::creator_tf pointer;
};

static etk::Vector<dataStructCreator> l_creatorList;

int64_t l_applWakeUpTime = 0; //!< Time of the application initialize
int64_t l_lastPeriodicCallTime = 0; //!< last call time ...

void ewol::widgetManager::Init(void)
{
	EWOL_DEBUG("==> Init Widget-Manager");
	// set the basic time properties :
	l_applWakeUpTime = ewol::GetTime();
	l_lastPeriodicCallTime = ewol::GetTime();
	
	// prevent android error ==> can create memory leak but I prefer
	m_focusWidgetDefault = NULL;
	m_focusWidgetCurrent = NULL;
	l_creatorList.Clear();
	l_listOfPeriodicWidget.Clear();
	l_havePeriodic = false;
	l_haveRedraw = true;
	//ewol::WIDGET_SceneInit();
	widget::Button::Init();
	widget::ButtonColor::Init();
	widget::Spacer::Init();
	widget::Slider::Init();
	widget::Sizer::Init();
	widget::ProgressBar::Init();
	widget::Layer::Init();
	widget::Label::Init();
	widget::Image::Init();
	widget::Gird::Init();
	widget::Entry::Init();
	widget::CheckBox::Init();
	widget::Scroll::Init();
	widget::ContextMenu::Init();
	widget::PopUp::Init();
	IsInit = true;
}

void ewol::widgetManager::UnInit(void)
{
	EWOL_DEBUG("==> Un-Init Widget-Manager");
	EWOL_INFO("Realease all FOCUS");
	ewol::widgetManager::FocusSetDefault(NULL);
	ewol::widgetManager::FocusRelease();
	
	widget::PopUp::UnInit();
	widget::ContextMenu::UnInit();
	widget::Scroll::UnInit();
	widget::CheckBox::UnInit();
	widget::Entry::UnInit();
	widget::Gird::UnInit();
	widget::Image::UnInit();
	widget::Label::UnInit();
	widget::Layer::UnInit();
	widget::ProgressBar::UnInit();
	widget::Sizer::UnInit();
	widget::Slider::UnInit();
	widget::Spacer::UnInit();
	widget::ButtonColor::UnInit();
	widget::Button::UnInit();
	
	IsInit = false;
	
	l_listOfPeriodicWidget.Clear();
	l_creatorList.Clear();
}

void ewol::widgetManager::Rm(ewol::Widget * newWidget)
{
	PeriodicCallRm(newWidget);
	FocusRemoveIfRemove(newWidget);
}

/* *************************************************************************
 * Focus Area : 
 * *************************************************************************/

void ewol::widgetManager::FocusKeep(ewol::Widget * newWidget)
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


void ewol::widgetManager::FocusSetDefault(ewol::Widget * newWidget)
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


void ewol::widgetManager::FocusRelease(void)
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


ewol::Widget * ewol::widgetManager::FocusGet(void)
{
	return m_focusWidgetCurrent;
}

void ewol::widgetManager::FocusRemoveIfRemove(ewol::Widget * newWidget)
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



void ewol::widgetManager::PeriodicCallAdd(ewol::Widget * pWidget)
{
	for (int32_t iii=0; iii < l_listOfPeriodicWidget.Size(); iii++) {
		if (l_listOfPeriodicWidget[iii] == pWidget) {
			return;
		}
	}
	for (int32_t iii=0; iii < l_listOfPeriodicWidget.Size(); iii++) {
		if (NULL == l_listOfPeriodicWidget[iii]) {
			l_listOfPeriodicWidget[iii] = pWidget;
			return;
		}
	}
	l_listOfPeriodicWidget.PushBack(pWidget);
	l_havePeriodic = true;
}

void ewol::widgetManager::PeriodicCallRm(ewol::Widget * pWidget)
{
	int32_t nbElement = 0;
	for (int32_t iii=l_listOfPeriodicWidget.Size()-1; iii>=0 ; iii--) {
		if (l_listOfPeriodicWidget[iii] == pWidget) {
			l_listOfPeriodicWidget[iii] = NULL;
		} else {
			nbElement++;
		}
	}
	if (0 == nbElement) {
		l_havePeriodic = false;
	}
}

void ewol::widgetManager::PeriodicCall(int64_t _localTime)
{
	int64_t previousTime = l_lastPeriodicCallTime;
	l_lastPeriodicCallTime = _localTime;
	if (l_listOfPeriodicWidget.Size() <= 0) {
		return;
	}
	float deltaTime = (float)(_localTime - previousTime)/1000000.0;
	
	EventTime myTime(_localTime, l_applWakeUpTime, deltaTime, deltaTime);
	
	EWOL_VERBOSE("periodic : " << _localTime);
	for (int32_t iii=l_listOfPeriodicWidget.Size()-1; iii>=0 ; iii--) {
		if (NULL != l_listOfPeriodicWidget[iii]) {
			int64_t deltaTimeCallUser = l_listOfPeriodicWidget[iii]->SystemGetCallDeltaTime();
			if (deltaTimeCallUser<=0) {
				myTime.SetDeltaCall(deltaTime);
				EWOL_VERBOSE("[" << iii << "] periodic : " << myTime);
				l_listOfPeriodicWidget[iii]->SystemSetLastCallTime(_localTime);
				l_listOfPeriodicWidget[iii]->PeriodicCall(myTime);
			} else {
				int64_t lastCallTime = l_listOfPeriodicWidget[iii]->SystemGetLastCallTime();
				if (lastCallTime == 0) {
					lastCallTime = _localTime;
				}
				float deltaLocalTime = (float)(_localTime-lastCallTime)/1000000.0;;
				if (deltaLocalTime>= lastCallTime) {
					myTime.SetDeltaCall(deltaLocalTime);
					EWOL_VERBOSE("[" << iii << "] periodic : " << myTime);
					l_listOfPeriodicWidget[iii]->SystemSetLastCallTime(_localTime);
					l_listOfPeriodicWidget[iii]->PeriodicCall(myTime);
				}
			}
		}
	}
}

bool ewol::widgetManager::PeriodicCallHave(void)
{
	return l_havePeriodic;
}


void ewol::widgetManager::MarkDrawingIsNeeded(void)
{
	l_haveRedraw = true;
}

bool ewol::widgetManager::IsDrawingNeeded(void)
{
	bool tmp = l_haveRedraw;
	l_haveRedraw = false;
	return tmp;
}



// element that generate the list of elements
void ewol::widgetManager::AddWidgetCreator(const etk::UString& name, ewol::widgetManager::creator_tf pointer)
{
	for (int32_t iii=0; iii<l_creatorList.Size() ; iii++) {
		if (name.CompareNoCase(l_creatorList[iii].name)) {
			if (NULL==pointer) {
				EWOL_INFO("Remove Creator of a specify widget : " << name);
			} else {
				EWOL_WARNING("Replace Creator of a specify widget : " << name);
				l_creatorList[iii].pointer = pointer;
			}
			return;
		}
	}
	if (NULL==pointer) {
		return;
	}
	EWOL_INFO("Add Creator of a specify widget : " << name);
	dataStructCreator tmpStruct;
	tmpStruct.name = name;
	tmpStruct.pointer = pointer;
	l_creatorList.PushBack(tmpStruct);
}

ewol::Widget* ewol::widgetManager::Create(const etk::UString& name)
{
	for (int32_t iii=0; iii<l_creatorList.Size() ; iii++) {
		if (name.CompareNoCase(l_creatorList[iii].name)) {
			if (NULL != l_creatorList[iii].pointer) {
				return (*l_creatorList[iii].pointer)();
			}
		}
	}
	EWOL_WARNING("try to create an UnExistant widget : " << name);
	return NULL;
}

bool ewol::widgetManager::Exist(const etk::UString& name)
{
	for (int32_t iii=0; iii<l_creatorList.Size() ; iii++) {
		if (name.CompareNoCase(l_creatorList[iii].name)) {
			return true;
		}
	}
	return false;
}

etk::UString ewol::widgetManager::List(void)
{
	etk::UString tmpVal;
	for (int32_t iii=0; iii<l_creatorList.Size() ; iii++) {
		tmpVal += l_creatorList[iii].name;
		tmpVal += ",";
	}
	return tmpVal;
}


